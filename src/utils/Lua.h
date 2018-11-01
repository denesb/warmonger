/** \file
 * Lua utils.
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

#ifndef W_UTILS_LUA_H
#define W_UTILS_LUA_H

#include <QColor>
#include <QString>
#include <sol/sol.hpp>

namespace sol {

template <>
struct lua_type_of<QString> : std::integral_constant<sol::type, sol::type::string>
{
};

template <>
struct lua_type_of<QColor> : std::integral_constant<sol::type, sol::type::string>
{
};

namespace stack {

template <>
struct getter<QString>
{
    static QString get(lua_State* L, int index, record& tracking);
};

template <>
struct pusher<QString>
{
    static int push(lua_State* L, const QString& str);
};

template <>
struct getter<QColor>
{
    static QColor get(lua_State* L, int index, record& tracking);
};

template <>
struct pusher<QColor>
{
    static int push(lua_State* L, const QColor& color);
};

} // namespace stack
} // namespace sol


namespace warmonger {
namespace utils {

void initLuaScript(sol::state& lua, const QString& basePath, const QString& mainRulesFile);

} // namespace utils
} // namespace warmonger

#endif // W_CORE_LUA_WORLD_RULES_H
