/** \file
 * Logging functions and macros.
 *
 * \copyright (C) 2015-2016 Botond Dénes
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

#ifndef W_UTILS_LOGGING_H
#define W_UTILS_LOGGING_H

#include <cstdlib>

#include <boost/log/trivial.hpp>

#include "utils/ToString.h"

namespace warmonger {
namespace utils {

std::string trimSrcFilePath(const char* fileName);

void initLogging();

#define wLog(lvl) BOOST_LOG_TRIVIAL(lvl) << warmonger::utils::trimSrcFilePath(__FILE__) << ":" << __LINE__ << " "
#define wTrace wLog(trace) << __PRETTY_FUNCTION__ << " "
#define wDebug wLog(debug) << __FUNCTION__ << "() "
#define wInfo wLog(info)
#define wWarning wLog(warning)
#define wError wLog(error)
#define wFatal wLog(fatal)

} // namespace utils
} // namespace warmonger

#endif // W_UTILS_LOGGING_H
