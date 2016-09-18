#include "core/MapNode.h"
#include "utils/Exception.h"

namespace warmonger {
namespace core {

MapNode::MapNode(QObject* parent)
    : QObject(parent)
    , terrainType(nullptr)
{
}

QString MapNode::getDisplayName() const
{
    return this->displayName;
}

void MapNode::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

TerrainType* MapNode::getTerrainType() const
{
    return this->terrainType;
}

void MapNode::setTerrainType(TerrainType* terrainType)
{
    if (this->terrainType != terrainType)
    {
        this->terrainType = terrainType;
        emit terrainTypeChanged();
    }
}

const MapNodeNeighbours& MapNode::getNeighbours() const
{
    return this->neighbours;
}

void MapNode::setNeighbours(const MapNodeNeighbours& neighbours)
{
    if (this->neighbours != neighbours)
    {
        this->neighbours = neighbours;
        emit neighboursChanged();
    }
}

void MapNode::setNeighbours(MapNodeNeighbours&& neighbours)
{
    if (this->neighbours != neighbours)
    {
        this->neighbours = std::move(neighbours);
        emit neighboursChanged();
    }
}

MapNode* MapNode::getNeighbour(Direction direction) const
{
    return this->neighbours.at(direction);
}

void MapNode::setNeighbour(Direction direction, MapNode* mapNode)
{
    if (this->neighbours[direction] != mapNode)
    {
        this->neighbours[direction] = mapNode;
        emit neighboursChanged();
    }
}

} // namespace core
} // namespace warmonger
