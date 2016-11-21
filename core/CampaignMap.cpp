/**
 * Copyright (C) 2015-2016 Botond Dénes
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
#include "core/TerrainType.h"
#include "utils/Logging.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

const QString mapNodeNameTemplate{"mapNode-%1"};
const QString settlementNameTemplate{"settlement-%1"};
const QString unitNameTemplate{"unit-%1"};
const QString armyNameTemplate{"army-%1"};
const QString factionObjectNameTemplate{"faction-%1"};
const QString factionDisplayNameTemplate{"New Faction %1"};

template <class ObjectList>
static int nextNameIndex(const ObjectList& list)
{
    int index{0};

    for (const auto object : list)
    {
        index = std::max(index, object->objectName().section('-', -1, -1).toInt());
    }

    return ++index;
}

static void destroy(QObject* obj)
{
    delete obj;
}

std::tuple<Banner*, QColor, QColor> nextAvailableCombination(
    const std::vector<Faction*>& factions, const std::vector<Banner*>& banners, const std::vector<QColor>& colors);

CampaignMap::CampaignMap(QObject* parent)
    : QObject(parent)
    , world(nullptr)
    , mapNodeIndex(0)
    , settlementIndex(0)
    , unitIndex(0)
    , armyIndex(0)
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

void CampaignMap::setMapNodes(const std::vector<MapNode*>& mapNodes)
{
    if (this->mapNodes != mapNodes)
    {
        std::for_each(this->mapNodes.begin(), this->mapNodes.end(), destroy);

        this->mapNodes = mapNodes;

        for (MapNode* node : this->mapNodes)
            node->setParent(this);

        this->mapNodeIndex = nextNameIndex(this->mapNodes);

        this->rebuildContents();

        emit mapNodesChanged();
    }
}

QVariantList CampaignMap::readSettlements() const
{
    return utils::toQVariantList(this->settlements);
}

void CampaignMap::setSettlements(const std::vector<Settlement*>& settlements)
{
    if (this->settlements != settlements)
    {
        std::for_each(this->settlements.begin(), this->settlements.end(), destroy);

        this->settlements = settlements;

        for (Settlement* settlement : this->settlements)
        {
            settlement->setParent(this);
            QObject::connect(settlement, &Settlement::mapNodeChanged, this, &CampaignMap::settlementMapNodeChanged);
        }

        this->settlementIndex = nextNameIndex(this->settlements);

        this->settlementMapNodeChanged();

        emit settlementsChanged();
    }
}

void CampaignMap::setUnits(const std::vector<Unit*>& units)
{
    if (this->units != units)
    {
        std::for_each(this->units.begin(), this->units.end(), destroy);

        this->units = units;

        for (Unit* unit : this->units)
            unit->setParent(this);

        this->unitIndex = nextNameIndex(this->units);

        emit unitsChanged();
    }
}

QVariantList CampaignMap::readUnits() const
{
    return utils::toQVariantList(this->units);
}

void CampaignMap::setArmies(const std::vector<Army*>& armies)
{
    if (this->armies != armies)
    {
        std::for_each(this->armies.begin(), this->armies.end(), destroy);

        this->armies = armies;

        for (Army* army : this->armies)
        {
            army->setParent(this);
            QObject::connect(army, &Army::mapNodeChanged, this, &CampaignMap::armyMapNodeChanged);
        }

        this->armyIndex = nextNameIndex(this->armies);

        this->armyMapNodeChanged();

        emit armiesChanged();
    }
}

QVariantList CampaignMap::readArmies() const
{
    return utils::toQVariantList(this->armies);
}

QVariantList CampaignMap::readFactions() const
{
    return utils::toQVariantList(this->factions);
}

void CampaignMap::setFactions(const std::vector<Faction*>& factions)
{
    if (this->factions != factions)
    {
        std::for_each(this->factions.begin(), this->factions.end(), destroy);

        this->factions = factions;

        this->factionIndex = nextNameIndex(this->factions);

        emit factionsChanged();
    }
}

MapNode* CampaignMap::createMapNode(TerrainType* terrainType, const MapNodeNeighbours& neighbours)
{
    MapNode* mapNode = new MapNode(this);

    mapNode->setObjectName(mapNodeNameTemplate.arg(this->mapNodeIndex++));

    mapNode->setTerrainType(terrainType);

    for (const std::pair<Direction, MapNode*>& neighbour : neighbours)
    {
        if (neighbour.second != nullptr)
        {
            mapNode->setNeighbour(neighbour.first, neighbour.second);
            neighbour.second->setNeighbour(oppositeDirection(neighbour.first), mapNode);
        }
    }

    this->mapNodes.push_back(mapNode);

    this->contents.push_back(std::tuple<MapNode*, Settlement*, Army*>(mapNode, nullptr, nullptr));

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

        const auto contentIt = std::find_if(this->contents.begin(), this->contents.end(), [&](const Content& content) {
            return std::get<0>(content) == mapNode;
        });

        if (contentIt != this->contents.end())
            this->contents.erase(contentIt);

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

Settlement* CampaignMap::createSettlement(SettlementType* settlementType)
{
    Settlement* settlement = new Settlement(this);

    settlement->setObjectName(settlementNameTemplate.arg(this->settlementIndex++));
    settlement->setType(settlementType);

    this->settlements.push_back(settlement);

    QObject::connect(settlement, &Settlement::mapNodeChanged, this, &CampaignMap::settlementMapNodeChanged);

    wDebug << "Created settlement " << settlement;

    emit settlementsChanged();

    return settlement;
}

std::unique_ptr<Settlement> CampaignMap::removeSettlement(Settlement* settlement)
{
    auto it = std::find(this->settlements.cbegin(), this->settlements.cend(), settlement);
    if (it != this->settlements.end())
    {
        this->settlements.erase(it);
        settlement->setParent(nullptr);

        emit settlementsChanged();

        QObject::disconnect(settlement, nullptr, this, nullptr);

        wDebug << "Removed settlement " << settlement;

        this->settlementMapNodeChanged();

        return std::unique_ptr<Settlement>(settlement);
    }
    else
    {
        return std::unique_ptr<Settlement>();
    }
}

Unit* CampaignMap::createUnit(UnitType* unitType)
{
    Unit* unit = new Unit(this);

    unit->setObjectName(unitNameTemplate.arg(this->unitIndex++));

    unit->setType(unitType);

    this->units.push_back(unit);

    wDebug << "Created unit " << unit;

    emit unitsChanged();

    return unit;
}

std::unique_ptr<Unit> CampaignMap::removeUnit(Unit* unit)
{
    auto it = std::find(this->units.cbegin(), this->units.cend(), unit);
    if (it != this->units.end())
    {
        this->units.erase(it);
        unit->setParent(nullptr);

        wDebug << "Removed unit " << unit;

        emit unitsChanged();

        return std::unique_ptr<Unit>(unit);
    }
    else
    {
        return std::unique_ptr<Unit>();
    }
}

Army* CampaignMap::createArmy(ArmyType* armyType)
{
    Army* army = new Army(this);

    army->setObjectName(armyNameTemplate.arg(this->armyIndex++));

    army->setType(armyType);

    this->armies.push_back(army);

    QObject::connect(army, &Army::mapNodeChanged, this, &CampaignMap::armyMapNodeChanged);

    wDebug << "Created army " << army;

    emit armiesChanged();

    return army;
}

std::unique_ptr<Army> CampaignMap::removeArmy(Army* army)
{
    auto it = std::remove(this->armies.begin(), this->armies.end(), army);

    if (it == this->armies.end())
    {
        return std::unique_ptr<Army>();
    }
    else
    {
        this->armies.erase(it);

        army->setParent(nullptr);

        QObject::disconnect(army, nullptr, this, nullptr);

        wDebug << "Removed army " << army;

        emit armiesChanged();

        return std::unique_ptr<Army>(army);
    }
}

Faction* CampaignMap::createFaction(Civilization* civilization)
{
    this->factions.emplace_back(new Faction(this));

    Faction* faction = this->factions.back();

    faction->setObjectName(factionObjectNameTemplate.arg(this->factionIndex));
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

void CampaignMap::rebuildContents()
{
    this->contents.clear();

    for (MapNode* mapNode : this->mapNodes)
    {
        const auto settlementIt = std::find_if(this->settlements.begin(),
            this->settlements.end(),
            [&](const Settlement* settlement) { return settlement->getMapNode() == mapNode; });

        Settlement* settlement = (settlementIt == this->settlements.end()) ? nullptr : *settlementIt;

        const auto armyIt = std::find_if(
            this->armies.begin(), this->armies.end(), [&](const Army* army) { return army->getMapNode() == mapNode; });

        Army* army = (armyIt == this->armies.end()) ? nullptr : *armyIt;

        this->contents.emplace_back(mapNode, settlement, army);
    }
}

void CampaignMap::settlementMapNodeChanged()
{
    const std::vector<Content> oldContents(this->contents);

    this->contents.clear();

    for (std::size_t i = 0; i < this->mapNodes.size(); ++i)
    {
        MapNode* mapNode = this->mapNodes[i];
        Army* army = std::get<2>(oldContents[i]);

        const auto it = std::find_if(this->settlements.begin(),
            this->settlements.end(),
            [&](const Settlement* settlement) { return settlement->getMapNode() == mapNode; });

        Settlement* settlement = (it == this->settlements.end()) ? nullptr : *it;

        this->contents.emplace_back(mapNode, settlement, army);
    }
}

void CampaignMap::armyMapNodeChanged()
{
    const std::vector<Content> oldContents(this->contents);

    this->contents.clear();

    for (std::size_t i = 0; i < this->mapNodes.size(); ++i)
    {
        MapNode* mapNode = this->mapNodes[i];
        Settlement* settlement = std::get<1>(oldContents[i]);

        const auto it = std::find_if(
            this->armies.begin(), this->armies.end(), [&](const Army* army) { return army->getMapNode() == mapNode; });

        Army* army = (it == this->armies.end()) ? nullptr : *it;

        this->contents.emplace_back(mapNode, settlement, army);
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
