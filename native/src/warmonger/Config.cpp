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

namespace warmonger {

Config::Config()
    : configFile(ConfigFile::_new())
{
    if (const auto res = config->load("user://config.cfg"); !res)
    {
        // FIXME: error handling
        return
    }
}

std::optional<godot::String> Config::getDefaultWorldPath() const
{
}

} // namespace warmonger
