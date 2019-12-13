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

// These have to be included before Logging.h otherwise the latter overrides
// spdlog's bundled format lib... its' a mess.
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/spdlog.h>

#include "utils/Logging.h"

#include <map>

#include "Version.h"

namespace warmonger {
namespace utils {

const char loggerName[] = "console";

static std::shared_ptr<spdlog::logger> wLogger;

static void qtMessageHandler(QtMsgType type, const QMessageLogContext& ctx, const QString& msg);
static void log(LogLevel level, const std::string& file, const char* function, int line, const std::string& msg);

LogConfig LogConfig::File(std::string file)
{
    return LogConfig(LogSinkType::File, std::move(file), {});
}

LogConfig LogConfig::Console()
{
    return LogConfig(LogSinkType::Console, {}, {});
}

LogConfig LogConfig::Stream(std::ostream& s)
{
    return LogConfig(LogSinkType::Stream, {}, &s);
}

void initLogging(const LogConfig& cfg)
{
    switch (cfg.getSinkType())
    {
        case LogSinkType::Console:
        {
            wLogger = spdlog::stdout_color_mt(loggerName);
        }
        break;
        case LogSinkType::File:
        {
            wLogger = spdlog::basic_logger_mt(loggerName, cfg.getFile());
        }
        break;
        case LogSinkType::Stream:
        {
            auto sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(cfg.getStream());
            wLogger = spdlog::create(loggerName, {sink});
        }
        break;
    }

    spdlog::set_level(spdlog::level::debug);
    qInstallMessageHandler(qtMessageHandler);
}

LogEntry::LogEntry(LogLevel level, const char* file, const char* function, int line)
    : level(level)
    , file(file)
    , function(function)
    , line(line)
{
}

LogEntry::~LogEntry()
{
    log(this->level, this->file, this->function, this->line, this->msg.str());
}

static void qtMessageHandler(QtMsgType type, const QMessageLogContext& ctx, const QString& msg)
{
    LogLevel lvl;
    switch (type)
    {
        case QtDebugMsg:
            lvl = LogLevel::Debug;
            break;

        case QtInfoMsg:
            lvl = LogLevel::Info;
            break;

        case QtWarningMsg:
            lvl = LogLevel::Warning;
            break;

        case QtCriticalMsg:
        case QtFatalMsg:
            lvl = LogLevel::Error;
            break;
    }

    const auto file = ctx.file ? std::string{ctx.file} : std::string{};
    log(lvl, file, nullptr, ctx.line, msg.toStdString());
}

static void log(LogLevel level, const std::string& file, const char* function, int line, const std::string& msg)
{
    spdlog::level::level_enum lvl;
    switch (level)
    {
        case LogLevel::Trace:
            lvl = spdlog::level::trace;
            break;

        case LogLevel::Debug:
            lvl = spdlog::level::debug;
            break;

        case LogLevel::Info:
            lvl = spdlog::level::info;
            break;

        case LogLevel::Warning:
            lvl = spdlog::level::warn;
            break;

        case LogLevel::Error:
            lvl = spdlog::level::err;
            break;
    }

    std::stringstream s;

    if (!file.empty() || function)
    {
        s << "[";
        if (!file.empty())
            s << file;

        if (line >= 0)
            s << ":" << line;

        if (!file.empty() && function && line >= 0)
            s << " ";

        if (function)
            s << function << "()";

        s << "] ";
    }
    s << msg;

    wLogger->log(lvl, s.str());
}

} // namespace utils
} // namespace warmonger
