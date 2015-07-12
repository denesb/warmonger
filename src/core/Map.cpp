#include "core/Map.h"
#include "core/World.h"
#include "core/MapTile.h"
#include "core/Player.h"
#include "core/Unit.h"
#include "core/Settlement.h"
#include "core/JsonUtil.hpp"
#include "core/Util.h"

using namespace warmonger::core;

const QString Map::DefinitionFile = "map.json";

Map::Map(QObject *parent) :
    GameObject(parent),
    displayName(),
    description(),
    world(nullptr),
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

QString Map::getDescription()const
{
    return this->description;
}

void Map::setDescription(const QString &description)
{
    this->description = description;
}

const World * Map::getWorld() const
{
    return this->world;
}

void Map::setWorld(const World *world)
{
    this->world = world;
}

int Map::getWidth() const
{
    return this->width;
}

void Map::setWidth(int width)
{
    this->width = width;
}

int Map::getHeight() const
{
    return this->height;
}

void Map::setHeight(int height)
{
    this->height = height;
}

const MapTile * Map::getMapTile(const MapPosition &position) const
{
    if (!this->mapTiles.contains(position))
        return nullptr;
    else
        return this->mapTiles[position];
}

MapTile * Map::getMapTile(const MapPosition &position)
{
    if (!this->mapTiles.contains(position))
        return nullptr;
    else
        return this->mapTiles[position];
}

void Map::setMapTile(const MapPosition &position, MapTile *mapTile)
{
    this->mapTiles[position] = mapTile;
}

QList<const Player *> Map::getPlayers() const
{
    return listConstClone(this->players);
}

QList<Player *> Map::getPlayers()
{
    return this->players;
}

void Map::setPlayers(const QList<Player *> &players)
{
    this->players = players;
}

QList<const Unit *> Map::getUnits() const
{
    return listConstClone(this->units);
}

QList<Unit *> Map::getUnits()
{
    return this->units;
}

void Map::setUnits(const QList<Unit *> &units)
{
    this->units = units;
}

QList<const Settlement *> Map::getSettlemets() const
{
    return listConstClone(this->settlements);
}

QList<Settlement *> Map::getSettlemets()
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
    this->description = obj["description"].toString();
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
    obj["description"] = this->description;
    obj["width"] = this->width;
    obj["height"] = this->height;
    obj["mapTiles"] = this->mapTilesToJson(this->mapTiles);
    obj["players"] = listToJson<Player>(this->players);
    obj["units"] = listToJson<Unit>(this->units);
    obj["settlements"] = listToJson<Settlement>(this->settlements);

    return std::move(obj);
}

QMap<MapPosition, MapTile *> Map::mapTilesFromJson(const QJsonObject &obj)
{
    QMap<MapPosition, MapTile *> mapTiles;

    QJsonObject::ConstIterator it;
    for (it = obj.constBegin(); it != obj.constEnd(); it++)
    {
        MapPosition pos(it.key());
        mapTiles[pos] = newFromJson<MapTile>(it.value().toObject(), this);
    }

    return std::move(mapTiles);
}

QJsonObject Map::mapTilesToJson(const QMap<MapPosition, MapTile *> &mapTiles) const
{
    QJsonObject obj;

    QMap<MapPosition, MapTile *>::ConstIterator it;
    for (it = mapTiles.constBegin(); it != mapTiles.constEnd(); it++)
    {
        obj[it.key().toStr()] = it.value()->toJson();
    }

    return std::move(obj);
}
