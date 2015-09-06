#include <QColor>
#include <QDir>
#include <QStringList>

#include "log/LogStream.h"
#include "core/Util.h"
#include "core/WorldSurface.h"
#include "core/Map.h"
#include "core/World.h"
#include "ApplicationContext.h"

using namespace warmonger;
using namespace warmonger::ui;

ApplicationContext::ApplicationContext(QObject *parent) :
    QObject(parent),
    maps()
{
    this->loadMaps();
}

ApplicationContext::~ApplicationContext()
{
}

void ApplicationContext::loadMaps()
{
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
            this->maps << map;
        }
    }

    emit mapsChanged();
}

QVariantList ApplicationContext::readMaps() const
{
    return core::toQVariantList<core::Map>(this->maps);
}
