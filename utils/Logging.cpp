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

#include <map>

#include "Version.h"
#include "utils/Logging.h"

namespace warmonger {
namespace utils {

const std::string noFile("-");

std::string trimSrcFilePath(const char* fileName)
{
    if (fileName == nullptr)
        return noFile;

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

static void qtMessageHandler(QtMsgType type, const QMessageLogContext& ctx, const QString& msg);

void initLogging()
{
    qInstallMessageHandler(qtMessageHandler);
}

static void qtMessageHandler(QtMsgType type, const QMessageLogContext& ctx, const QString& msg)
{
    switch (type)
    {
        case QtDebugMsg:
            BOOST_LOG_TRIVIAL(debug) << trimSrcFilePath(ctx.file) << ":" << ctx.line << " " << msg.toStdString();
            break;

        case QtInfoMsg:
            BOOST_LOG_TRIVIAL(info) << trimSrcFilePath(ctx.file) << ":" << ctx.line << " " << msg.toStdString();
            break;

        case QtWarningMsg:
            BOOST_LOG_TRIVIAL(warning) << trimSrcFilePath(ctx.file) << ":" << ctx.line << " " << msg.toStdString();
            break;

        case QtCriticalMsg:
            BOOST_LOG_TRIVIAL(error) << trimSrcFilePath(ctx.file) << ":" << ctx.line << " " << msg.toStdString();
            break;

        case QtFatalMsg:
            BOOST_LOG_TRIVIAL(fatal) << trimSrcFilePath(ctx.file) << ":" << ctx.line << " " << msg.toStdString();
            break;
    }
}

} // namespace utils
} // namespace warmonger
