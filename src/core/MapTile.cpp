#include "core/MapTile.h"
#include "core/World.h"
#include "core/TerrainType.h"
#include "core/JsonUtil.hpp"
#include "Util.h"

using namespace warmonger::core;

MapTile::MapTile(QObject *parent) :
    QObject(parent),
    terrainType(nullptr),
    position(),
    neighbours({
            std::make_pair(MapTile::North, nullptr),
            std::make_pair(MapTile::NorthEast, nullptr),
            std::make_pair(MapTile::SouthEast, nullptr),
            std::make_pair(MapTile::South, nullptr),
            std::make_pair(MapTile::SouthWest, nullptr),
            std::make_pair(MapTile::NorthWest, nullptr)
            })
{
}

MapTile::~MapTile()
{
}

TerrainType * MapTile::getTerrainType() const
{
    return this->terrainType;
}

void MapTile::setTerrainType(TerrainType *terrainType)
{
    this->terrainType = terrainType;
}

QPoint MapTile::getPosition() const
{
    return this->position;
}

void MapTile::setPosition(const QPoint &position)
{
    this->position = position;
}

MapTile * MapTile::getNeighbour(Direction direction) const
{
    return this->neighbours[direction];
}

void MapTile::setNeighbour(Direction direction, MapTile *mapTile)
{
    this->neighbours[direction] = mapTile;
}

void MapTile::fromJson(const QJsonObject &obj)
{
    //TODO: error handling
    World *world = this->parent()->findChild<World *>(QString(), Qt::FindDirectChildrenOnly);

    this->terrainType = world->findChild<TerrainType *>(obj["terrainType"].toString());
    this->position = str2pos(obj["position"].toString());
}

QJsonObject MapTile::toJson() const
{
    QJsonObject obj;

    obj["terrainType"] = this->terrainType->objectName();
    obj["position"] = pos2str(this->position);

    return std::move(obj);
}
