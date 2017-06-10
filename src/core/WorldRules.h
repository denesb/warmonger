/** \file
 * WorldRules interface.
 *
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

#ifndef W_CORE_WORLD_RULES_H
#define W_CORE_WORLD_RULES_H

namespace warmonger {
namespace core {

class Map;
class World;

/**
 * World-rules interface.
 *
 * World-rules provide a bridge between the game rules loaded from the world
 * and the game-engine.
 * All calls into the game rules go through this interface.
 */
class WorldRules
{
public:
    /**
     * Get the world.
     *
     * \returns the world
     */
    virtual World* getWorld() = 0;

    /**
     * Generate a map.
     *
     * \param size the radius of the map
     *
     * \returns the generated map
     */
    virtual std::unique_ptr<core::Map> generateMap(int size) = 0;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_WORLD_RULES_H
