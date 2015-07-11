#include <QDir>

#include "core/MapLoader.h"
#include "core/Map.h"
#include "core/JsonUtil.hpp"

using namespace warmonger::core;

MapLoader::MapLoader(QObject *parent) :
    QObject(parent)
{
}

MapLoader::~MapLoader()
{
}

QStringList MapLoader::getSearchPath() const
{
    return this->searchPath;
}

void MapLoader::setSearchPath(const QStringList &searchPath)
{
    if (this->searchPath != searchPath)
    {
        this->searchPath = searchPath;

        emit this->searchPathChanged(this->searchPath);

        this->scanSearchPath();
        this->loadMapList();
    }
}

QList<Map*> MapLoader::getMapList() const
{
    return this->mapList;
}

Map * MapLoader::loadMap(const QString &mapName)
{
    return newFromJsonFile<Map>(this->pathToMapMap[mapName] + "/" + Map::MapDefinitionFile, nullptr);
}

void MapLoader::scanSearchPath()
{
    this->pathToMapMap.clear();

    for (const QString scanPath : this->searchPath)
    {
        QDir scanDir(scanPath);
        scanDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
        scanDir.setSorting(QDir::Name);

        const QString scanCanonicalPath = scanDir.canonicalPath();

        for(const QString dir : scanDir.entryList())
        {
            const QString path = scanCanonicalPath + "/" + dir;
            if (QFile::exists(path + "/" + Map::MapDefinitionFile))
            {
                this->pathToMapMap[dir] = path;
            }
        }
    }
}

void MapLoader::loadMapList()
{
    for (Map *map : this->mapList)
    {
        map->deleteLater();
    }
    this->mapList.clear();

    QMap<QString, QString>::ConstIterator it;
    for (it = this->pathToMapMap.constBegin(); it != this->pathToMapMap.constEnd(); it++)
    {
        this->mapList.append(newFromJsonFile<Map>(it.value() + "/" + Map::MapDefinitionFile, this));
    }
}
