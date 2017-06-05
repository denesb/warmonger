/**
 * Copyright (C) 2015-2017 Botond Dénes
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

#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/spdlog.h>

#include "tools/Utils.h"
#include "utils/Logging.h"

namespace warmonger {
namespace tools {

std::shared_ptr<std::stringstream> setupLogging()
{
    auto stream = std::make_shared<std::stringstream>();
    auto sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(*stream.get());

    utils::initLogging(spdlog::create(utils::loggerName, {sink}));

    return stream;
}

} // namespace tools
} // namespace warmonger
