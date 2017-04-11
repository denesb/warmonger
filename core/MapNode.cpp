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

#include "core/MapNode.h"
#include "utils/Exception.h"

namespace warmonger {
namespace core {

MapNode::MapNode(long id)
    : WObject(id)
{
}

MapNode::MapNode(QObject* parent)
    : WObject(parent)
{
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
