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

#include "core/WorldRules.h"

#include <QMetaEnum>

#include "core/LuaWorldRules.h"
#include "utils/Utils.h"

namespace warmonger {
namespace core {

QString rulesTypeToString(WorldRules::Type type)
{
    return utils::enumToString(type);
}

WorldRules::Type rulesTypeFromString(const QString& str)
{
    return utils::enumFromString<WorldRules::Type>(str);
}

std::unique_ptr<WorldRules> createWorldRules(core::World* world)
{
    switch (world->getRulesType())
    {
        case WorldRules::Type::Lua:
            return LuaWorldRules::make(world);
    }

    throw utils::ValueError("Cannot create world-rules for unknown rules type: " +
        QString::number(static_cast<int>(world->getRulesType())));
}

} // namespace core
} // namespace warmonger
