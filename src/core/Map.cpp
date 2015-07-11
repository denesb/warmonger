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
    this->width = obj["width"].toInt();
    this->height = obj["height"].toInt();
    this->mapTiles = this->mapTilesFromJson(obj["mapTiles"].toObject());
    this->players = newListFromJson<Player>(obj["players"].toArray(), this);
    this->units = newListFromJson<Unit>(obj["units"].toArray(), this);
    this->settlements = newListFromJson<Settlement>(obj["settlements"].toArray(), this);
}

QJsonObject Map::toJson() const
{
    QJsonObject obj;

    obj["objectName"] = this->objectName();
    obj["displayName"] = this->displayName;
    obj["width"] = this->width;
    obj["height"] = this->height;
    obj["mapTiles"] = this->mapTilesToJson(this->mapTiles);
    obj["players"] = listToJson<Player *>(this->players);
    obj["units"] = listToJson<Unit *>(this->units);
    obj["settlements"] = listToJson<Settlement *>(this->settlements);

    return std::move(obj);
}

QMap<QPoint, MapTile *> Map::mapTilesFromJson(const QJsonObject &obj)
{
    QMap<QPoint, MapTile *> mapTiles;
    QJsonObject::constIterator it;
    for (it = obj.constBegin(); it != obj.constEnd(); it++)
    {
        QPoint pos = str2pos(it.key());
        mapTiles[pos] = newFromJson<MapTile>(it.value().toObject());
    }

    return std::move(mapTiles);
}

QJsonObject Map::mapTilesFromJson(const QMap<QPoint, MapTile *> &mapTiles)
{
    QJsonObject obj;

    QMap<QPoint, MapTile *>::constIterator it;
    for (it = mapTiles.constBegin(); it != mapTiles.constEnd(); it++)
    {
        QString pos = pos2str(it.key());
        obj[pos] it.value()->toJson();
    }

    return std::move(obj);
}
