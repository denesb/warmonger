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
    static QString get(lua_State* L, int index, record& tracking)
    {
        tracking.use(1);
        std::size_t len;
        const auto str = lua_tolstring(L, index, &len);
        return QString::fromLocal8Bit(str, len);
    }
};

template <>
struct pusher<QString>
{
    static int push(lua_State* L, const QString& str)
    {
        const QByteArray data = str.toLocal8Bit();
        lua_pushlstring(L, data.data(), data.size());
        return 1;
    }
};

template <>
struct getter<QColor>
{
    static QColor get(lua_State* L, int index, record& tracking)
    {
        tracking.use(1);
        std::size_t len;
        const auto str = lua_tolstring(L, index, &len);
        return QColor(QString::fromLocal8Bit(str, len));
    }
};

template <>
struct pusher<QColor>
{
    static int push(lua_State* L, const QColor& color)
    {
        const QByteArray data = color.name().toLocal8Bit();
        lua_pushlstring(L, data.data(), data.size());
        return 1;
    }
};

} // namespace stack

} // namespace sol

namespace warmonger {
namespace core {

static void exposeAPI(sol::state& lua, core::World* world);
static void exposeComponentTypes(sol::state& lua, const std::vector<ComponentType*>& componentTypes);
static void wLuaLog(sol::this_state ts, utils::LogLevel logLevel, const std::string& msg);

LuaWorldRules::LuaWorldRules(const QString& basePath, core::World* world)
    : world(world)
    , state(std::make_unique<sol::state>())
{
    sol::state& lua = *this->state.get();

    exposeAPI(lua, this->world);

    const auto entryPoint = utils::makePath(basePath, this->world->getRulesEntryPoint()).toStdString();

    wInfo << "Loading lua world rules from entry point " << entryPoint;

    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::table, sol::lib::math);

    lua.script_file(entryPoint);

    this->initHook = lua["init"];
    this->generateMapHook = lua["generate_map"];

    this->initHook(this->world);
}

std::unique_ptr<core::Map> LuaWorldRules::generateMap(unsigned int size)
{
    auto map{std::make_unique<core::Map>()};

    map->setWorld(world);
    map->generateMapNodes(size);

    this->generateMapHook(map.get(), size);

    return map;
}

static void exposeAPI(sol::state& lua, core::World* world)
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
        sol::property([](const MapNodeNeighbours& neighbours) { return neighbours.at(Direction::West); }),
        "north_west",
        sol::property([](const MapNodeNeighbours& neighbours) { return neighbours.at(Direction::NorthWest); }),
        "north_east",
        sol::property([](const MapNodeNeighbours& neighbours) { return neighbours.at(Direction::NorthEast); }),
        "east",
        sol::property([](const MapNodeNeighbours& neighbours) { return neighbours.at(Direction::East); }),
        "south_east",
        sol::property([](const MapNodeNeighbours& neighbours) { return neighbours.at(Direction::SouthEast); }),
        "south_west",
        sol::property([](const MapNodeNeighbours& neighbours) { return neighbours.at(Direction::SouthWest); }));

    lua.new_usertype<MapNode>("map_node",
        sol::meta_function::construct,
        sol::no_constructor,
        "neighbours",
        sol::property(&MapNode::getNeighbours));

    lua.new_usertype<Faction>("faction",
        sol::meta_function::construct,
        sol::no_constructor,
        "name",
        sol::property(&Faction::getName),
        "primaryColor",
        sol::property(&Faction::getPrimaryColor),
        "secondaryColor",
        sol::property(&Faction::getSecondaryColor),
        "banner",
        sol::property(&Faction::getBanner),
        "civilization",
        sol::property(&Faction::getCivilization));

    exposeComponentTypes(lua, world->getComponentTypes());

    lua.new_usertype<Entity>(
        "entity",
        sol::meta_function::construct,
        sol::no_constructor,
        "components",
        sol::property(&Entity::getComponents),
        "get_component",
        sol::overload([](Entity*, const ComponentType* const) { return sol::object(sol::nil); },
            [](Entity*, const QString&) { return sol::object(sol::nil); }),
        "create_component",
        &Entity::createComponent,
        "remove_component",
        sol::overload([](Entity* entity, const ComponentType* const type) { return entity->removeComponent(type); },
            [](Entity* entity, const QString& typeName) { return entity->removeComponent(typeName); }));

    lua.new_usertype<Map>("map",
        sol::meta_function::construct,
        sol::no_constructor,
        "name",
        sol::property(&Map::getName),
        "world",
        sol::property(&Map::getWorld),
        "map_nodes",
        sol::property(&Map::getMapNodes),
        "factions",
        sol::property(&Map::getFactions),
        "entities",
        sol::property(&Map::getEntities),
        "create_entity",
        [](Map* const map) { return map->createEntity(); });
}

static void exposeComponentTypes(sol::state& lua, const std::vector<ComponentType*>& componentTypes)
{
    for (const auto componentType : componentTypes)
    {
        wDebug << "Exposing component-type " << componentType->getName();
        auto& lcomponentType = lua.new_usertype<Component>(componentType->getName().toStdString());

        for (const auto field : componentType->getFields())
        {
            switch (field->getType()->id())
            {
                case Field::TypeId::Integer:
                    lcomponentType[field->getName().toStdString()] =
                        sol::property([](const Component* const component,
                                          const QString& name) { return component->getField(name).toInt(); },
                            [](Component* const component, const QString& name, int value) {
                                component->setField(name, value);
                            });
                    break;
                case Field::TypeId::Real:
                    // lcomponentType[field->getName().toStdString()] = sol::property(getRealFieldValue, setRealValue)
                    break;
                case Field::TypeId::String:
                    // lcomponentType[field->getName().toStdString()] = sol::property(getStringFieldValue,
                    // setStringValue)
                    break;
                case Field::TypeId::Reference:
                    // TODO: reference
                    break;
                case Field::TypeId::List:
                    // TODO: list
                    break;
                case Field::TypeId::Map:
                    // TODO: map
                    break;
            }
        }
    }
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

} // namespace core
} // namespace warmonger
