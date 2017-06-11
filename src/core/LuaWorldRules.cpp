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

#include "core/LuaWorldRules.h"

#include <sol/sol.hpp>

#include "core/Map.h"
#include "utils/Logging.h"
#include "utils/Utils.h"

namespace warmonger {
namespace core {

static void exposeAPI(sol::state& lua, core::World* world);

static void wLuaLog(sol::this_state ts, utils::LogLevel logLevel, const std::string& msg);
static void inline wLuaTrace(sol::this_state ts, const std::string& msg);
static void inline wLuaDebug(sol::this_state ts, const std::string& msg);
static void inline wLuaInfo(sol::this_state ts, const std::string& msg);
static void inline wLuaWarning(sol::this_state ts, const std::string& msg);
static void inline wLuaError(sol::this_state ts, const std::string& msg);

LuaWorldRules::LuaWorldRules(const QString& basePath, core::World* world)
    : world(world)
    , state(std::make_unique<sol::state>())
{
    sol::state& lua = *this->state.get();

    exposeAPI(lua, this->world);

    const auto entryPoint = utils::makePath(basePath, this->world->getRulesEntryPoint()).toStdString();

    wInfo << "Loading lua world rules from entry point " << entryPoint;

    lua.script_file(entryPoint);

    this->initHook = lua["init"];
    this->generateMapHook = lua["generate_map"];

    this->initHook();
}

std::unique_ptr<core::Map> LuaWorldRules::generateMap(unsigned int size)
{
    auto map{std::make_unique<core::Map>()};

    map->setWorld(world);
    map->generateMapNodes(size);

    return map;
}

static void exposeAPI(sol::state& lua, core::World*)
{
    lua.set_function("wTrace", wLuaTrace);
    lua.set_function("wDebug", wLuaDebug);
    lua.set_function("wInfo", wLuaInfo);
    lua.set_function("wWarning", wLuaWarning);
    lua.set_function("wError", wLuaError);
}

static void wLuaLog(sol::this_state ts, utils::LogLevel logLevel, const std::string& msg)
{
    lua_State* L = ts;

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

static void inline wLuaTrace(sol::this_state ts, const std::string& msg)
{
    wLuaLog(ts, utils::LogLevel::Trace, msg);
}

static void inline wLuaDebug(sol::this_state ts, const std::string& msg)
{
    wLuaLog(ts, utils::LogLevel::Debug, msg);
}

static void inline wLuaInfo(sol::this_state ts, const std::string& msg)
{
    wLuaLog(ts, utils::LogLevel::Info, msg);
}

static void inline wLuaWarning(sol::this_state ts, const std::string& msg)
{
    wLuaLog(ts, utils::LogLevel::Warning, msg);
}

static void inline wLuaError(sol::this_state ts, const std::string& msg)
{
    wLuaLog(ts, utils::LogLevel::Error, msg);
}

} // namespace core
} // namespace warmonger
