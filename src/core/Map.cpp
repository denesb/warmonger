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

#include <algorithm>
#include <random>

#include "core/Map.h"
#include "utils/Logging.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

const QString factionNameTemplate{"New Faction %1"};

static std::vector<MapNode*> unserializeMapNodes(std::vector<ir::Value> serializedMapNodes, Map* map);
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

Map::Map(ir::Value v, World& world, QObject* parent)
    : QObject(parent)
{
    auto obj = std::move(v).asObject();

    if (world.getUuid() != obj["world"].asString())
        throw utils::ValueError(
            fmt::format("World mismatch, expected `{}' got `{}'", obj["world"].asString(), world.getUuid()));

    this->world = &world;

    this->name = std::move(obj["name"]).asString();
    this->mapNodes = unserializeMapNodes(std::move(obj["mapNodes"]).asList(), this);

    auto factionList = std::move(obj["factions"]).asList();
    std::transform(factionList.begin(), factionList.end(), std::back_inserter(this->factions), [this](ir::Value& v) {
        return new Faction(std::move(v), *this->world, this);
    });

    auto entityList = std::move(obj["entities"]).asList();
    std::transform(entityList.begin(), entityList.end(), std::back_inserter(this->entities), [this](ir::Value& v) {
        return new Entity(std::move(v), this->world->getRules(), this);
    });
}

ir::Value Map::serialize() const
{
    std::unordered_map<QString, ir::Value> obj;

    obj["name"] = this->name;
    obj["world"] = this->world->getUuid();

    std::vector<ir::Value> serializedMapNodes;
    std::transform(
        this->mapNodes.cbegin(), this->mapNodes.cend(), std::back_inserter(serializedMapNodes), [](MapNode* mn) {
            return mn->serialize();
        });
    obj["mapNodes"] = std::move(serializedMapNodes);

    std::vector<ir::Value> serializedFactions;
    std::transform(
        this->factions.cbegin(), this->factions.cend(), std::back_inserter(serializedFactions), [](Faction* f) {
            return f->serialize();
        });
    obj["factions"] = std::move(serializedFactions);

    std::vector<ir::Value> serializedEntities;
    std::transform(
        this->entities.cbegin(), this->entities.cend(), std::back_inserter(serializedEntities), [](Entity* e) {
            return e->serialize();
        });
    obj["entities"] = std::move(serializedEntities);

    return std::move(obj);
}

