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

#include "utils/Settings.h"

#include <functional>
#include <map>

#include <QCoreApplication>
#include <QSettings>

#include "Version.h"
#include "utils/Constants.h"

namespace warmonger {
namespace utils {

const QString separator{"/"};

const std::map<SettingsKey, QVariant> defaultValues{};

const std::map<WorldSettingsKey, QVariant> worldDefaultValues{};

const std::map<SettingsKey, QString> settingsKeyToString{
    {SettingsKey::worldsDir, "worldsDir"}, {SettingsKey::workDir, "workDir"}};

const std::map<WorldSettingsKey, QString> worldSettingsKeyToString{{WorldSettingsKey::preferredSurface, "surface"}};

static QString toRawKey(const SettingsKey key);
static QString toRawKey(const QObject* world, const WorldSettingsKey key);

static QVariant getDefaultValue(const SettingsKey key);
static QVariant getWorldDefaultValue(const WorldSettingsKey key);

static QVariant getSettingsValue(const QString& key, const std::function<QVariant()>& getDefaultValueForKey);
static void setSettingsValue(const QString& key, const QVariant& value);

void initSettings()
{
    QCoreApplication::setOrganizationName(organizationName);
    QCoreApplication::setOrganizationDomain(organizationDomain);
    QCoreApplication::setApplicationName(applicationName);
    QCoreApplication::setApplicationVersion(version);
}

QVariant settingsValue(const SettingsKey key)
{
    return getSettingsValue(toRawKey(key), std::bind(getDefaultValue, key));
}

QVariant settingsValue(const QObject* world, const WorldSettingsKey key)
{
    return getSettingsValue(toRawKey(world, key), std::bind(getWorldDefaultValue, key));
}

void setSettingsValue(const SettingsKey key, const QVariant& value)
{
    setSettingsValue(toRawKey(key), value);
}

void setSettingsValue(const QObject* world, const WorldSettingsKey key, const QVariant& value)
{
    setSettingsValue(toRawKey(world, key), value);
}

static QString toRawKey(const SettingsKey key)
{
    return settingsKeyToString.at(key);
}

static QString toRawKey(const QObject* world, const WorldSettingsKey key)
{
    return world->objectName() + separator + worldSettingsKeyToString.at(key);
}

static QVariant getDefaultValue(const SettingsKey key)
{
    try
    {
        return defaultValues.at(key);
    }
    catch (std::out_of_range&)
    {
        return QVariant();
    }
}

static QVariant getWorldDefaultValue(const WorldSettingsKey key)
{
    try
    {
        return worldDefaultValues.at(key);
    }
    catch (std::out_of_range&)
    {
        return QVariant();
    }
}

static QVariant getSettingsValue(const QString& key, const std::function<QVariant()>& getDefaultValueForKey)
{
    QSettings settings;

    QVariant value = settings.value(key);

    if (value.isNull())
    {
        value = getDefaultValueForKey();
    }

    return value;
}

static void setSettingsValue(const QString& key, const QVariant& value)
{
    QSettings settings;

    settings.setValue(key, value);
}

} // namespace warmonger
} // namespace utils
