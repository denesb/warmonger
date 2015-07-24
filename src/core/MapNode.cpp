#include "core/MapNode.h"
#include "core/World.h"
#include "core/TerrainType.h"
#include "core/JsonUtil.hpp"
#include "Util.h"

using namespace warmonger::core;

MapNode::MapNode(QObject *parent) :
    GameObject(parent),
    terrainType(nullptr),
    neighbours()
{
}

MapNode::~MapNode()
{
}

const TerrainType * MapNode::getTerrainType() const
{
    return this->terrainType;
}

void MapNode::setTerrainType(const TerrainType *terrainType)
{
    this->terrainType = terrainType;
}

const MapNode * MapNode::getNeighbour(Direction direction) const
{
    return this->neighbours[direction];
}

const QList<const MapNode *> MapNode::getNeighbours() const
{
    return this->neighbours;
}

void MapNode::setNeighbour(Direction direction, const MapNode *neighbour)
{
    this->neighbours[direction] = neighbour;
}

void MapNode::setNeighbours(const QList<const MapNode *> &neighbours)
{
    this->neighbours = neighbours;
}

void MapNode::dataFromJson(const QJsonObject &obj)
{
    World *world = this->parent()->findChild<World *>(QString(), Qt::FindDirectChildrenOnly);
    if (world == nullptr)
    {
        wError("core.MapNode") << "world is null";
        throw Exception(Exception::NullPointer);
    }

    const QString terrainTypeName = obj["terrainType"].toString();
    this->terrainType = world->findChild<TerrainType *>(terrainTypeName);
    if (this->terrainType == nullptr)
    {
        Exception e(Exception::UnresolvedReference, {"TerrainType", terrainTypeName});
        wError("core.MapNode") << e.getMessage();
        throw e;
    }

    this->neighbours = referenceListFromJson<MapNode>(obj["neighours"].toArray(), this);
}

void MapNode::dataToJson(QJsonObject &obj) const
{
    obj["terrainType"] = this->terrainType->objectName();
}
