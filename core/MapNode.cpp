#include "core/MapNode.h"
#include "utils/Exception.h"

namespace warmonger {
namespace core {

MapNode::MapNode(QObject* parent)
    : QObject(parent)
    , terrainType(nullptr)
{
}

void MapNode::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

void MapNode::setTerrainType(TerrainType* terrainType)
{
    if (this->terrainType != terrainType)
    {
        this->terrainType = terrainType;
        emit terrainTypeChanged();
    }
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
