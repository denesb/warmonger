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

#include "ui/WorldSurfaceRules.h"

#include "core/Map.h"
#include "ui/LuaWorldSurfaceRules.h"
#include "ui/WorldSurface.h"
#include "utils/Utils.h"

namespace warmonger {
namespace ui {

WorldSurfaceRules::WorldSurfaceRules(WorldSurface& worldSurface)
    : worldSurface(worldSurface)
{
}

QString rulesTypeToString(WorldSurfaceRules::Type type)
{
    return utils::enumToString(type);
}

WorldSurfaceRules::Type rulesTypeFromString(const QString& str)
{
    return utils::enumFromString<WorldSurfaceRules::Type>(str);
}

std::unique_ptr<WorldSurfaceRules> createWorldSurfaceRules(WorldSurface& worldSurface)
{
    switch (worldSurface.getRulesType())
    {
        case WorldSurfaceRules::Type::Lua:
            return std::make_unique<LuaWorldSurfaceRules>(worldSurface);
    }

    throw utils::ValueError("Cannot create world surface rules for unknown rules type: " +
        QString::number(static_cast<int>(worldSurface.getRulesType())));
}

} // namespace ui
} // namespace warmonger
