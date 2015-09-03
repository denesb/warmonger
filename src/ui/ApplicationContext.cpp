#include <QColor>
#include <QDir>
#include <QStringList>

#include "log/LogStream.h"
#include "core/Exception.h"
#include "core/WorldSurface.h"
#include "core/Map.h"
#include "core/World.h"
#include "ApplicationContext.h"

using namespace warmonger::ui;

ApplicationContext::ApplicationContext(QObject *parent) :
    QObject(parent)
{
}

ApplicationContext::~ApplicationContext()
{
}

QVariantList ApplicationContext::mapList()
{
    QVariantList vl;

    QStringList nameFilters;
    nameFilters << "*.wmd";

    QFlags<QDir::Filter> filters = QDir::Files | QDir::Readable;

    for (QString path : QDir::searchPaths("maps"))
    {
        QDir mapsDir(path);

        for (QString mapFile : mapsDir.entryList(nameFilters, filters))
        {
            core::Map *map = new core::Map(this);
            map->loadAs(mapsDir.absoluteFilePath(mapFile));
            map->getWorld()->setSurface("default");
            vl << QVariant::fromValue<QObject *>(map);
        }
    }

    return vl;
}

QObject * ApplicationContext::openMap(QString mapName)
{
    core::Map *map = new core::Map(this);
    map->load(map->specification(mapName));
    map->getWorld()->setSurface("default");

    return map;
}

void ApplicationContext::closeMap(QObject *map)
{
    delete map;
}
