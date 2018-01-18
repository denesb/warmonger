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
#include "utils/Exception.h"

namespace warmonger {
namespace core {

QString rulesTypeToString(WorldRules::Type type)
{
    const QMetaEnum metaEnum{QMetaEnum::fromType<WorldRules::Type>()};

    const auto str = metaEnum.valueToKey(static_cast<int>(type));
    if (!str)
        throw utils::ValueError(
            "Cannot convert unknown rules-type: " + QString::number(static_cast<int>(type)) + " to string");
    return QString(str);
}

WorldRules::Type rulesTypeFromString(const QString& str)
{
    const QMetaEnum metaEnum{QMetaEnum::fromType<WorldRules::Type>()};

    bool ok{true};
    const auto value = metaEnum.keyToValue(str.toLocal8Bit().data(), &ok);

    if (!ok)
        throw utils::ValueError("Cannot parse unknown rules-type: " + str + " from string");

    return static_cast<WorldRules::Type>(value);
}

WorldRules* createWorldRules(core::World* world)
{
    switch (world->getRulesType())
    {
        case WorldRules::Type::Lua:
            return new LuaWorldRules(world);
    }

    throw utils::ValueError("Cannot create world-rules for unknown rules type: " +
        QString::number(static_cast<int>(world->getRulesType())));
}

} // namespace core
} // namespace warmonger
