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

#include <set>

#include <QFileInfo>
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
    LuaWorldComponent(ComponentType* type, QObject* parent, ObjectId id);

    FieldValue* field(const QString& name) override;
    const FieldValue* field(const QString& name) const override;
    std::unordered_map<QString, FieldValue> getFields() const override;
    void setFields(std::unordered_map<QString, FieldValue> fields) override;

private:
    std::unordered_map<QString, FieldValue> fields;
};

static void exposeAPI(sol::state& lua);
static void wLuaLog(sol::this_state L, utils::LogLevel logLevel, const std::string& msg);
static sol::object referenceToLua(FieldValue& fieldValue, sol::this_state L);
static sol::object fieldValueToLua(FieldValue& fieldValue, sol::this_state L);
static FieldValue referenceFromLua(sol::object value);
static FieldValue fieldValueFromLua(sol::stack_object& value, sol::this_state L);
static FieldValue nestedFieldValueFromLua(sol::object value);
static sol::object fieldValueIndex(FieldValue* const fieldValue, sol::stack_object key, sol::this_state L);
static void fieldValueNewIndex(
    FieldValue* const fieldValue, sol::stack_object key, sol::stack_object value, sol::this_state L);
static sol::object fieldValueLength(FieldValue* const fieldValue, sol::this_state L);
static sol::object componentIndex(Component* const component, sol::stack_object key, sol::this_state L);
static bool isCompatibleType(const FieldValue& fieldValue, int type);
static bool assignValueToField(sol::stack_object& value, FieldValue& field, sol::this_state L);
static bool assignTableToListField(sol::stack_object& value, FieldValue& field, sol::this_state L);
static bool assignTableToMapField(sol::stack_object& value, FieldValue& field, sol::this_state L);
static void componentNewIndex(
    Component* const component, sol::stack_object key, sol::stack_object value, sol::this_state L);

class LuaExternalValue : public FieldValue::ExternalValue::Impl
{
public:
    LuaExternalValue(sol::object value)
        : value(std::move(value))
    {
    }

    FieldValue materialize() const override
    {
        wError << "UIMPLEMENTED";
        return FieldValue();
    }

    sol::object getValue() const
    {
        return value;
    }

private:
    sol::object value;
};

LuaWorldRules::LuaWorldRules(core::World* world)
    : world(world)
    , state(std::make_unique<sol::state>())
{
}

void LuaWorldRules::loadRules(const QString& basePath, const QString& mainRulesFile)
{
    sol::state& lua = *this->state.get();

    exposeAPI(lua);

    const auto path = utils::makePath(basePath, mainRulesFile);
    wInfo << "Loading lua world rules from entry point " << path;

    lua.open_libraries(
        sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::debug);

    lua["W"] = this->world;

    lua.script_file(path.toStdString());

    this->worldInitHook = lua["world_init"];
    this->generateMapHook = lua["generate_map"];
    this->mapInitHook = lua["map_init"];

    this->worldInitHook();
}

