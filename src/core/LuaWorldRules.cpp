/**
 * \copyright (C) 2015-2017 Botond Dénes
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

#include <sol/sol.hpp>

#include "core/Map.h"
#include "utils/Logging.h"
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
public:
    LuaWorldComponent(ComponentType* type, QObject* parent, int id);

    FieldValue* field(const QString& name) override;
    const FieldValue* field(const QString& name) const override;
    std::unordered_map<QString, FieldValue> getFields() const override;
    void setFields(std::unordered_map<QString, FieldValue> fields) override;

private:
    std::unordered_map<QString, FieldValue> fields;
};

static void exposeAPI(sol::state& lua);
static void wLuaLog(sol::this_state ts, utils::LogLevel logLevel, const std::string& msg);
static FieldValue* getField(Component* const component, sol::stack_object key, sol::this_state L);
static void setField(Component* const component, sol::stack_object key, sol::stack_object value, sol::this_state L);

LuaWorldRules::LuaWorldRules(const QString& basePath, core::World* world)
    : world(world)
    , state(std::make_unique<sol::state>())
{
    sol::state& lua = *this->state.get();

    exposeAPI(lua);

    const auto entryPoint = utils::makePath(basePath, this->world->getRulesEntryPoint()).toStdString();

    wInfo << "Loading lua world rules from entry point " << entryPoint;

    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::table, sol::lib::math);

    lua["W"] = this->world;

    lua.script_file(entryPoint);

    this->worldInitHook = lua["world_init"];
    this->generateMapHook = lua["generate_map"];
    this->mapInitHook = lua["map_init"];

    this->worldInitHook();
}

std::unique_ptr<Component> LuaWorldRules::createComponent(ComponentType* type, int id)
{
    return std::make_unique<LuaWorldComponent>(type, nullptr, id);
}

std::unique_ptr<core::Map> LuaWorldRules::generateMap(unsigned int size)
{
    auto map{std::make_unique<core::Map>()};

    map->setWorld(world);
    map->generateMapNodes(size);

    this->generateMapHook(map.get(), size);

    return map;
}

void LuaWorldRules::mapInit(Map* map)
{
    this->mapInitHook(map);
}

LuaWorldComponent::LuaWorldComponent(ComponentType* type, QObject* parent, int id)
    : Component(type, parent, id)
{
    auto fieldDefs = type->getFields();
    for (auto& fieldDef : fieldDefs)
    {
        this->fields.emplace(fieldDef->getName(), FieldValue{});
    }
}

FieldValue* LuaWorldComponent::field(const QString& name)
{
    auto it = this->fields.find(name);
    if (it == this->fields.end())
    {
        wWarning << "Attempt to get value of non-existing field `" << name << "' from " << this->type->getName()
                 << " component";
        return nullptr;
    }
    return &it->second;
}

const FieldValue* LuaWorldComponent::field(const QString& name) const
{
    auto it = this->fields.find(name);
    if (it == this->fields.end())
    {
        wWarning << "Attempt to get value of non-existing field `" << name << "' from " << this->type->getName()
                 << " component";
        return nullptr;
    }
    return &it->second;
}

std::unordered_map<QString, FieldValue> LuaWorldComponent::getFields() const
{
    return this->fields;
}

void LuaWorldComponent::setFields(std::unordered_map<QString, FieldValue> fields)
{
    this->checkAndSetFields(std::move(fields), this->fields);
}

static void exposeAPI(sol::state& lua)
{
    lua.set_function(
        "w_trace", [](sol::this_state ts, const std::string& msg) { wLuaLog(ts, utils::LogLevel::Trace, msg); });
    lua.set_function(
        "w_debug", [](sol::this_state ts, const std::string& msg) { wLuaLog(ts, utils::LogLevel::Debug, msg); });
    lua.set_function(
        "w_info", [](sol::this_state ts, const std::string& msg) { wLuaLog(ts, utils::LogLevel::Info, msg); });
    lua.set_function(
        "w_warning", [](sol::this_state ts, const std::string& msg) { wLuaLog(ts, utils::LogLevel::Warning, msg); });
    lua.set_function(
        "w_error", [](sol::this_state ts, const std::string& msg) { wLuaLog(ts, utils::LogLevel::Error, msg); });

    lua.new_usertype<Civilization>("civilization",
        sol::meta_function::construct,
        sol::no_constructor,
        "name",
        sol::property(&Civilization::getName));

    lua.new_usertype<Banner>("banner",
        sol::meta_function::construct,
        sol::no_constructor,
        "name",
        sol::property(&Banner::getName),
        "civilizations",
        sol::property(&Banner::getCivilizations));

    lua.new_usertype<ComponentType>("component_type",
        sol::meta_function::construct,
        sol::no_constructor,
        "name",
        sol::property(&ComponentType::getName));

    lua.new_usertype<World>("world",
        sol::meta_function::construct,
        sol::no_constructor,
        "name",
        sol::property(&World::getName),
        "uuid",
        sol::property(&World::getUuid),
        "banners",
        sol::property(&World::getBanners),
        "civilizations",
        sol::property(&World::getCivilizations),
        "colors",
        sol::property(&World::getColors),
        "component_types",
        sol::property(&World::getComponentTypes));

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
        "primaryColor",
        sol::property(&Faction::getPrimaryColor, &Faction::setPrimaryColor),
        "secondaryColor",
        sol::property(&Faction::getSecondaryColor, &Faction::setSecondaryColor),
        "banner",
        sol::property(&Faction::getBanner, &Faction::setBanner),
        "civilization",
        sol::property(&Faction::getCivilization, &Faction::setCivilization));

    lua.new_usertype<Component>("component",
        sol::meta_function::construct,
        sol::no_constructor,
        "type",
        sol::property(&Component::getType),
        sol::meta_function::index,
        getField,
        sol::meta_function::new_index,
        setField);

    lua.new_usertype<Entity>("entity",
        sol::meta_function::construct,
        sol::no_constructor,
        "components",
        sol::property(&Entity::getComponents),
        "get_component",
        sol::overload(sol::resolve<Component*(const ComponentType* const)>(&Entity::getComponent),
            sol::resolve<Component*(const QString&)>(&Entity::getComponent)),
        "create_component",
        &Entity::createComponent,
        "remove_component",
        sol::overload([](Entity* entity, const ComponentType* const type) { return entity->removeComponent(type); },
            [](Entity* entity, const QString& typeName) { return entity->removeComponent(typeName); }));

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
        [](Map* const map) { return map->createEntity(); },
        "remove_entity",
        [](Map* const map, Entity* entity) { map->removeEntity(entity); });
}

static void wLuaLog(sol::this_state ts, utils::LogLevel logLevel, const std::string& msg)
{
    lua_State* L = ts;

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

static FieldValue* getField(Component* const component, sol::stack_object key, sol::this_state)
{
    auto maybeFieldName = key.as<sol::optional<QString>>();
    if (!maybeFieldName)
    {
        wWarning << "Attempt to index field with non-string key";
        return nullptr;
    }

    return component->field(*maybeFieldName);
}

static void setField(Component* const component, sol::stack_object key, sol::stack_object value, sol::this_state)
{
    auto maybeFieldName = key.as<sol::optional<QString>>();
    if (!maybeFieldName)
    {
        wWarning << "Attempt to index field with non-string key";
        return;
    }

    // TODO a more sofisticated type detection possibly using the Lua type enum
    if (auto maybeInteger = value.as<sol::optional<int>>())
    {
        component->field(*maybeFieldName)->set(*maybeInteger);
    }
    else if (auto maybeReal = value.as<sol::optional<double>>())
    {
        component->field(*maybeFieldName)->set(*maybeReal);
    }
    else if (auto maybeString = value.as<sol::optional<QString>>())
    {
        component->field(*maybeFieldName)->set(*maybeString);
    }
    else // TODO: reference, list, map
    {
        wWarning << "Attempt to set value of unsupported type to field `" << *maybeFieldName << "'";
    }
}

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
