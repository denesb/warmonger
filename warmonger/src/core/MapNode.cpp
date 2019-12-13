/**
 * Copyright (C) 2015-2018 Botond Dénes
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

MapNodeNeighbours::MapNodeNeighbours()
    : neighbours{{Direction::West, nullptr},
          {Direction::NorthWest, nullptr},
          {Direction::NorthEast, nullptr},
          {Direction::East, nullptr},
          {Direction::SouthEast, nullptr},
          {Direction::SouthWest, nullptr}}
{
}

MapNodeNeighbours::MapNodeNeighbours(std::initializer_list<std::pair<Direction, MapNode*>> init)
    : neighbours{{Direction::West, nullptr},
          {Direction::NorthWest, nullptr},
          {Direction::NorthEast, nullptr},
          {Direction::East, nullptr},
          {Direction::SouthEast, nullptr},
          {Direction::SouthWest, nullptr}}
{
    for (auto value : init)
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
    return !std::any_of(this->neighbours.cbegin(),
        this->neighbours.cend(),
        [](const std::pair<Direction, MapNode*>& i) { return i.second != nullptr; });
}

MapNode::MapNode(QObject* parent, ObjectId id)
    : WObject(parent, id)
{
}

MapNode::MapNode(ir::Value v, QObject* parent)
    : WObject(parent, v.getObjectId())
{
}

ir::Value MapNode::serialize() const
{
    std::unordered_map<QString, ir::Value> obj;

    obj["id"] = this->getId().get();

    std::unordered_map<QString, ir::Value> serializedNeigbours;
    std::transform(this->neighbours.cbegin(),
        this->neighbours.cend(),
        std::inserter(serializedNeigbours, serializedNeigbours.begin()),
        [](std::pair<Direction, MapNode*> n) { return std::make_pair(direction2str(n.first), ir::Value(n.second)); });
    obj["neighbours"] = std::move(serializedNeigbours);

    return obj;
}

void MapNode::setNeighbours(MapNodeNeighbours neighbours)
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

void MapNode::setTerrainType(QString terrainType)
{
    if (this->terrainType != terrainType)
    {
        this->terrainType = terrainType;
        emit terrainTypeChanged();
    }
}

} // namespace core
} // namespace warmonger
