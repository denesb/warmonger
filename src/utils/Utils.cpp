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

#include "utils/Utils.h"
#include "utils/Constants.h"
#include "utils/Settings.h"

namespace warmonger {
namespace utils {

QString makeFileName(const QString& fileName, const QString& extension)
{
    return fileName + "." + extension;
}

QString worldSurfacePath(const QString& worldName, const QString& worldSurfaceName)
{
    const QString worldPath = settingsValue(SettingsKey::worldsDir).toString();

    return makeFileName(
        makePath(worldPath, worldName, paths::surfaces, worldSurfaceName), fileExtensions::surfacePackage);
}

QString worldMapsPath(const QString& worldName)
{
    const QString worldPath = settingsValue(SettingsKey::worldsDir).toString();

    return makePath(worldPath, worldName, paths::maps);
}

} // namespace utils
} // namespace warmonger