void Map::setName(const QString& name)
{
    if (this->name != name)
    {
        this->name = name;
        this->setObjectName(this->name);
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

MapNode* Map::createMapNode(ObjectId id)
{
    MapNode* mapNode = new MapNode(this, id);

    this->mapNodes.push_back(mapNode);

    wTrace << "Created map-node " << mapNode << " in map " << this;

    emit mapNodesChanged();

    return mapNode;
}

MapNode* Map::addMapNode(std::unique_ptr<MapNode> mapNode)
{
    assert(mapNode->parent() == this);

    auto mn = mapNode.get();

    this->mapNodes.push_back(mapNode.release());

    wTrace << "Added mapNode " << mn << " to map " << this;

    emit mapNodesChanged();

    return mn;
}

std::unique_ptr<MapNode> Map::removeMapNode(MapNode* mapNode)
{
    auto it = std::find(this->mapNodes.cbegin(), this->mapNodes.cend(), mapNode);
    if (it != this->mapNodes.end())
    {
        this->mapNodes.erase(it);
        mapNode->setParent(nullptr);

        QObject::disconnect(mapNode, nullptr, this, nullptr);

        wTrace << "Removed map-node " << mapNode;

        emit mapNodesChanged();

        return std::unique_ptr<MapNode>(mapNode);
    }
    else
    {
        return std::unique_ptr<MapNode>();
    }
}

Entity* Map::createEntity(QString name, ObjectId id)
{
    Entity* entity = new Entity(std::move(name), this->world->getRules(), this, id);

    this->entities.push_back(entity);

    wTrace << "Created entity " << entity << " in map " << this;

    emit entitiesChanged();

    return entity;
}

Entity* Map::addEntity(std::unique_ptr<Entity> entity)
{
    assert(entity->parent() == this);

    auto e = entity.get();

    this->entities.push_back(entity.release());

    wTrace << "Added entity " << e << " to map " << this;

    emit entitiesChanged();

    return e;
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

        wTrace << "Removed entity " << entity;

        return std::unique_ptr<Entity>(entity);
    }
    else
    {
        return std::unique_ptr<Entity>();
    }
}

Faction* Map::createFaction(ObjectId id)
{
    Faction* faction = new Faction(this, id);

    this->factions.emplace_back(faction);

    wTrace << "Created faction " << faction << " in map " << this;

    emit factionsChanged();

    return faction;
}

Faction* Map::addFaction(std::unique_ptr<Faction> faction)
{
    assert(faction->parent() == this);

    auto f = faction.get();

    this->factions.push_back(faction.release());

    wTrace << "Added faction " << f << " to map " << this;

    emit factionsChanged();

    return f;
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

        wTrace << "Removed faction " << faction;

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

bool operator==(const BannerConfiguration& a, const BannerConfiguration& b)
{
    return a.banner == b.banner && a.primaryColor == b.primaryColor && a.secondaryColor == b.secondaryColor;
}

BannerConfiguration nextAvailableBannerConfiguration(
    const World& world, const std::vector<std::unique_ptr<Faction>>& factions)
{
    std::vector<BannerConfiguration> usedConfigurations;
    usedConfigurations.reserve(factions.size());

    for (const auto& faction : factions)
    {
        usedConfigurations.emplace_back(
            *faction->getBanner(), *faction->getPrimaryColor(), *faction->getSecondaryColor());
    }

    auto& banners = world.getBanners();
    auto& colors = world.getColors();

    std::random_device rd;
    std::mt19937 mtd(rd());

    std::uniform_int_distribution<std::size_t> bannersDist(0, banners.size() - 1);
    std::uniform_int_distribution<std::size_t> colorsDist(0, colors.size() - 1);

    BannerConfiguration nextConfiguration;
    do
    {
        std::size_t primaryColorIndex = colorsDist(mtd);
        std::size_t secondaryColorIndex = colorsDist(mtd);

        if (secondaryColorIndex == primaryColorIndex)
            secondaryColorIndex = (secondaryColorIndex + 1) % colors.size();

        nextConfiguration = BannerConfiguration{
            *banners.at(bannersDist(mtd)), *colors.at(primaryColorIndex), *colors.at(secondaryColorIndex)};
    } while (
        std::find(usedConfigurations.begin(), usedConfigurations.end(), nextConfiguration) != usedConfigurations.end());

    return nextConfiguration;
}

static std::vector<MapNode*> unserializeMapNodes(std::vector<ir::Value> serializedMapNodes, Map* map)
{
    std::vector<MapNode*> mapNodes;
    std::vector<std::tuple<MapNode*, Direction, ir::Value>> neighbours;

    for (auto& element : serializedMapNodes)
    {
        auto object = std::move(element).asMap();
        auto nodeNeighbours = std::move(object["neighbours"]).asMap();

        mapNodes.push_back(new MapNode(std::move(object), map));

        // for now just store the references to the neighbours
        // they will be resolved after all mapnodes have been processed
        for (auto& nodeNeighbour : nodeNeighbours)
        {
            neighbours.emplace_back(
                mapNodes.back(), str2direction(nodeNeighbour.first), std::move(nodeNeighbour.second));
        }
    }

    MapNode* mn;
    Direction d;
    for (const auto& neighbour : neighbours)
    {
        std::tie(mn, d, std::ignore) = neighbour;
        mn->setNeighbour(d, std::get<ir::Value>(std::move(neighbour)).asReference<MapNode>(map));
    }

    return mapNodes;
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
