/**
 * \copyright (C) 2015-2018 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "core/LuaWorldRules.h"

#include <set>

#include <QFileInfo>
#include <sol/sol.hpp>

#include "core/Map.h"
#include "utils/Logging.h"
#include "utils/PathBuilder.h"
#include "utils/Utils.h"

namespace sol {

template <>
struct lua_type_of<QString> : std::integral_constant<sol::type, sol::type::string>
{
};

template <>
struct lua_type_of<QColor> : std::integral_constant<sol::type, sol::type::string>
{
};

template <>
struct is_container<::warmonger::core::MapNodeNeighbours> : std::false_type
{
};

namespace stack {

template <>
struct getter<QString>
{
    static QString get(lua_State* L, int index, record& tracking);
};

template <>
struct pusher<QString>
{
    static int push(lua_State* L, const QString& str);
};

template <>
struct getter<QColor>
{
    static QColor get(lua_State* L, int index, record& tracking);
};

template <>
struct pusher<QColor>
{
    static int push(lua_State* L, const QColor& color);
};

} // namespace stack
} // namespace sol

namespace warmonger {
namespace core {

class LuaWorldComponent : public Component
{
    Q_OBJECT

public:
    LuaWorldComponent(QString name, sol::state& L, QObject* parent);

    LuaWorldComponent(ir::Value v, sol::state& L, QObject* parent);

    ir::Value serialize() const override;

    const QString& getName() const override
    {
        return this->name;
    }

    bool isBuiltIn() const override
    {
        return false;
    }

    sol::table& getTable()
    {
        return this->table;
    }

    sol::object index(sol::stack_object key, sol::this_state);
    void newIndex(sol::stack_object key, sol::stack_object value, sol::this_state);

private:
    sol::state& L;
    sol::table table;
    QString name;
};

static void exposeAPI(sol::state& lua);
static void wLuaLog(sol::this_state L, utils::LogLevel logLevel, const std::string& msg);
static sol::object createComponent(Entity* const entity, QString name, sol::this_state L);
static sol::object entityIndex(Entity* const entity, sol::stack_object key, sol::this_state L);
static void loadWorldModule(sol::state& lua, const QString& basePath, const QString& moduleName);

std::unique_ptr<WorldRules> LuaWorldRules::make(World* world)
{
    return std::make_unique<LuaWorldRules>(world);
}

LuaWorldRules::LuaWorldRules(core::World* world)
    : world(world)
    , state(std::make_unique<sol::state>())
{
}

LuaWorldRules::~LuaWorldRules()
{
}

void LuaWorldRules::loadRules(const QString& basePath, const QString& mainRulesFile)
{
    this->basePath = basePath;
    sol::state& lua = *this->state;

    exposeAPI(lua);

    const QString path = this->basePath / mainRulesFile;
    wInfo << "Loading lua world rules from entry point " << path;

    lua.open_libraries(
        sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::debug);

    sol::function tableInsert = lua["table"]["insert"];
    tableInsert(lua["package"]["searchers"], [this](sol::stack_object moduleName) {
        return std::function<void()>([ this, moduleName = QString(moduleName.as<const char*>()) ] {
            loadWorldModule(*this->state, this->basePath, moduleName);
        });
    });

    lua["W"] = this->world;

    lua.script_file(path.toStdString());

    this->worldInitHook = lua["world_init"];
    this->generateMapHook = lua["generate_random_map_content"];
    this->mapInitHook = lua["map_init"];

    this->worldInitHook();
}

std::unique_ptr<Component> LuaWorldRules::createComponent(QString name, QObject* parent)
{
    return std::make_unique<LuaWorldComponent>(std::move(name), *this->state, parent);
}

std::unique_ptr<Component> LuaWorldRules::createComponent(ir::Value v, QObject* parent)
{
    return std::make_unique<LuaWorldComponent>(std::move(v), *this->state, parent);
}

std::unique_ptr<core::Map> LuaWorldRules::generateMap(
    int seed, unsigned int size, std::vector<std::unique_ptr<Faction>> players)
{
    auto map{std::make_unique<core::Map>()};

    map->setName("New Random Map");

    map->setWorld(world);
    map->generateMapNodes(size);

    for (auto& player : players)
    {
        player->setParent(map.get());
        map->addFaction(std::move(player));
    }

    this->generateMapHook(map.get(), seed, size);

    return map;
}

void LuaWorldRules::mapInit(Map* map)
{
    this->mapInitHook(map);
}

LuaWorldComponent::LuaWorldComponent(QString name, sol::state& L, QObject* parent)
    : Component(parent)
    , L(L)
    , table(L, sol::create)
    , name(name)
{
}

LuaWorldComponent::LuaWorldComponent(ir::Value v, sol::state& L, QObject* parent)
    : Component(parent, v.getObjectId())
    , L(L)
    , table(L, sol::create)
{
    auto obj = std::move(v).asObject();

    this->name = std::move(obj["name"]).asString();

    // FIXME call into the rules to unserialize
}

ir::Value LuaWorldComponent::serialize() const
{
    std::unordered_map<QString, ir::Value> obj;

    obj.emplace("name", this->name);

    // FIXME call into the rules to serialize

    return obj;
}

sol::object LuaWorldComponent::index(sol::stack_object key, sol::this_state)
{
    return this->table[key];
}

void LuaWorldComponent::newIndex(sol::stack_object key, sol::stack_object value, sol::this_state)
{
    this->table[key] = value;
}

static void exposeAPI(sol::state& lua)
{
    lua.set_function(
        "w_trace", [](sol::this_state L, const std::string& msg) { wLuaLog(L, utils::LogLevel::Trace, msg); });
    lua.set_function(
        "w_debug", [](sol::this_state L, const std::string& msg) { wLuaLog(L, utils::LogLevel::Debug, msg); });
    lua.set_function(
        "w_info", [](sol::this_state L, const std::string& msg) { wLuaLog(L, utils::LogLevel::Info, msg); });
    lua.set_function(
        "w_warning", [](sol::this_state L, const std::string& msg) { wLuaLog(L, utils::LogLevel::Warning, msg); });
    lua.set_function(
        "w_error", [](sol::this_state L, const std::string& msg) { wLuaLog(L, utils::LogLevel::Error, msg); });

    lua.new_usertype<Civilization>("civilization",
        sol::meta_function::construct,
        sol::no_constructor,
        "name",
        sol::property(&Civilization::getName));

    lua.new_usertype<Banner>(
        "banner", sol::meta_function::construct, sol::no_constructor, "name", sol::property(&Banner::getName));

    lua.new_usertype<World>("world",
        sol::meta_function::construct,
        sol::no_constructor,
        "name",
        sol::property(&World::getName),
        "uuid",
        sol::property(&World::getUuid),
        "create_banner",
        &World::createBanner,
        "banners",
        sol::property(&World::getBanners),
        "create_civilization",
        &World::createCivilization,
        "civilizations",
        sol::property(&World::getCivilizations),
        "create_color",
        &World::createColor,
        "colors",
        sol::property(&World::getColors));

    lua.new_usertype<MapNodeNeighbours>("map_node_neighbours",
        sol::meta_function::construct,
        sol::no_constructor,
        "west",
        sol::property([](const MapNodeNeighbours& neighbours) { return neighbours.at(Direction::West); },
            [](MapNodeNeighbours& neighbours, MapNode* neighbour) { return neighbours[Direction::West] = neighbour; }),
        "north_west",
        sol::property([](const MapNodeNeighbours& neighbours) { return neighbours.at(Direction::NorthWest); },
            [](MapNodeNeighbours& neighbours, MapNode* neighbour) {
                return neighbours[Direction::NorthWest] = neighbour;
            }),
        "north_east",
        sol::property([](const MapNodeNeighbours& neighbours) { return neighbours.at(Direction::NorthEast); },
            [](MapNodeNeighbours& neighbours, MapNode* neighbour) {
                return neighbours[Direction::NorthEast] = neighbour;
            }),
        "east",
        sol::property([](const MapNodeNeighbours& neighbours) { return neighbours.at(Direction::East); },
            [](MapNodeNeighbours& neighbours, MapNode* neighbour) { return neighbours[Direction::East] = neighbour; }),
        "south_east",
        sol::property([](const MapNodeNeighbours& neighbours) { return neighbours.at(Direction::SouthEast); },
            [](MapNodeNeighbours& neighbours, MapNode* neighbour) {
                return neighbours[Direction::SouthEast] = neighbour;
            }),
        "south_west",
        sol::property([](const MapNodeNeighbours& neighbours) { return neighbours.at(Direction::SouthWest); },
            [](MapNodeNeighbours& neighbours, MapNode* neighbour) {
                return neighbours[Direction::SouthWest] = neighbour;
            }));

    lua.new_usertype<MapNode>("map_node",
        sol::meta_function::construct,
        sol::no_constructor,
        "neighbours",
        sol::property(&MapNode::getNeighbours));

    lua.new_usertype<Faction>("faction",
        sol::meta_function::construct,
        sol::no_constructor,
        "name",
        sol::property(&Faction::getName, &Faction::setName),
        "primary_color",
        sol::property(&Faction::getPrimaryColor, &Faction::setPrimaryColor),
        "secondary_color",
        sol::property(&Faction::getSecondaryColor, &Faction::setSecondaryColor),
        "banner",
        sol::property(&Faction::getBanner, &Faction::setBanner),
        "civilization",
        sol::property(&Faction::getCivilization, &Faction::setCivilization));

    lua.new_usertype<PositionComponent>("position_component",
        sol::meta_function::construct,
        sol::no_constructor,
        "name",
        sol::property(&PositionComponent::getName),
        "map_node",
        sol::property(&PositionComponent::getMapNode, &PositionComponent::setMapNode));

    lua.new_usertype<GraphicsComponent>("graphics_component",
        sol::meta_function::construct,
        sol::no_constructor,
        "name",
        sol::property(&GraphicsComponent::getName),
        "path",
        sol::property(&GraphicsComponent::getPath, &GraphicsComponent::setPath),
        "x",
        sol::property(&GraphicsComponent::getX, &GraphicsComponent::setX),
        "y",
        sol::property(&GraphicsComponent::getY, &GraphicsComponent::setY),
        "z",
        sol::property(&GraphicsComponent::getZ, &GraphicsComponent::setZ),
        "container",
        sol::property(&GraphicsComponent::getContainer, &GraphicsComponent::setContainer));

    lua.new_usertype<LuaWorldComponent>("lua_world_component",
        sol::meta_function::construct,
        sol::no_constructor,
        "name",
        sol::property(&Component::getName),
        sol::meta_function::index,
        &LuaWorldComponent::index,
        sol::meta_function::new_index,
        &LuaWorldComponent::newIndex);

    lua.new_usertype<Entity>("entity",
        sol::meta_function::construct,
        sol::no_constructor,
        "name",
        sol::property(&Entity::getName),
        "components",
        sol::property(&Entity::getComponents),
        "position",
        sol::property(&Entity::getPositionComponent),
        "graphics",
        sol::property(&Entity::getGraphicsComponent),
        sol::meta_function::index,
        entityIndex,
        "create_component",
        createComponent,
        "remove_component",
        &Entity::removeComponent);

    lua.new_usertype<Map>("map",
        sol::meta_function::construct,
        sol::no_constructor,
        "name",
        sol::property(&Map::getName, &Map::setName),
        "world",
        sol::property(&Map::getWorld),
        "map_nodes",
        sol::property(&Map::getMapNodes),
        "create_map_node",
        [](Map* const map) { return map->createMapNode(); },
        "remove_map_node",
        [](Map* const map, MapNode* mapNode) { map->removeMapNode(mapNode); },
        "factions",
        sol::property(&Map::getFactions),
        "create_faction",
        [](Map* const map) { return map->createFaction(); },
        "remove_faction",
        [](Map* const map, Faction* faction) { map->removeFaction(faction); },
        "entities",
        sol::property(&Map::getEntities),
        "create_entity",
        [](Map* const map, QString name) { return map->createEntity(name); },
        "remove_entity",
        [](Map* const map, Entity* entity) { map->removeEntity(entity); });
}

static void wLuaLog(sol::this_state L, utils::LogLevel logLevel, const std::string& msg)
{
    // TODO: trim source files
    lua_Debug info;
    int level = 1;
    const int pre_stack_size = lua_gettop(L);

    if (lua_getstack(L, level, &info) != 1)
    {
        wError << "Unable to traverse the Lua stack";
        lua_settop(L, pre_stack_size);

        utils::LogEntry(logLevel, "", "", -1) << msg;
    }
    else if (lua_getinfo(L, "nlS", &info) == 0)
    {
        wError << "Unable to get Lua debug info";
        lua_settop(L, pre_stack_size);

        utils::LogEntry(logLevel, "", "", -1) << msg;
    }
    else
    {
        utils::LogEntry(logLevel, info.short_src, info.name, info.currentline) << msg;
    }
}

static sol::object createComponent(Entity* const entity, QString name, sol::this_state L)
{
    auto* c = entity->createComponent(std::move(name));

    if (c->isBuiltIn())
    {
        if (c->getName() == PositionComponent::name)
            return sol::object(L, sol::in_place, static_cast<PositionComponent*>(c));
        else if (c->getName() == GraphicsComponent::name)
            return sol::object(L, sol::in_place, static_cast<GraphicsComponent*>(c));
        else
            abort(); // Should never get here
    }
    else
    {
        return sol::object(L, sol::in_place, static_cast<LuaWorldComponent*>(c));
    }
}

static sol::object entityIndex(Entity* const entity, sol::stack_object key, sol::this_state L)
{
    auto maybeComponentName = key.as<sol::optional<QString>>();
    if (!maybeComponentName)
    {
        wWarning.format("Attempt to index entity with non-string key");
        return sol::object(L, sol::in_place, sol::lua_nil);
    }

    auto* component = entity->getComponent(*maybeComponentName);

    if (!component)
        return sol::object(L, sol::in_place, sol::lua_nil);

    assert(!component->isBuiltIn());

    // At this point we can safely assume we have a LuaWorldComponent.
    // The entity has specific properties for all the built-in components.
    // If this cast fails that is a bug.
    return sol::object(L, sol::in_place, static_cast<LuaWorldComponent*>(component));
}

static void loadWorldModule(sol::state& lua, const QString& basePath, const QString& moduleName)
{
    const QString moduleFile = basePath / moduleName + ".lua";
    wInfo.format("Loading required module `{}' from `{}'", moduleName, moduleFile);
    lua.script_file(moduleFile.toStdString());
}

#include "LuaWorldRules.moc"

} // namespace core
} // namespace warmonger

namespace sol {
namespace stack {

QString getter<QString>::get(lua_State* L, int index, record& tracking)
{
    tracking.use(1);
    std::size_t len;
    const auto str = lua_tolstring(L, index, &len);
    return QString::fromLocal8Bit(str, len);
}

int pusher<QString>::push(lua_State* L, const QString& str)
{
    const QByteArray data = str.toLocal8Bit();
    lua_pushlstring(L, data.data(), data.size());
    return 1;
}

QColor getter<QColor>::get(lua_State* L, int index, record& tracking)
{
    tracking.use(1);
    std::size_t len;
    const auto str = lua_tolstring(L, index, &len);
    return QColor(QString::fromLocal8Bit(str, len));
}

int pusher<QColor>::push(lua_State* L, const QColor& color)
{
    const QByteArray data = color.name().toLocal8Bit();
    lua_pushlstring(L, data.data(), data.size());
    return 1;
}

} // namespace stack
} // namespace sol
