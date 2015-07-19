#include "core/MapTile.h"
#include "core/World.h"
#include "core/TerrainType.h"
#include "core/JsonUtil.hpp"
#include "Util.h"

using namespace warmonger::core;

MapTile::MapTile(QObject *parent) :
    GameObject(parent),
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

void MapTile::dataFromJson(const QJsonObject &obj)
{
    World *world = this->parent()->findChild<World *>(QString(), Qt::FindDirectChildrenOnly);
    if (world == nullptr)
    {
        wError("core.MapTile") << "world is null";
        throw Exception(Exception::NullPointer);
    }

    const QString terrainTypeName = obj["terrainType"].toString();
    this->terrainType = world->findChild<TerrainType *>(terrainTypeName);
    if (this->terrainType == nullptr)
    {
        wError("core.MapTile") << "Unable to resolve reference <TerrainType>" << terrainTypeName;
        throw Exception(Exception::UnresolvedReference, {"TerrainType", terrainTypeName});
    }
    
    this->position = MapPosition(obj["position"].toString());
}

void MapTile::dataToJson(QJsonObject &obj) const
{
    obj["terrainType"] = this->terrainType->objectName();
    obj["position"] = this->position.toStr();
}
