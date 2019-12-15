/** \file
 * LuaWorldSurfaceRules class.
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

#ifndef W_UI_LUA_WORLD_SURFACE_RULES_H
#define W_UI_LUA_WORLD_SURFACE_RULES_H

#include <functional>

#include "ui/WorldSurfaceRules.h"

namespace sol {
class state;
}

namespace warmonger {
namespace ui {

/**
 * Lua World Surface Rules.
 */
class LuaWorldSurfaceRules : public WorldSurfaceRules
{
public:
    LuaWorldSurfaceRules(WorldSurface& worldSurface);
    ~LuaWorldSurfaceRules();

    void loadRules(const QString& basePath, const QString& mainRulesFile) override;

    graphics::Map renderMap(core::Map& map) override;

private:
    std::unique_ptr<sol::state> state; // to avoid exposing the massive sol.hpp
    std::function<graphics::Map(core::Map& map)> renderMapFunc;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_LUA_WORLD_SURFACE_RULES_H
