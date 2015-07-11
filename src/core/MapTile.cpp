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

const TerrainType * MapTile::getTerrainType() const
{
    return this->terrainType;
}

void MapTile::setTerrainType(const TerrainType *terrainType)
{
    this->terrainType = terrainType;
}

MapPosition MapTile::getPosition() const
{
    return this->position;
}

void MapTile::setPosition(const MapPosition &position)
{
    this->position = position;
}

const MapTile * MapTile::getNeighbour(Direction direction) const
{
    return this->neighbours[direction];
}

void MapTile::setNeighbour(Direction direction, const MapTile *mapTile)
{
    this->neighbours[direction] = mapTile;
}

void MapTile::fromJson(const QJsonObject &obj)
{
    //TODO: error handling
    World *world = this->parent()->findChild<World *>(QString(), Qt::FindDirectChildrenOnly);

    this->terrainType = world->findChild<TerrainType *>(obj["terrainType"].toString());
    this->position = MapPosition(obj["position"].toString());
}

QJsonObject MapTile::toJson() const
{
    QJsonObject obj;

    obj["terrainType"] = this->terrainType->objectName();
    obj["position"] = this->position.toStr();

    return std::move(obj);
}
