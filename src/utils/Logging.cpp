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

#include <map>

#include "Version.h"
#include "utils/Logging.h"

namespace warmonger {
namespace utils {

static std::shared_ptr<spdlog::logger> wLogger;

static std::string trimSrcFilePath(const char* fileName);
static void qtMessageHandler(QtMsgType type, const QMessageLogContext& ctx, const QString& msg);
static void log(LogLevel level, const std::string& file, const char* function, int line, const std::string& msg);

void initLogging(std::shared_ptr<spdlog::logger> logger)
{
    if (!logger)
        wLogger = spdlog::stdout_color_mt(loggerName);
    else
        wLogger = logger;

    spdlog::set_level(spdlog::level::debug);
    qInstallMessageHandler(qtMessageHandler);
}

LogEntry::LogEntry(LogLevel level, const char* file, const char* function, int line)
    : level(level)
    , file(trimSrcFilePath(file))
    , function(function)
    , line(line)
{
}

LogEntry::~LogEntry()
{
    log(this->level, this->file, this->function, this->line, this->msg.str());
}

std::string trimSrcFilePath(const char* fileName)
{
    if (fileName == nullptr)
        return std::string();

    const std::string path(fileName);
    if (path.compare(0, basePath.size(), basePath) == 0)
    {
        return path.substr(basePath.size() + 1);
    }
    else
    {
        return path;
    }
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

    log(lvl, trimSrcFilePath(ctx.file), nullptr, ctx.line, msg.toStdString());
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
            s << " " << function;

        s << "] ";
    }
    s << msg;

    wLogger->log(lvl, s.str());
}

} // namespace utils
} // namespace warmonger
