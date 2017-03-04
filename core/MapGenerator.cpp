/**
 * Copyright (C) 2015-2017 Botond Dénes
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

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iterator>
#include <utility>

#include "core/MapGenerator.h"

namespace warmonger {
namespace core {

namespace {

void addMapNodeRing(std::vector<MapNode*>& nodes);
MapNode* createNeighbour(MapNode* node, const Direction direction);
void connectWithCommonNeighbour(MapNode* n1, MapNode* n2, const Direction dn1n2, const Direction dn1n3);
}

std::vector<MapNode*> generateMapNodes(unsigned int radius)
{
    std::vector<MapNode*> nodes;

    if (radius == 0)
    {
        return nodes;
    }

    MapNode* mapNode = new MapNode();
    nodes.push_back(mapNode);

    for (unsigned i = 1; i < radius; ++i)
    {
        addMapNodeRing(nodes);
    }

    return nodes;
}

namespace {

void addMapNodeRing(std::vector<MapNode*>& nodes)
{
    std::vector<MapNode*> newNodes;

    for (MapNode* node : nodes)
    {
        for (Direction direction : directions)
        {
            if (node->getNeighbour(direction) == nullptr)
                newNodes.push_back(createNeighbour(node, direction));
        }
    }

    std::copy(newNodes.begin(), newNodes.end(), std::back_inserter(nodes));
}

MapNode* createNeighbour(MapNode* node, const Direction direction)
{
    MapNode* newNode = new MapNode();

    node->setNeighbour(direction, newNode);
    newNode->setNeighbour(oppositeDirection(direction), node);

    const std::pair<Direction, Direction> axis = neighbourDirections(direction);

    connectWithCommonNeighbour(node, newNode, direction, std::get<0>(axis));
    connectWithCommonNeighbour(node, newNode, direction, std::get<1>(axis));

    return newNode;
}

void connectWithCommonNeighbour(MapNode* n1, MapNode* n2, const Direction dn1n2, const Direction dn1n3)
{
    MapNode* n3 = n1->getNeighbour(dn1n3);

    if (n3 == nullptr)
        return;

    std::pair<Direction, Direction> connectionAxis = connectingDirections(dn1n2, dn1n3);

    n2->setNeighbour(std::get<0>(connectionAxis), n3);
    n3->setNeighbour(std::get<1>(connectionAxis), n2);
}
}

} // namespace core
} // namespace warmonger
