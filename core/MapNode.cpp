#include "core/Exception.h"
#include "core/MapNode.h"

static const QString loggerName{"core.MapNode"};

using namespace warmonger;
using namespace core;

static const std::vector<Direction> directions{
    Direction::West,
    Direction::NorthWest,
    Direction::NorthEast,
    Direction::East,
    Direction::SouthEast,
    Direction::SouthWest
};

void fillNeighbours(std::map<Direction, MapNode *> &neighbours);
bool neighboursAreEquivalent(const std::map<Direction, MapNode *> &n1, const std::map<Direction, MapNode *> &n2);

namespace warmonger {
namespace core {

QString direction2str(Direction d)
{
    static const std::map<Direction, QString> dir2str{
        {Direction::West, "West"},
        {Direction::NorthWest, "NorthWest"},
        {Direction::NorthEast, "NorthEast"},
        {Direction::East, "East"},
        {Direction::SouthEast, "SouthEast"},
        {Direction::SouthWest, "SouthWest"}
    };
    return dir2str.at(d);
}

Direction str2direction(const QString &str)
{
    static const std::map<QString, Direction> str2dir{
        {"West", Direction::West},
        {"NorthWest", Direction::NorthWest},
        {"NorthEast", Direction::NorthEast},
        {"East", Direction::East},
        {"SouthEast", Direction::SouthEast},
        {"SouthWest", Direction::SouthWest}
    };

    if (str2dir.find(str) == str2dir.end())
    {
        throw ValueError(str + " is not a valid direction");
    }

    return str2dir.at(str);
}

Direction oppositeDirection(Direction d)
{
    static const std::map<Direction, Direction> oppositeDirs{
        {Direction::West, Direction::East},
        {Direction::NorthWest, Direction::SouthEast},
        {Direction::NorthEast, Direction::SouthWest},
        {Direction::East, Direction::West},
        {Direction::SouthEast, Direction::NorthWest},
        {Direction::SouthWest, Direction::NorthEast}
    };

    return oppositeDirs.at(d);
}

MapNode::MapNode(QObject *parent) :
    QObject(parent),
    terrainType(nullptr),
    neighbours{
        {Direction::West, nullptr},
        {Direction::NorthWest, nullptr},
        {Direction::NorthEast, nullptr},
        {Direction::East, nullptr},
        {Direction::SouthEast, nullptr},
        {Direction::SouthWest, nullptr}
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

std::map<Direction, MapNode *> MapNode::getNeighbours() const
{
    return this->neighbours;
}

void MapNode::setNeighbours(const std::map<Direction, MapNode *> &neighbours)
{
    if (!neighboursAreEquivalent(this->neighbours, neighbours))
    {
        this->neighbours = neighbours;
        fillNeighbours(this->neighbours);

        emit neighboursChanged();
    }
}

void MapNode::setNeighbours(std::map<Direction, MapNode *> &&neighbours)
{
    if (!neighboursAreEquivalent(this->neighbours, neighbours))
    {
        this->neighbours = std::move(neighbours);
        fillNeighbours(this->neighbours);

        emit neighboursChanged();
    }
}

MapNode * MapNode::getNeighbour(Direction direction) const
{
    return this->neighbours.at(direction);
}

void MapNode::setNeighbour(Direction direction, MapNode *mapNode)
{
    if (this->neighbours[direction] != mapNode)
    {
        this->neighbours[direction] = mapNode;
        emit neighboursChanged();
    }
}

} // namespace core
} // namespace warmonger

void fillNeighbours(std::map<Direction, MapNode *> &neighbours)
{
    for (Direction d : directions)
    {
        auto it = neighbours.find(d);
        if (it == neighbours.end())
        {
            neighbours[d] = nullptr;
        }
    }
}

bool neighboursAreEquivalent(const std::map<Direction, MapNode *> &n1, const std::map<Direction, MapNode *> &n2)
{
    bool equivalent{true};

    for (Direction d : directions)
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
