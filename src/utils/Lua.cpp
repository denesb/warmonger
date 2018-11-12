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

#include "utils/Lua.h"

#include "utils/Logging.h"
#include "utils/PathBuilder.h"

namespace sol {
namespace stack {

QString getter<QString>::get(lua_State* L, int index, record& tracking)
{
    tracking.use(1);
    std::size_t len;
    const auto str = lua_tolstring(L, index, &len);
    return QString::fromLocal8Bit(str, len);
}

int pusher<QString>::push(lua_State* L, const QString& str)
{
    const QByteArray data = str.toLocal8Bit();
    lua_pushlstring(L, data.data(), data.size());
    return 1;
}

QColor getter<QColor>::get(lua_State* L, int index, record& tracking)
{
    tracking.use(1);
    std::size_t len;
    const auto str = lua_tolstring(L, index, &len);
    return QColor(QString::fromLocal8Bit(str, len));
}

int pusher<QColor>::push(lua_State* L, const QColor& color)
{
    const QByteArray data = color.name().toLocal8Bit();
    lua_pushlstring(L, data.data(), data.size());
    return 1;
}

} // namespace stack
} // namespace sol

namespace warmonger {
namespace utils {

static void wLuaLog(sol::this_state L, utils::LogLevel logLevel, const std::string& msg);
static void loadModule(sol::state& lua, const QString& basePath, const QString& moduleName);

void initLuaAPI(sol::state& lua)
{
    lua.set_function(
        "w_trace", [](sol::this_state L, const std::string& msg) { wLuaLog(L, utils::LogLevel::Trace, msg); });
    lua.set_function(
        "w_debug", [](sol::this_state L, const std::string& msg) { wLuaLog(L, utils::LogLevel::Debug, msg); });
    lua.set_function(
        "w_info", [](sol::this_state L, const std::string& msg) { wLuaLog(L, utils::LogLevel::Info, msg); });
    lua.set_function(
        "w_warning", [](sol::this_state L, const std::string& msg) { wLuaLog(L, utils::LogLevel::Warning, msg); });
    lua.set_function(
        "w_error", [](sol::this_state L, const std::string& msg) { wLuaLog(L, utils::LogLevel::Error, msg); });

    lua.open_libraries(
        sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::debug);
}

void initLuaScript(sol::state& lua, const QString& basePath, const QString& mainRulesFile)
{
    sol::function tableInsert = lua["table"]["insert"];
    tableInsert(lua["package"]["searchers"], [&lua, basePath](sol::stack_object moduleName) {
        return std::function<void()>([&lua, basePath, moduleName = QString(moduleName.as<const char*>())] {
            loadModule(lua, basePath, moduleName);
        });
    });

    const QString path = basePath / mainRulesFile;
    wInfo << "Loading lua world rules from entry point " << path;

    lua.script_file(path.toStdString());
}

static void wLuaLog(sol::this_state L, utils::LogLevel logLevel, const std::string& msg)
{
    // TODO: trim source files
    lua_Debug info;
    int level = 1;
    const int pre_stack_size = lua_gettop(L);

    if (lua_getstack(L, level, &info) != 1)
    {
        wError << "Unable to traverse the Lua stack";
        lua_settop(L, pre_stack_size);

        utils::LogEntry(logLevel, "", "", -1) << msg;
    }
    else if (lua_getinfo(L, "nlS", &info) == 0)
    {
        wError << "Unable to get Lua debug info";
        lua_settop(L, pre_stack_size);

        utils::LogEntry(logLevel, "", "", -1) << msg;
    }
    else
    {
        utils::LogEntry(logLevel, info.short_src, info.name, info.currentline) << msg;
    }
}

static void loadModule(sol::state& lua, const QString& basePath, const QString& moduleName)
{
    const QString moduleFile = basePath / moduleName + ".lua";
    wInfo.format("Loading required module `{}' from `{}'", moduleName, moduleFile);
    lua.script_file(moduleFile.toStdString());
}

} // namespace utils
} // namespace warmonger
