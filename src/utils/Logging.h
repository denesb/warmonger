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

#include "utils/Format.h"
#include "utils/ToString.h"

namespace warmonger {
namespace utils {

enum class LogLevel : char
{
    Trace,
    Debug,
    Info,
    Warning,
    Error
};

enum class LogSinkType
{
    Console,
    File,
    Stream
};

/**
 * Logging configuration.
 */
class LogConfig
{
public:
    /**
     * Create a logging configuration with a log-file.
     */
    static LogConfig File(std::string file);

    /**
     * Create a logging configuration for the console.
     */
    static LogConfig Console();

    /**
     * Create a logging configuration with a stream.
     */
    static LogConfig Stream(std::ostream& s);

    LogSinkType getSinkType() const
    {
        return this->sinkType;
    }

    std::string getFile() const
    {
        return this->file;
    }

    std::ostream& getStream() const
    {
        return *this->stream;
    }

private:
    LogConfig(LogSinkType sinkType, std::string file, std::ostream* stream)
        : sinkType(sinkType)
        , file(std::move(file))
        , stream(stream)
    {
    }

    LogSinkType sinkType;
    std::string file;
    std::ostream* stream{};
};

void initLogging(const LogConfig& cfg = LogConfig::Console());

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

    template <typename... Args>
    LogEntry& format(const char* format, Args&&... args)
    {
        this->msg << fmt::format(format, std::forward<Args>(args)...);
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

} // namespace utils
} // namespace warmonger

#endif // W_UTILS_LOGGING_H
