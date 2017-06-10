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

#include "core/WorldRules.h"

#include "core/LuaWorldRules.h"
#include "utils/Exception.h"

namespace warmonger {
namespace core {

QString rulesTypeToString(RulesType type)
{
    switch (type)
    {
        case RulesType::Lua:
            return "Lua";
    }

    throw utils::ValueError(
        "Cannot convert unknown rules-type: " + QString::number(static_cast<int>(type)) + " to string");
}

RulesType rulesTypeFromString(const QString& str)
{
    if (str == "Lua")
    {
        return RulesType::Lua;
    }
    else
    {
        throw utils::ValueError("Cannot parse unknown rules-type: " + str + " from string");
    }
}

WorldRules* createWorldRules(core::World* world)
{
    switch (world->getRulesType())
    {
        case RulesType::Lua:
            return new LuaWorldRules(world);
    }

    throw utils::ValueError("Cannot create world-rules for unknown rules type: " +
        QString::number(static_cast<int>(world->getRulesType())));
}

} // namespace core
} // namespace warmonger
