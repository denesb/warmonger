#include "core/MapNodeNeighbours.h"

namespace warmonger {
namespace core {

MapNodeNeighbours::MapNodeNeighbours() :
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

MapNodeNeighbours::MapNodeNeighbours(std::initializer_list<std::pair<utils::Direction, MapNode*>> init) :
    neighbours{
        {utils::Direction::West, nullptr},
        {utils::Direction::NorthWest, nullptr},
        {utils::Direction::NorthEast, nullptr},
        {utils::Direction::East, nullptr},
        {utils::Direction::SouthEast, nullptr},
        {utils::Direction::SouthWest, nullptr}
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

MapNode*& MapNodeNeighbours::operator[](const utils::Direction direction)
{
    return this->neighbours[direction];
}

MapNode* MapNodeNeighbours::at(const utils::Direction direction) const
{
    return this->neighbours.at(direction);
}

bool MapNodeNeighbours::empty() const
{
    return !std::any_of(
            this->neighbours.cbegin(),
            this->neighbours.cend(),
            [](const std::pair<utils::Direction, MapNode*>& i) { return i.second != nullptr; });
}

} // namespace core
} // namespace warmonger
