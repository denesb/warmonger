/**
 * Copyright (C) 2015-2019 Botond Dénes
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

#include "warmonger/Config.h"

#include <ConfigFile.hpp>
#include <fmt/format.h>

#include "utils/Exception.h"

namespace warmonger {

namespace {

const auto configFilePath = "user://config.cfg";

} // anonymous namespace

Config::Config()
    : configFile(godot::ConfigFile::_new())
{
    if (const auto res = configFile->load(configFilePath); res != godot::Error::OK)
    {
        throw utils::GodotError(res, fmt::format("Failed to load configuration from {}", configFilePath));
    }
}

std::optional<godot::String> Config::getDefaultWorldPath() const
{
    auto val = configFile->get_value("", "default_world_path");
    if (val.get_type() == godot::Variant::Type::NIL)
    {
        return {};
    }
    return val;
}

void Config::setDefaultWorldPath(godot::String path)
{
    configFile->set_value("", "default_world_path", path);
    if (const auto res = configFile->load(configFilePath); res != godot::Error::OK)
    {
        throw utils::GodotError(res, fmt::format("Failed to save configuration to {}", configFilePath));
    }
}

} // namespace warmonger
