/** \file
 * LuaWorldRules class.
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

#ifndef W_CORE_LUA_WORLD_RULES_H
#define W_CORE_LUA_WORLD_RULES_H

#include "core/World.h"

namespace sol {
class state;
}

namespace warmonger {
namespace core {

class Map;

/**
 * World-rules implementation for rules written in Lua.
 */
class LuaWorldRules : public WorldRules
{
public:
    /**
     * Create and initialize the world rules object.
     *
     * Loads the rules associated with the world and initializes them.
     *
     * \param basePath base path for relative paths in the rules
     * \param world the world
     *
     * \throws IOError if the rules can't be loaded
     * \throws ValueError if the rules can't be parsed or initialization fails
     */
    LuaWorldRules(const QString& basePath, core::World* world);

    World* getWorld() override
    {
        return this->world;
    }

    std::unique_ptr<core::Map> generateMap(unsigned int size) override;

    void mapInit(Map* map) override;

private:
    World* world;
    std::unique_ptr<sol::state> state; // to avoid exposing the massive sol.hpp
    std::function<void()> worldInitHook;
    std::function<void(Map*, int)> generateMapHook;
    std::function<void(Map*)> mapInitHook;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_LUA_WORLD_RULES_H
