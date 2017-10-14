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
#include <random>

#include "core/Map.h"
#include "utils/Logging.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

const QString factionNameTemplate{"New Faction %1"};

static void addMapNodeRing(std::vector<MapNode*>& nodes, Map* map);
static MapNode* createNeighbour(MapNode* node, const Direction direction, Map* map);
static void connectWithCommonNeighbour(MapNode* n1, MapNode* n2, const Direction dn1n2, const Direction dn1n3);

Map::Map(QObject* parent)
    : QObject(parent)
    , world(nullptr)
    , mapNodeIndex(0)
    , entityIndex(0)
    , factionIndex(0)
{
}

void Map::setName(const QString& name)
{
    if (this->name != name)
    {
        this->name = name;
        emit nameChanged();
    }
}

void Map::setWorld(World* world)
{
    if (this->world != world)
    {
        this->world = world;
        emit worldChanged();
    }
}

QVariantList Map::readMapNodes() const
{
    return utils::toQVariantList(this->mapNodes);
}

QVariantList Map::readFactions() const
{
    return utils::toQVariantList(this->factions);
}

QVariantList Map::readEntities() const
{
    return utils::toQVariantList(this->entities);
}

MapNode* Map::createMapNode(int id)
{
    MapNode* mapNode = new MapNode(this, id);

    this->mapNodes.push_back(mapNode);

    wDebug << "Created map-node " << mapNode << " in map " << this;

    emit mapNodesChanged();

    return mapNode;
}

std::unique_ptr<MapNode> Map::removeMapNode(MapNode* mapNode)
{
    auto it = std::find(this->mapNodes.cbegin(), this->mapNodes.cend(), mapNode);
    if (it != this->mapNodes.end())
    {
        this->mapNodes.erase(it);
        mapNode->setParent(nullptr);

        QObject::disconnect(mapNode, nullptr, this, nullptr);

        wDebug << "Removed map-node " << mapNode;

        emit mapNodesChanged();

        return std::unique_ptr<MapNode>(mapNode);
    }
    else
    {
        return std::unique_ptr<MapNode>();
    }
}

Entity* Map::createEntity(int id)
{
    Entity* entity = new Entity(this, id);

    this->entities.push_back(entity);

    wDebug << "Created entity " << entity << " in map " << this;

    emit entitiesChanged();

    return entity;
}

std::unique_ptr<Entity> Map::removeEntity(Entity* entity)
{
    auto it = std::find(this->entities.cbegin(), this->entities.cend(), entity);
    if (it != this->entities.end())
    {
        this->entities.erase(it);
        entity->setParent(nullptr);

        emit entitiesChanged();

        QObject::disconnect(entity, nullptr, this, nullptr);

        wDebug << "Removed entity " << entity;

        return std::unique_ptr<Entity>(entity);
    }
    else
    {
        return std::unique_ptr<Entity>();
    }
}

Faction* Map::createFaction(int id)
{
    Faction* faction = new Faction(this, id);

    this->factions.emplace_back(faction);

    wDebug << "Created faction " << faction << " in map " << this;

    emit factionsChanged();

    return faction;
}

std::unique_ptr<Faction> Map::removeFaction(Faction* faction)
{
    const auto it = std::remove(this->factions.begin(), this->factions.end(), faction);

    if (it == this->factions.end())
    {
        return std::unique_ptr<Faction>();
    }
    else
    {
        this->factions.erase(it);

        faction->setParent(nullptr);

        QObject::disconnect(faction, nullptr, this, nullptr);

        wDebug << "Removed faction " << faction;

        emit factionsChanged();

        return std::unique_ptr<Faction>(faction);
    }
}

void Map::generateMapNodes(unsigned int radius)
{
    if (radius == 0)
    {
        return;
    }

    std::vector<MapNode*> generatedMapNodes;

    generatedMapNodes.emplace_back(new MapNode(this));

    for (unsigned i = 1; i < radius; ++i)
    {
        addMapNodeRing(generatedMapNodes, this);
    }

    for (auto mapNode : this->mapNodes)
    {
        delete mapNode;
    }

    this->mapNodes = generatedMapNodes;

    emit mapNodesChanged();
}

static void addMapNodeRing(std::vector<MapNode*>& nodes, Map* map)
{
    std::vector<MapNode*> newNodes;

    for (MapNode* node : nodes)
    {
        for (Direction direction : directions)
        {
            if (node->getNeighbour(direction) == nullptr)
                newNodes.push_back(createNeighbour(node, direction, map));
        }
    }

    std::copy(newNodes.begin(), newNodes.end(), std::back_inserter(nodes));
}

static MapNode* createNeighbour(MapNode* node, const Direction direction, Map* map)
{
    MapNode* newNode = new MapNode(map);

    node->setNeighbour(direction, newNode);
    newNode->setNeighbour(oppositeDirection(direction), node);

    const std::pair<Direction, Direction> axis = neighbourDirections(direction);

    connectWithCommonNeighbour(node, newNode, direction, std::get<0>(axis));
    connectWithCommonNeighbour(node, newNode, direction, std::get<1>(axis));

    return newNode;
}

static void connectWithCommonNeighbour(MapNode* n1, MapNode* n2, const Direction dn1n2, const Direction dn1n3)
{
    MapNode* n3 = n1->getNeighbour(dn1n3);

    if (n3 == nullptr)
        return;

    std::pair<Direction, Direction> connectionAxis = connectingDirections(dn1n2, dn1n3);

    n2->setNeighbour(std::get<0>(connectionAxis), n3);
    n3->setNeighbour(std::get<1>(connectionAxis), n2);
}

} // namespace core
} // namespace warmonger
