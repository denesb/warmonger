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

#include "core/Map.h"
#include "core/Settlement.h"
#include "utils/Logging.h"
#include "utils/Lua.h"
#include "utils/Utils.h"

namespace sol {

template <>
struct is_container<::warmonger::core::MapNodeNeighbours> : std::false_type
{
};

} // namespace sol

namespace warmonger {
namespace core {

static void exposeAPI(sol::state& lua);

std::unique_ptr<WorldRules> LuaWorldRules::make(World* world)
{
    return std::make_unique<LuaWorldRules>(world);
}

LuaWorldRules::LuaWorldRules(core::World* world)
    : world(world)
    , state(std::make_unique<sol::state>())
{
    sol::state& lua = *this->state;

    utils::initLuaAPI(lua);
    exposeAPI(lua);

    lua["W"] = this->world;
}

LuaWorldRules::~LuaWorldRules()
{
}

void LuaWorldRules::loadRules(const QString& basePath, const QString& mainRulesFile)
{
    sol::state& lua = *this->state;

    utils::initLuaScript(lua, basePath, mainRulesFile);

    this->generateMapHook = lua["generate_random_map_content"];
    this->mapInitHook = lua["map_init"];

    lua["world_init"]();
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

static void exposeAPI(sol::state& lua)
{
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
        sol::property(&MapNode::getNeighbours),
        "terrain_type",
        sol::property(&MapNode::getTerrainType, &MapNode::setTerrainType));

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

    lua.new_usertype<Settlement>("settlement",
        sol::meta_function::construct,
        sol::no_constructor,
        "type",
        sol::property(&Settlement::getType, &Settlement::setType),
        "position",
        sol::property(&Settlement::getPosition, &Settlement::setPosition),
        "owner",
        sol::property(&Settlement::getOwner, &Settlement::setOwner));

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
        "settlements",
        sol::property(&Map::getSettlements),
        "create_settlement",
        [](Map* const map) { return map->createSettlement(); });
}

} // namespace core
} // namespace warmonger
