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

static const QString category{"ui"};

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

    for (QString path : QDir::searchPaths("Map"))
    {
        QDir mapsDir(path);

        for (QString mapFile : mapsDir.entryList(nameFilters, filters))
        {
            core::Map *map = new core::Map();
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

QObject * ApplicationContext::newGame(QObject *map)
{
    core::Map *m = qobject_cast<core::Map *>(map);
    if (m == nullptr)
    {
        wError(category) << "map is null or has wrong type";
        throw core::Exception(core::Exception::InvalidValue);
    }

    core::Game *game = new core::Game();
    game->fromMapJson(m->toJson());
    game->getWorld()->setSurface("default");



    return game;
}

void ApplicationContext::setCurrentMap(QObject *map)
{
}
