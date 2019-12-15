/**
 * Copyright (C) 2015-2018 Botond Dénes
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

#include "tools/Utils.h"

namespace warmonger {
namespace tools {

std::shared_ptr<std::stringstream> setupLogging()
{
    auto stream = std::make_shared<std::stringstream>();

    utils::initLogging(utils::LogConfig::Stream(*stream.get()));

    return stream;
}

} // namespace tools
} // namespace warmonger
