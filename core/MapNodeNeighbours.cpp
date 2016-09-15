#include "core/MapNodeNeighbours.h"

namespace warmonger {
namespace core {

MapNodeNeighbours::MapNodeNeighbours() :
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

MapNodeNeighbours::MapNodeNeighbours(std::initializer_list<std::pair<Direction, MapNode*>> init) :
    neighbours{
        {Direction::West, nullptr},
        {Direction::NorthWest, nullptr},
        {Direction::NorthEast, nullptr},
        {Direction::East, nullptr},
        {Direction::SouthEast, nullptr},
        {Direction::SouthWest, nullptr}
    }
{
    for(auto value : init)
    {
        this->neighbours[value.first] = value.second;
    }
}

MapNodeNeighbours::iterator MapNodeNeighbours::begin()
{
    return this->neighbours.begin();
}

MapNodeNeighbours::const_iterator MapNodeNeighbours::begin() const
{
    return this->neighbours.begin();
}

MapNodeNeighbours::const_iterator MapNodeNeighbours::cbegin() const
{
    return this->neighbours.cbegin();
}

MapNodeNeighbours::iterator MapNodeNeighbours::end()
{
    return this->neighbours.end();
}

MapNodeNeighbours::const_iterator MapNodeNeighbours::end() const
{
    return this->neighbours.end();
}

MapNodeNeighbours::const_iterator MapNodeNeighbours::cend() const
{
    return this->neighbours.cend();
}

bool MapNodeNeighbours::operator==(const MapNodeNeighbours& other)
{
    return this->neighbours == other.neighbours;
}

bool MapNodeNeighbours::operator!=(const MapNodeNeighbours& other)
{
    return this->neighbours != other.neighbours;
}

MapNode*& MapNodeNeighbours::operator[](const Direction direction)
{
    return this->neighbours[direction];
}

MapNode* MapNodeNeighbours::at(const Direction direction) const
{
    return this->neighbours.at(direction);
}

bool MapNodeNeighbours::empty() const
{
    return !std::any_of(
            this->neighbours.cbegin(),
            this->neighbours.cend(),
            [](const std::pair<Direction, MapNode*>& i) { return i.second != nullptr; });
}

} // namespace core
} // namespace warmonger
