/** \file
 * MapNode class.
 *
 * \copyright (C) 2015-2017 Botond Dénes
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

#ifndef W_CORE_MAP_NODE_H
#define W_CORE_MAP_NODE_H

#include <map>

#include "core/Hexagon.h"
#include "core/MapNodeNeighbours.h"
#include "core/WObject.h"

namespace warmonger {
namespace core {

/**
 * The basic building block of any game map.
 *
 * Map-nodes form the map. A map-node has a list of it's neighbours. The
 * map-nodes together form a graph that defines the structure of the map.
 * The granularity of positioning in the game is the map-node.
 * The map-node is assumed to be hexagonal, and thus has exactly six
 * neighbours.
 */
class MapNode : public WObject
{
    Q_OBJECT

public:
    template <class Visitor>
    static auto describe(Visitor&& visitor)
    {
        return visitor.template visitParent<WObject>()
            .visitMember("neighbours", &MapNode::getNeighbours, &MapNode::setNeighbours)
            .template visitConstructor<QObject*, int>("parent", "id");
    }

    /**
     * Constructs an empty map-node.
     *
     * \param parent the parent QObject.
     * \param id the id
     *
     * \see WObject::WObject
     */
    MapNode(QObject* parent, int id = WObject::invalidId);

    /**
     * Get he neighbours of this map.
     *
     * \returns the neighbours
     */
    const MapNodeNeighbours& getNeighbours() const
    {
        return this->neighbours;
    }

    /**
     * Set the neighbours.
     *
     * Will emit the signal MapNode::neighboursChanged() if the newly set value
     * is different than the current one.
     *
     * \param neighbours the neigbours
     */
    void setNeighbours(MapNodeNeighbours neighbours);

    /**
     * Get the neighbours for the given direction.
     *
     * \param direction the direction
     *
     * \returns the neighbour for the direction
     *
     * \see warmonger::core::Direction
     */
    MapNode* getNeighbour(Direction direction) const
    {
        return this->neighbours.at(direction);
    }

    /**
     * Set the the given neighbour for the given direction.
     *
     * Will emit the signal MapNode::neighboursChanged() if the newly set value
     * is different than the current one.
     *
     * \param direction the direction
     * \param mapNode the new neighbour
     */
    void setNeighbour(Direction direction, MapNode* mapNode);

signals:
    /**
     * Emitted when the map-nodes change.
     */
    void neighboursChanged();

private:
    MapNodeNeighbours neighbours;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_MAP_NODE_H
