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

#include "ui/LuaWorldSurfaceRules.h"

#include "core/Map.h"
#include "core/Settlement.h"
#include "ui/WorldSurface.h"
#include "utils/Lua.h"

namespace sol {

template <>
struct is_container<::warmonger::core::MapNodeNeighbours> : std::false_type
{
};

} // namespace sol

namespace warmonger {
namespace ui {

static int getObjectId(const core::WObject& obj);
static void exposeAPI(sol::state& lua);

LuaWorldSurfaceRules::LuaWorldSurfaceRules(WorldSurface& worldSurface)
    : WorldSurfaceRules(worldSurface)
    , state(std::make_unique<sol::state>())
{
    sol::state& lua = *this->state;

    utils::initLuaAPI(lua);
    exposeAPI(lua);

    lua["WS"] = &this->getWorldSurface();
}

LuaWorldSurfaceRules::~LuaWorldSurfaceRules()
{
}

void LuaWorldSurfaceRules::loadRules(const QString& basePath, const QString& mainRulesFile)
{
    sol::state& lua = *this->state;

    utils::initLuaScript(lua, basePath, mainRulesFile);

    this->renderMapFunc = lua["render_map"];

    try
    {
        lua["init"]();
    }
    catch (sol::error& e)
    {
        throw utils::ScriptError(fmt::format("LuaWorldSurfaceRules: init() failed: {}", e.what()));
    }
}

graphics::Map LuaWorldSurfaceRules::renderMap(core::Map& map)
{
    try
    {
        return this->renderMapFunc(map);
    }
    catch (sol::error& e)
    {
        throw utils::ScriptError(fmt::format("LuaWorldSurfaceRules: render_map() failed: {}", e.what()));
    }
}

static int getObjectId(const core::WObject& obj)
{
    return obj.getId().get();
}

static void exposeAPI(sol::state& lua)
{
    lua.new_usertype<core::Banner>(
        "banner", sol::meta_function::construct, sol::no_constructor, "name", sol::property(&core::Banner::getName));

    lua.new_usertype<core::Civilization>("civilization",
        sol::meta_function::construct,
        sol::no_constructor,
        "name",
        sol::property(&core::Civilization::getName));

    lua.new_usertype<core::Color>(
        "color", sol::meta_function::construct, sol::no_constructor, "name", sol::property(&core::Color::getName));

    lua.new_usertype<core::World>("world",
        sol::meta_function::construct,
        sol::no_constructor,
        "name",
        sol::property(&core::World::getName),
        "uuid",
        sol::property(&core::World::getUuid),
        "banners",
        sol::property(&core::World::getBanners),
        "civilizations",
        sol::property(&core::World::getCivilizations),
        "colors",
        sol::property(&core::World::getColors));

    lua.new_usertype<core::MapNodeNeighbours>("map_node_neighbours",
        sol::meta_function::construct,
        sol::no_constructor,
        "west",
        sol::property([](const core::MapNodeNeighbours& neighbours) { return neighbours.at(core::Direction::West); }),
        "north_west",
        sol::property(
            [](const core::MapNodeNeighbours& neighbours) { return neighbours.at(core::Direction::NorthWest); }),
        "north_east",
        sol::property(
            [](const core::MapNodeNeighbours& neighbours) { return neighbours.at(core::Direction::NorthEast); }),
        "east",
        sol::property([](const core::MapNodeNeighbours& neighbours) { return neighbours.at(core::Direction::East); }),
        "south_east",
        sol::property(
            [](const core::MapNodeNeighbours& neighbours) { return neighbours.at(core::Direction::SouthEast); }),
        "south_west",
        sol::property(
            [](const core::MapNodeNeighbours& neighbours) { return neighbours.at(core::Direction::SouthWest); }));

    lua.new_usertype<core::MapNode>("map_node",
        sol::meta_function::construct,
        sol::no_constructor,
        "id",
        sol::property(getObjectId),
        "neighbours",
        sol::property(&core::MapNode::getNeighbours),
        "terrain_type",
        sol::property(&core::MapNode::getTerrainType));

    lua.new_usertype<core::Faction>("faction",
        sol::meta_function::construct,
        sol::no_constructor,
        "id",
        sol::property(getObjectId),
        "name",
        sol::property(&core::Faction::getName),
        "primary_color",
        sol::property(&core::Faction::getPrimaryColor),
        "secondary_color",
        sol::property(&core::Faction::getSecondaryColor),
        "banner",
        sol::property(&core::Faction::getBanner),
        "civilization",
        sol::property(&core::Faction::getCivilization));

    lua.new_usertype<core::Settlement>("settlement",
        sol::meta_function::construct,
        sol::no_constructor,
        "id",
        sol::property(getObjectId),
        "type",
        sol::property(&core::Settlement::getType),
        "position",
        sol::property(&core::Settlement::getPosition),
        "owner",
        sol::property(&core::Settlement::getOwner));

    lua.new_usertype<core::Map>("map",
        sol::meta_function::construct,
        sol::no_constructor,
        "name",
        sol::property(&core::Map::getName),
        "world",
        sol::property(&core::Map::getWorld),
        "map_nodes",
        sol::property(&core::Map::getMapNodes),
        "factions",
        sol::property(&core::Map::getFactions),
        "settlements",
        sol::property(&core::Map::getSettlements));

    lua.new_usertype<graphics::GridTile>("graphic_grid_tile",
        "x",
        &graphics::GridTile::x,
        "y",
        &graphics::GridTile::y,
        "width",
        &graphics::GridTile::width,
        "height",
        &graphics::GridTile::height,
        "asset_id",
        &graphics::GridTile::assetId);

    lua.new_usertype<graphics::MapNodeLayer>(
        "graphic_map_node_layer", "grid_tiles", &graphics::MapNodeLayer::gridTiles);

    lua.new_usertype<graphics::MapNode>(
        "graphic_map_node", "map_node", &graphics::MapNode::mapNode, "layers", &graphics::MapNode::layers);

    lua.new_usertype<graphics::Map>("graphic_map", "map_nodes", &graphics::Map::mapNodes);

    lua.new_usertype<WorldSurface>("world_surface",
        sol::meta_function::construct,
        sol::no_constructor,
        "name",
        sol::property(&WorldSurface::getName),
        "tile_size",
        sol::property(&WorldSurface::getTileSize),
        "grid_size",
        sol::property(&WorldSurface::getGridSize),
        "get_asset_id_for",
        &WorldSurface::getAssetIdFor);
}

} // namespace ui
} // namespace warmonger
