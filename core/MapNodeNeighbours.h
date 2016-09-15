#ifndef W_CORE_NEIGHBOURS_H
#define W_CORE_NEIGHBOURS_H

#include <map>

#include "core/Hexagon.h"

namespace warmonger {
namespace core {

class MapNode;

class MapNodeNeighbours
{
public:
    typedef std::map<Direction, MapNode*> NeighbourMap;
    typedef NeighbourMap::iterator iterator;
    typedef NeighbourMap::const_iterator const_iterator;

    MapNodeNeighbours();
    MapNodeNeighbours(std::initializer_list<std::pair<Direction, MapNode*>> init);

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

    bool operator==(const MapNodeNeighbours& other);
    bool operator!=(const MapNodeNeighbours& other);

    MapNode*& operator[](const Direction direction);
    MapNode* at(const Direction direction) const;

    bool empty() const;

private:
    NeighbourMap neighbours;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_NEIGHBOURS_H
