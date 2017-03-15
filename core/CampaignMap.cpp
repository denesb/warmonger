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

#include <boost/optional.hpp>

#include "core/CampaignMap.h"
#include "utils/Logging.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

const QString factionDisplayNameTemplate{"New Faction %1"};

std::tuple<Banner*, QColor, QColor> nextAvailableCombination(
    const std::vector<Faction*>& factions, const std::vector<Banner*>& banners, const std::vector<QColor>& colors);

CampaignMap::CampaignMap(QObject* parent)
    : QObject(parent)
    , world(nullptr)
    , mapNodeIndex(0)
    , entityIndex(0)
    , factionIndex(0)
{
}

void CampaignMap::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

void CampaignMap::setWorld(World* world)
{
    if (this->world != world)
    {
        this->world = world;
        emit worldChanged();
    }
}

QVariantList CampaignMap::readMapNodes() const
{
    return utils::toQVariantList(this->mapNodes);
}

QVariantList CampaignMap::readFactions() const
{
    return utils::toQVariantList(this->factions);
}

QVariantList CampaignMap::readEntities() const
{
    return utils::toQVariantList(this->entities);
}

void CampaignMap::addMapNode(std::unique_ptr<MapNode>&& mapNode)
{
    mapNode->setParent(this);

    this->mapNodes.push_back(mapNode.release());

    wDebug << "Added map-node " << this->mapNodes.back();

    emit mapNodesChanged();
}

MapNode* CampaignMap::createMapNode(const MapNodeNeighbours& neighbours)
{
    MapNode* mapNode = new MapNode(this);

    mapNode->setObjectName(QString::number(this->mapNodeIndex++));

    for (const std::pair<Direction, MapNode*>& neighbour : neighbours)
    {
        if (neighbour.second != nullptr)
        {
            mapNode->setNeighbour(neighbour.first, neighbour.second);
            neighbour.second->setNeighbour(oppositeDirection(neighbour.first), mapNode);
        }
    }

    this->mapNodes.push_back(mapNode);

    wDebug << "Created map-node " << mapNode;

    emit mapNodesChanged();

    return mapNode;
}

std::unique_ptr<MapNode> CampaignMap::removeMapNode(MapNode* mapNode)
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

void CampaignMap::addEntity(std::unique_ptr<Entity>&& entity)
{
    entity->setParent(this);

    this->entities.push_back(entity.get());

    wDebug << "Added entity " << entity.get() << " to world " << this;

    entity.reset();

    emit entitiesChanged();
}

Entity* CampaignMap::createEntity(EntityType* entityType)
{
    Entity* entity = new Entity(entityType, this);

    entity->setObjectName(QString::number(this->entityIndex++));

    this->entities.push_back(entity);

    wDebug << "Created entity " << entity;

    emit entitiesChanged();

    return entity;
}

std::unique_ptr<Entity> CampaignMap::removeEntity(Entity* entity)
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

Faction* CampaignMap::createFaction(Civilization* civilization)
{
    this->factions.emplace_back(new Faction(this));

    Faction* faction = this->factions.back();

    faction->setObjectName(QString::number(this->factionIndex));
    faction->setDisplayName(factionDisplayNameTemplate.arg(this->factionIndex++));

    faction->setCivilization(civilization);

    const auto combination =
        nextAvailableCombination(this->factions, this->world->getBanners(), this->world->getColors());

    faction->setBanner(std::get<0>(combination));
    faction->setPrimaryColor(std::get<1>(combination));
    faction->setSecondaryColor(std::get<2>(combination));

    wDebug << "Created faction " << faction;

    emit factionsChanged();

    return faction;
}

void CampaignMap::addFaction(std::unique_ptr<Faction>&& faction)
{
    faction->setParent(this);

    this->factions.push_back(faction.get());

    wDebug << "Added faction " << faction.get() << " to world " << this;

    faction.reset();

    emit factionsChanged();
}

std::unique_ptr<Faction> CampaignMap::removeFaction(Faction* faction)
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

std::tuple<Banner*, QColor, QColor> nextAvailableCombination(
    const std::vector<Faction*>& factions, const std::vector<Banner*>& banners, const std::vector<QColor>& colors)
{
    typedef std::tuple<Banner*, QColor, QColor> Combination;

    std::vector<Combination> usedCombinations;

    for (const auto& faction : factions)
    {
        usedCombinations.emplace_back(faction->getBanner(), faction->getPrimaryColor(), faction->getSecondaryColor());
    }

    std::vector<Banner*> shuffledBanners(banners);
    std::vector<QColor> shuffledColors(colors);
    std::random_device rd;
    std::mt19937 mtd(rd());

    std::shuffle(shuffledBanners.begin(), shuffledBanners.end(), mtd);
    std::shuffle(shuffledColors.begin(), shuffledColors.end(), mtd);

    std::uniform_int_distribution<std::size_t> bannersDist(0, banners.size() - 1);
    std::uniform_int_distribution<std::size_t> colorsDist(0, colors.size() - 1);

    Combination nextCombination;
    do
    {
        std::size_t primaryColorIndex = colorsDist(mtd);
        std::size_t secondaryColorIndex = colorsDist(mtd);

        if (secondaryColorIndex == primaryColorIndex)
            secondaryColorIndex = (secondaryColorIndex + 1) % colors.size();

        nextCombination =
            std::make_tuple(banners.at(bannersDist(mtd)), colors.at(primaryColorIndex), colors.at(secondaryColorIndex));
    } while (std::find(usedCombinations.begin(), usedCombinations.end(), nextCombination) != usedCombinations.end());

    return nextCombination;
}

} // namespace core
} // namespace warmonger
