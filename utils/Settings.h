/** \file
 * Settings utility functions.
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

#ifndef W_UTILS_SETTINGS_H
#define W_UTILS_SETTINGS_H

#include <QVariant>

namespace warmonger {
namespace utils {

enum class SettingsKey
{
    worldsDir,
    workDir
};

enum class WorldSettingsKey
{
    preferredSurface
};

/**
 * Initializes the settings subsystem
 *
 * Must be called before accessing any settings!!!
 */
void initSettings();

/**
 * Get the settings value corresponding to static key `key'
 *
 * If the settings is not set but it has a default value than that is returned.
 * It is highly recommended to use the key constants defined in
 * utils/Settings.h.
 *
 * @param[in] const SettingsKey key the key
 *
 * @returns QVariant the settings value
 */
QVariant settingsValue(const SettingsKey key);

/**
 * Get the settings value corresponding to the world key `key'
 *
 * World keys depend on the world object they refer to. If the settings is not
 * set but it has a default value than that is returned. It is highly
 * recommended to use the key constants defined in utils/Settings.h.
 *
 * @param[in] const WorldSettingsKey key the key
 *
 * @returns QVariant the settings value
 */
QVariant settingsValue(const QObject* world, const WorldSettingsKey key);

/**
 * Set the settings value corresponding to static key `key'
 *
 * @param[in] const SettingsKey key the key
 * @param[in] const QVariant& value the value
 */
void setSettingsValue(const SettingsKey key, const QVariant& value);

/**
 * Set the settings value corresponding to the world key `key'
 *
 * World keys depend on the world object they refer to.
 *
 * @param[in] const WorldSettingsKey key the key
 * @param[in] const QVariant& value the value
 */
void setSettingsValue(const QObject* world, const WorldSettingsKey key, const QVariant& value);

} // namespace warmonger
} // namespace utils

#endif // W_UTILS_SETTINGS_H
