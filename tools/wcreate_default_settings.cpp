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

#include <QDir>
#include <QSettings>

#include "utils/Logging.h"
#include "utils/Settings.h"
#include "utils/Utils.h"

using namespace warmonger;

/**
 * Create the default settings for the application.
 */
int main()
{
    utils::initSettings();
    utils::initLogging();

    QDir homeDir{QDir::home()};

    const QString workDirPath{utils::makePath(QDir::homePath(), QStringLiteral(".warmonger"))};
    const QString worldsDirPath{utils::makePath(workDirPath, QStringLiteral("worlds"))};

    homeDir.mkpath(worldsDirPath);

    wInfo << "Created work directory " << workDirPath;
    wInfo << "Created worlds directory " << worldsDirPath;

    utils::setSettingsValue(utils::SettingsKey::worldsDir, worldsDirPath);
    utils::setSettingsValue(utils::SettingsKey::workDir, workDirPath);

    QSettings settings;

    wInfo << "Settings saved to " << settings.fileName();

    return 0;
}
