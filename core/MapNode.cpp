#include "core/MapNode.h"
#include "utils/Exception.h"

namespace warmonger {
namespace core {

namespace {

void fillNeighbours(std::map<utils::Direction, MapNode *> &neighbours);
bool neighboursAreEquivalent(
        const std::map<utils::Direction, MapNode *> &n1,
        const std::map<utils::Direction, MapNode *> &n2
    );

}

MapNode::MapNode(QObject *parent) :
    QObject(parent),
    terrainType(nullptr),
    neighbours{
        {utils::Direction::West, nullptr},
        {utils::Direction::NorthWest, nullptr},
        {utils::Direction::NorthEast, nullptr},
        {utils::Direction::East, nullptr},
        {utils::Direction::SouthEast, nullptr},
        {utils::Direction::SouthWest, nullptr}
    }
{
}

QString MapNode::getDisplayName() const
{
    return this->displayName;
}

void MapNode::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

TerrainType * MapNode::getTerrainType() const
{
    return this->terrainType;
}

void MapNode::setTerrainType(TerrainType *terrainType)
{
    if (this->terrainType != terrainType)
    {
        this->terrainType = terrainType;
        emit terrainTypeChanged();
    }
}

std::map<utils::Direction, MapNode *> MapNode::getNeighbours() const
{
    return this->neighbours;
}

void MapNode::setNeighbours(const std::map<utils::Direction, MapNode *> &neighbours)
{
    if (!neighboursAreEquivalent(this->neighbours, neighbours))
    {
        this->neighbours = neighbours;
        fillNeighbours(this->neighbours);

        emit neighboursChanged();
    }
}

void MapNode::setNeighbours(std::map<utils::Direction, MapNode *> &&neighbours)
{
    if (!neighboursAreEquivalent(this->neighbours, neighbours))
    {
        this->neighbours = std::move(neighbours);
        fillNeighbours(this->neighbours);

        emit neighboursChanged();
    }
}

MapNode * MapNode::getNeighbour(utils::Direction direction) const
{
    return this->neighbours.at(direction);
}

void MapNode::setNeighbour(utils::Direction direction, MapNode *mapNode)
{
    if (this->neighbours[direction] != mapNode)
    {
        this->neighbours[direction] = mapNode;
        emit neighboursChanged();
    }
}

namespace {

void fillNeighbours(std::map<utils::Direction, MapNode *> &neighbours)
{
    for (utils::Direction d : utils::directions)
    {
        auto it = neighbours.find(d);
        if (it == neighbours.end())
        {
            neighbours[d] = nullptr;
        }
    }
}

bool neighboursAreEquivalent(
        const std::map<utils::Direction, MapNode *> &n1,
        const std::map<utils::Direction, MapNode *> &n2
    )
{
    bool equivalent{true};

    for (utils::Direction d : utils::directions)
    {
        auto it1 = n1.find(d);
        auto it2 = n2.find(d);

        MapNode *mn1{nullptr}, *mn2{nullptr};

        if (it1 != n1.end())
        {
            mn1 = it1->second;
        }

        if (it2 != n2.end())
        {
            mn2 = it2->second;
        }

        equivalent &= mn1 == mn2;
    }

    return equivalent;
}

}

} // namespace core
} // namespace warmonger
