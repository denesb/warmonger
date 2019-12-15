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

#include <QDir>

#include "ui/SearchPaths.h"
#include "utils/Constants.h"
#include "utils/Logging.h"
#include "utils/Settings.h"

namespace warmonger {
namespace ui {

static void addSubdirToSearchPath(const QString& worldPath, const QString& subdirName, QStringList& searchPath);

void setupSearchPaths()
{
    const QVariant worldsDirVal = utils::settingsValue(utils::SettingsKey::worldsDir);

    if (worldsDirVal.isNull())
    {
        wError << "worldsDir not found in settings, search paths will not be set";
        return;
    }

    const QString worldsDirPath = worldsDirVal.toString();

    wDebug << "World dir is " << worldsDirPath;

    QStringList worldSearchPath, mapSearchPath, surfaceSearchPath;

    const QDir worldsDir(worldsDirPath);
    const QFileInfoList entries = worldsDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs);
    for (const auto& entry : entries)
    {
        const QString& worldPath = entry.canonicalFilePath();

        worldSearchPath.append(worldPath);

        addSubdirToSearchPath(worldPath, utils::paths::surfaces, surfaceSearchPath);
        addSubdirToSearchPath(worldPath, utils::paths::maps, mapSearchPath);

        wInfo << "Added world " << worldPath << " to world search path";
    }

    QDir::setSearchPaths(utils::searchPaths::world, worldSearchPath);
    QDir::setSearchPaths(utils::searchPaths::worldSurface, surfaceSearchPath);
    QDir::setSearchPaths(utils::searchPaths::map, mapSearchPath);
}

static void addSubdirToSearchPath(const QString& worldPath, const QString& subdirName, QStringList& searchPath)
{
    const QString& subdirPath = worldPath + "/" + subdirName;
    const QFileInfo& subdirDirInfo(subdirPath);

    if (subdirDirInfo.exists() && subdirDirInfo.isDir())
        searchPath.append(subdirDirInfo.canonicalFilePath());

    wInfo << "Added " << subdirPath << " to " << subdirName << " search path";
}

} // namespace ui
} // namespace warmonger
