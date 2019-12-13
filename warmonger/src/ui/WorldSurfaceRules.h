/** \file
 * WorldSurfaceRules interface.
 *
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

#ifndef W_UI_WORLD_SURFACE_RULES_H
#define W_UI_WORLD_SURFACE_RULES_H

#include <memory>

#include <QObject>

namespace warmonger {

namespace core {

class Map;
class MapNode;

} // namespace core

namespace ui {

namespace graphics {

struct GridTile
{
    // grid indexes
    int x;
    int y;
    // no. of grids spanned
    int width;
    int height;
    int assetId;
};

struct MapNodeLayer
{
    std::vector<GridTile> gridTiles;
};

struct MapNode
{
    core::MapNode* mapNode;
    std::vector<MapNodeLayer> layers;
};

struct Map
{
    std::vector<MapNode> mapNodes;
};

} // namespace graphics

class WorldSurface;

/**
 * World Surface Rules.
 *
 * Surface provided rules for rendering.
 */
class WorldSurfaceRules
{
    Q_GADGET

public:
    enum class Type
    {
        Lua
    };
    Q_ENUM(Type);

    WorldSurfaceRules(WorldSurface& worldSurface);

    virtual ~WorldSurfaceRules() = default;

    /**
     * Loads the rules from the path.
     *
     * Parses the rules and calls the init rule.
     * After this function returns the rules are ready to use.
     *
     * \param basePath the path to the rules directory
     * \param mainRulesFile the name of the rule's main file
     */
    virtual void loadRules(const QString& basePath, const QString& mainRulesFile) = 0;

    virtual graphics::Map renderMap(core::Map& map) = 0;

    WorldSurface& getWorldSurface() const
    {
        return this->worldSurface;
    }

private:
    WorldSurface& worldSurface;
};

/**
 * Convert the rules-type to string.
 *
 * \throws utils::ValueError if type is not a valid enum member.
 */
QString rulesTypeToString(WorldSurfaceRules::Type type);

/**
 * Parse the rules-type from a string.
 *
 * \throws utils::ValueError if the parsing fails
 */
WorldSurfaceRules::Type rulesTypeFromString(const QString& str);

/**
 * Creates the rules object for the world surface.
 *
 * This is a factory function that creates the appropriate rules object
 * for the world surface's rules-type. It creates and initializes the
 * rules object but the rules themselves are not loaded yet.
 * The rules object will be created as a children of the world surface.
 *
 * \throws ValueError if the rules can't be parsed or initialization fails
 */
std::unique_ptr<WorldSurfaceRules> createWorldSurfaceRules(WorldSurface& worldSurface);

} // namespace ui
} // namespace warmonger

#endif // W_UI_WORLD_SURFACE_RULES_H
