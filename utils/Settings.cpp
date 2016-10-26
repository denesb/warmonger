#include <functional>
#include <map>

#include <QCoreApplication>
#include <QSettings>

#include "utils/Constants.h"
#include "utils/Settings.h"

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
