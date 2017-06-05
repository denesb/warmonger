/** \file
 * Logging functions and macros.
 *
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

#ifndef W_UTILS_LOGGING_H
#define W_UTILS_LOGGING_H

#include <sstream>

#include <spdlog/spdlog.h>

#include "utils/ToString.h"

namespace warmonger {
namespace utils {

const char loggerName[] = "console";

void initLogging(std::shared_ptr<spdlog::logger> logger = nullptr);

enum class LogLevel : char
{
    Trace,
    Debug,
    Info,
    Warning,
    Error
};

struct LogEntry
{
    LogEntry(LogLevel level, const char* file, const char* function, int line);
    ~LogEntry();

    template <typename T>
    inline LogEntry& operator<<(T&& v)
    {
        this->msg << v;
        return *this;
    }

    LogLevel level;
    std::string file;
    const char* function;
    int line;
    std::stringstream msg;
};

#define wLog(lvl) ::warmonger::utils::LogEntry(lvl, __FILE__, __FUNCTION__, __LINE__)
#define wTrace wLog(::warmonger::utils::LogLevel::Trace)
#define wDebug wLog(::warmonger::utils::LogLevel::Debug)
#define wInfo wLog(::warmonger::utils::LogLevel::Info)
#define wWarning wLog(::warmonger::utils::LogLevel::Warning)
#define wError wLog(::warmonger::utils::LogLevel::Error)

/* TODO: make it work with the spdlog streaming style
#define wTrace ::spdlog::get(::warmonger::utils::loggerName)->trace() << __PRETTY__FUNCTION__
#define wDebug ::spdlog::get(::warmonger::utils::loggerName)->debug()
#define wInfo ::spdlog::get(::warmonger::utils::loggerName)->info()
#define wWarning ::spdlog::get(::warmonger::utils::loggerName)->warning()
#define wError ::spdlog::get(::warmonger::utils::loggerName)->error()
*/

} // namespace utils
} // namespace warmonger

#endif // W_UTILS_LOGGING_H
