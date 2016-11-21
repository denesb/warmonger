/**
 * Copyright (C) 2015-2016 Botond Dénes
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

#include <boost/log/sinks.hpp>

#include "tools/Utils.h"
#include "utils/Logging.h"

namespace warmonger {
namespace tools {

boost::shared_ptr<std::stringstream> setupLogging()
{
    utils::initLogging();

    // Construct the sink
    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> TextSink;
    boost::shared_ptr<TextSink> sink = boost::make_shared<TextSink>();

    boost::shared_ptr<std::stringstream> logStream = boost::make_shared<std::stringstream>();

    // Add a stream to write log to
    sink->locked_backend()->add_stream(logStream);

    // Register the sink in the logging core
    boost::log::core::get()->add_sink(sink);

    return logStream;
}

} // namespace tools
} // namespace warmonger