std::unique_ptr<Component> LuaWorldRules::createComponent(ComponentType* type, QObject* parent, ObjectId id)
{
    return std::make_unique<LuaWorldComponent>(type, parent, id);
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

LuaWorldComponent::LuaWorldComponent(ComponentType* type, QObject* parent, ObjectId id)
    : Component(type, parent, id)
{
    auto fieldDefs = type->getFields();
    for (auto& fieldDef : fieldDefs)
    {
        this->fields.emplace(fieldDef->getName(), FieldValue{fieldDef->getType()});
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
        "primary_color",
        sol::property(&Faction::getPrimaryColor, &Faction::setPrimaryColor),
        "secondary_color",
        sol::property(&Faction::getSecondaryColor, &Faction::setSecondaryColor),
        "banner",
        sol::property(&Faction::getBanner, &Faction::setBanner),
        "civilization",
        sol::property(&Faction::getCivilization, &Faction::setCivilization));

    lua.new_usertype<FieldValue>("field_value",
        sol::meta_function::construct,
        sol::no_constructor,
        sol::meta_function::index,
        fieldValueIndex,
        sol::meta_function::new_index,
        fieldValueNewIndex,
        sol::meta_function::length,
        fieldValueLength);

    lua.new_usertype<Component>("component",
        sol::meta_function::construct,
        sol::no_constructor,
        "type",
        sol::property(&Component::getType),
        sol::meta_function::index,
        componentIndex,
        sol::meta_function::new_index,
        componentNewIndex);

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

static void wLuaLog(sol::this_state L, utils::LogLevel logLevel, const std::string& msg)
{
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

static sol::object referenceToLua(FieldValue& fieldValue, sol::this_state L)
{
    auto ref = fieldValue.asReference();
    const auto& typeId = typeid(*ref);

    if (typeId == typeid(Banner))
        return sol::object(L, sol::in_place, static_cast<Banner*>(ref));
    else if (typeId == typeid(Civilization))
        return sol::object(L, sol::in_place, static_cast<Civilization*>(ref));
    else if (typeId == typeid(Component))
        return sol::object(L, sol::in_place, static_cast<Component*>(ref));
    else if (typeId == typeid(ComponentType))
        return sol::object(L, sol::in_place, static_cast<ComponentType*>(ref));
    else if (typeId == typeid(Entity))
        return sol::object(L, sol::in_place, static_cast<Entity*>(ref));
    else if (typeId == typeid(Faction))
        return sol::object(L, sol::in_place, static_cast<Faction*>(ref));
    else if (typeId == typeid(MapNode))
        return sol::object(L, sol::in_place, static_cast<MapNode*>(ref));

    wWarning << "Unknown WObject derived type: `" << typeId.name() << "'";
    return sol::object(L, sol::in_place, sol::lua_nil);
}

static sol::object fieldValueToLua(FieldValue& fieldValue, sol::this_state L)
{
    switch (fieldValue.getState())
    {
        case FieldValue::State::Null:
            return sol::object(L, sol::in_place, sol::lua_nil);
        case FieldValue::State::Integer:
            return sol::object(L, sol::in_place, fieldValue.asInteger());
        case FieldValue::State::Real:
            return sol::object(L, sol::in_place, fieldValue.asReal());
        case FieldValue::State::String:
            return sol::object(L, sol::in_place, fieldValue.asString());
        case FieldValue::State::Reference:
            return referenceToLua(fieldValue, L);
        case FieldValue::State::List:
        case FieldValue::State::Map:
            return sol::object(L, sol::in_place, &fieldValue);
        case FieldValue::State::External:
            return static_cast<LuaExternalValue*>(fieldValue.asExternalValue().getImpl())->getValue();
    }
    wWarning << "Uncrecognized field state " << fieldValue.getState();
    return sol::object(L, sol::in_place, sol::lua_nil);
}

static FieldValue referenceFromLua(sol::object value)
{
    if (auto v = value.as<sol::optional<Banner*>>())
        return FieldValue(*v);
    else if (auto v = value.as<sol::optional<Civilization*>>())
        return FieldValue(*v);
    else if (auto v = value.as<sol::optional<Component*>>())
        return FieldValue(*v);
    else if (auto v = value.as<sol::optional<ComponentType*>>())
        return FieldValue(*v);
    else if (auto v = value.as<sol::optional<Entity*>>())
        return FieldValue(*v);
    else if (auto v = value.as<sol::optional<Faction*>>())
        return FieldValue(*v);
    else if (auto v = value.as<sol::optional<MapNode*>>())
        return FieldValue(*v);

    wWarning << "Expected WObject derived userdata, got something else";
    return FieldValue();
}

static FieldValue fieldValueFromLua(sol::stack_object& value, sol::this_state L)
{
    switch (lua_type(L, value.stack_index()))
    {
        case LUA_TNUMBER:
            return FieldValue(value.as<double>());

        case LUA_TSTRING:
            return FieldValue(value.as<QString>());

        case LUA_TUSERDATA:
            if (value.is<FieldValue*>())
                return *value.as<FieldValue*>();
            return referenceFromLua(value);

        case LUA_TTABLE:
            return FieldValue::makeExternal<LuaExternalValue>(sol::object(L, value.stack_index()));

        default:
            wWarning << "Attempt to set value of incompatible type";
            return {};
    }
}

// Convert a nested Lua value (a table element) to a FieldValue:
// * Copy primitive types (number, boolean, string).
// * Copy userdata (we only pass pointers to lua anyway).
// * Keep "objects" (tables) in lua and keep a reference only.
static FieldValue nestedFieldValueFromLua(sol::object value)
{
    if (auto maybeNumber = value.as<sol::optional<double>>())
    {
        return FieldValue(*maybeNumber);
    }
    else if (auto maybeString = value.as<sol::optional<QString>>())
    {
        return FieldValue(*maybeString);
    }
    else if (value.is<sol::table>())
    {
        return FieldValue::makeExternal<LuaExternalValue>(std::move(value));
    }
    else
    {
        auto ref = referenceFromLua(value);
        if (!!ref)
            return ref;
    }
    wWarning << "Unknown type";
    return FieldValue();
}

static sol::object fieldValueIndex(FieldValue* const fieldValue, sol::stack_object key, sol::this_state L)
{
    if (!fieldValue->isList() && !fieldValue->isMap())
    {
        wWarning << "Attempt to index non container field";
        return sol::object(L, sol::in_place, sol::lua_nil);
    }

    if (fieldValue->isList())
    {
        auto maybeIndex = key.as<sol::optional<ssize_t>>();
        if (!maybeIndex)
        {
            wWarning << "Attempt to index list field with non-integer key";
            return sol::object(L, sol::in_place, sol::lua_nil);
        }

        // Lua array indexes start from 1
        const auto index = *maybeIndex - 1;
        auto& list = fieldValue->asList();
        if (index < 0 || static_cast<std::size_t>(index) >= list.size())
        {
            wWarning << "Index " << index << " is out of bounds";
            return sol::object(L, sol::in_place, sol::lua_nil);
        }

        return fieldValueToLua(list[index], L);
    }
    else
    {
        auto maybeKey = key.as<sol::optional<QString>>();
        if (!maybeKey)
        {
            wWarning << "Attempt to index map field with non-string key";
            return sol::object(L, sol::in_place, sol::lua_nil);
        }

        auto& map = fieldValue->asMap();
        auto it = map.find(*maybeKey);
        if (it == map.end())
        {
            return sol::object(L, sol::in_place, sol::lua_nil);
        }
        else
        {
            return fieldValueToLua(it->second, L);
        }
    }
}

static void fieldValueNewIndex(
    FieldValue* const fieldValue, sol::stack_object key, sol::stack_object value, sol::this_state L)
{
    if (!fieldValue->isList() && !fieldValue->isMap())
    {
        wWarning << "Attempt to new_index non container field";
        return;
    }

    if (fieldValue->isList())
    {
        auto maybeIndex = key.as<sol::optional<ssize_t>>();
        if (!maybeIndex)
        {
            wWarning << "Attempt to new_index list field with non-integer key";
            return;
        }

        // Lua indexes start from 1
        const auto index = *maybeIndex - 1;
        auto& list = fieldValue->asList();
        if (index < 0 || static_cast<std::size_t>(index) > list.size())
        {
            wWarning << "Index " << index << " is out of bounds";
            return;
        }

        if (static_cast<std::size_t>(index) == list.size())
            list.push_back(fieldValueFromLua(value, L));
        else
            list[index] = fieldValueFromLua(value, L);
    }
    else
    {
        auto maybeKey = key.as<sol::optional<QString>>();
        if (!maybeKey)
        {
            wWarning << "Attempt to index map field with non-string key";
            return;
        }

        auto& map = fieldValue->asMap();
        auto it = map.find(*maybeKey);
        if (it == map.end())
        {
            if (value != sol::nil)
                map.emplace(*maybeKey, fieldValueFromLua(value, L));
        }
        else
        {
            if (value == sol::nil)
                map.erase(it);
            else
                it->second = fieldValueFromLua(value, L);
        }
    }
}

static sol::object fieldValueLength(FieldValue* const fieldValue, sol::this_state L)
{
    if (fieldValue == nullptr)
    {
        wWarning << "Attempt to call length on nil field";
        return sol::object(L, sol::in_place, sol::lua_nil);
    }

    if (!fieldValue->isList() && !fieldValue->isMap())
    {
        wWarning << "Attempt to call length on non container field";
        return sol::object(L, sol::in_place, sol::lua_nil);
    }

    if (fieldValue->isList())
        return sol::object(L, sol::in_place, fieldValue->asList().size());
    else if (fieldValue->isMap())
        return sol::object(L, sol::in_place, fieldValue->asMap().size());

    return sol::object(L, sol::in_place, sol::lua_nil);
}

static sol::object componentIndex(Component* const component, sol::stack_object key, sol::this_state L)
{
    auto maybeFieldName = key.as<sol::optional<QString>>();
    if (!maybeFieldName)
    {
        wWarning << "Attempt to index field with non-string key";
        return sol::object(L, sol::in_place, sol::lua_nil);
    }

    auto value{component->field(*maybeFieldName)};

    if (value == nullptr || value->isNull())
        return sol::object(L, sol::in_place, sol::lua_nil);

    return fieldValueToLua(*value, L);
}

// Checks that `type' is compatible with the type of the component's fields as
// defined by the component-type.
// Type compatibility is not a guarantee that the assignment will be
// successfull as well, but it's a prerequisite.
static bool isCompatibleType(const FieldValue& fieldValue, int type)
{
    const auto fieldType = fieldValue.getType();
    return (type == LUA_TNUMBER && (fieldType == Field::Type::Integer || fieldType == Field::Type::Real)) ||
        (type == LUA_TSTRING && fieldType == Field::Type::String) ||
        (type == LUA_TUSERDATA && fieldType == Field::Type::Reference) ||
        (type == LUA_TTABLE && (fieldType == Field::Type::List || fieldType == Field::Type::Map));
}

// Assigns `value' to `field'.
// Assumes that the types are compatible, check this with isCompatibleType()
// before calling this function.
static bool assignValueToField(sol::stack_object& value, FieldValue& field, sol::this_state L)
{
    switch (field.getType())
    {
        case Field::Type::Integer:
            field = value.as<int>();
            return true;

        case Field::Type::Real:
            field = value.as<double>();
            return true;

        case Field::Type::String:
            field = value.as<QString>();
            return true;

        case Field::Type::Reference:
            field = referenceFromLua(value);
            return false;

        case Field::Type::List:
            return assignTableToListField(value, field, L);

        case Field::Type::Map:
            return assignTableToMapField(value, field, L);
    }
    wWarning << "Uncrecognized field value " << field.getType();
    return false;
}

// Assign a "list" table to a list field.
//
// We assume the table has integer-only indexes but we check anyway...
// Since tables are unordered first we need to sort the extracted elements
// to know ho much space to reserve for the list.
// Supports only convenional Lua lists:
// * Indexes must start from 1.
// * Indexes must be strictly increasing.
// * Indexes must be contiguous.
static bool assignTableToListField(sol::stack_object& value, FieldValue& field, sol::this_state)
{
    auto table = value.as<sol::table>();

    std::set<std::size_t> indexes;
    for (auto& element : table)
    {
        if (auto maybeIndex = element.first.as<sol::optional<std::size_t>>())
            indexes.insert(*maybeIndex);
    }

    if (indexes.size() != table.size())
    {
        wWarning << "Attempted to assign table with non-integer keys to a list";
        return false;
    }

    if (*indexes.begin() != 1 || *indexes.rbegin() != indexes.size())
    {
        wWarning << "Attempted to assign table with non-contiguous or non 1-started indexes to a list";
        return false;
    }

    auto& list = field.asList();
    list.clear();
    list.resize(table.size());

    for (auto& element : table)
    {
        // Lua array indexes start from 1
        list[element.first.as<std::size_t>() - 1] = nestedFieldValueFromLua(element.second);
    }
    return true;
}

// Assign a table to a map field.
//
// We are less picky here then with the list fields. The only requirement is
// that keys are convertible to string. Non-string keys are simply ignored.
static bool assignTableToMapField(sol::stack_object& value, FieldValue& field, sol::this_state)
{
    auto table = value.as<sol::table>();

    auto& map = field.asMap();
    map.clear();

    for (auto& element : table)
    {
        if (auto maybeKey = element.first.as<sol::optional<QString>>())
        {
            map[*maybeKey] = nestedFieldValueFromLua(element.second);
        }
        else
        {
            wWarning << "Ignoring non-string key while assigning table elements to map field";
            continue;
        }
    }
    return true;
}

static void componentNewIndex(
    Component* const component, sol::stack_object key, sol::stack_object value, sol::this_state L)
{
    auto maybeFieldName = key.as<sol::optional<QString>>();
    if (!maybeFieldName)
    {
        wWarning << "Attempt to new_index " << component->getType()->getName() << " component with non-string key";
        return;
    }
    auto fieldName = *maybeFieldName;
    auto field = component->field(fieldName);
    if (field == nullptr)
    {
        wWarning << "Attempt to assign to non-existing field `" << *maybeFieldName << "' of  "
                 << component->getType()->getName() << " component";
        return;
    }

    auto type = lua_type(L, value.stack_index());
    if (!isCompatibleType(*field, type))
    {
        wWarning << "Attempt to assign a value of incompatible type to field `" << fieldName << "' of "
                 << component->getType()->getName() << " component";
        return;
    }

    assignValueToField(value, *field, L);
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
