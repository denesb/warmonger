#include "core/Map.h"
#include "core/MapTile.h"
#include "core/Player.h"
#include "core/Unit.h"
#include "core/Settlement.h"

using namespace warmonger::core;

Map::Map(QObject &parent) :
    QObject(parent),
    displayName(),
    width(0),
    height(0),
    mapTiles(),
    players(),
    units(),
    settlements()
{
}

Map::~Map()
{
}

QString Map::getDisplayName() const
{
    return this->displayName;
}

void Map::setDisplayName(const QString &displayName)
{
    this->displayName = displayName;
}

MapTile * Map::getMapTile(const QPoint &position) const
{
    if (!this->mapTiles.contains(position)
    {
        //TODO: throw something
    }
    return mapTiles[position];
}

void Map::setMapTile(const QPoint &position, MapTile *mapTile)
{
    this->mapTiles[position] = mapTile;
}

QList<Unit *> Map::getUnits() const
{
    return this->units;
}

void Map::setUnits(const QList<Unit *> &units)
{
    this->units = units;
}

QList<Settlement *> Map::getSettlemets() const
{
    return this->settlements;
}

void Map::setSettlements(const QList<Settlement *> &settlements)
{
    this->settlements = settlements;
}

void Map::fromJson(const QJsonObject &obj)
{
    this->setObjectName(obj["objectName"].toString());
    this->displayName = obj["displayName"].toString();
    this->width = obj["displayName"].toInt();
    this->height = obj["height"].toInt();
}

QJsonObject Map::toJson() const
{
}
