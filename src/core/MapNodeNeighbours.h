/** \file
 * MapNodeNeighbours class.
 *
 * \copyright (C) 2015-2018 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef W_CORE_NEIGHBOURS_H
#define W_CORE_NEIGHBOURS_H

#include <map>

#include "core/Hexagon.h"

namespace warmonger {
namespace core {

class MapNode;

/**
 * The neighbours of a map-node.
 *
 * This class ensures that the map-node registry of a map-node is always
 * consistent, after creation, insertion, removal or change.
 * A map-node is expected to store nullptr for direction for which it doesn't
 * have a neighbour.
 */
class MapNodeNeighbours
{
public:
    typedef std::map<Direction, MapNode*> NeighbourMap;
    typedef NeighbourMap::iterator iterator;
    typedef NeighbourMap::const_iterator const_iterator;
    typedef std::pair<Direction, MapNode*> value_type;

    /**
     * Construct an empty neighbours object.
     */
    MapNodeNeighbours();

    /**
     * Construct an empty neighbours object from the initializer list.
     */
    MapNodeNeighbours(std::initializer_list<std::pair<Direction, MapNode*>> init);

    /**
     * Get the size (always 6).
     */
    std::size_t size() const
    {
        return 6;
    }

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
