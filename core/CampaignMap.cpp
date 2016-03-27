#include <algorithm>

#include "core/CampaignMap.h"
#include "core/QVariantUtil.h"
#include "core/TerrainType.h"
#include "core/WorldSurface.h"
#include "core/UnitClass.h"

using namespace warmonger::core;

const QString loggerName{"core.Map"};

const QString mapNodeNameTemplate{"mapNode%1"};
const QString settlementNameTemplate{"settlement%1"};
const QString unitNameTemplate{"unit%1"};
const QString armyNameTemplate{"unit%1"};

namespace warmonger {
namespace core {

CampaignMap::CampaignMap(QObject *parent) :
    QObject(parent),
    world(nullptr),
    mapNodeIndex(0),
    settlementIndex(0),
    unitIndex(0),
    armyIndex(0),
    mapNodes(),
    factions(),
    settlements(),
    units()
{
}

World * CampaignMap::getWorld() const
{
    return this->world;
}

QString CampaignMap::getDisplayName() const
{
    return this->displayName;
}

void CampaignMap::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

void CampaignMap::setWorld(World *world)
{
    if (this->world != world)
    {
        this->world = world;
        emit worldChanged();
    }
}

int CampaignMap::getMapNodeIndex() const
{
    return this->mapNodeIndex;
}

void CampaignMap::setMapNodeIndex(int mapNodeIndex)
{
    if (this->mapNodeIndex != mapNodeIndex)
    {
        this->mapNodeIndex = mapNodeIndex;
        emit mapNodeIndexChanged();
    }
}

int CampaignMap::getSettlementIndex() const
{
    return this->settlementIndex;
}

void CampaignMap::setSettlementIndex(int settlementIndex)
{
    if (this->settlementIndex != settlementIndex)
    {
        this->settlementIndex = settlementIndex;
        emit settlementIndexChanged();
    }
}

int CampaignMap::getUnitIndex() const
{
    return this->unitIndex;
}

void CampaignMap::setUnitIndex(int unitIndex)
{
    if (this->unitIndex != unitIndex)
    {
        this->unitIndex = unitIndex;
        emit unitIndexChanged();
    }
}

int CampaignMap::getArmyIndex() const
{
    return this->armyIndex;
}

void CampaignMap::setArmyIndex(int armyIndex)
{
    if (this->armyIndex != armyIndex)
    {
        this->armyIndex = armyIndex;
        emit armyIndexChanged();
    }
}

std::vector<MapNode *> CampaignMap::getMapNodes() const
{
    return this->mapNodes;
}

void CampaignMap::setMapNodes(const std::vector<MapNode *> &mapNodes)
{
    if (this->mapNodes != mapNodes)
    {
        this->mapNodes = mapNodes;
        emit mapNodesChanged();
    }
}

QVariantList CampaignMap::readMapNodes() const
{
    return toQVariantList(this->mapNodes);
}

std::vector<Settlement *> CampaignMap::getSettlements() const
{
    return this->settlements;
}

void CampaignMap::setSettlements(const std::vector<Settlement *> &settlements)
{
    if (this->settlements != settlements)
    {
        this->settlements = settlements;
        emit settlementsChanged();
    }
}

QVariantList CampaignMap::readSettlements() const
{
    return toQVariantList(this->settlements);
}

std::vector<Unit *> CampaignMap::getUnits() const
{
    return this->units;
}

void CampaignMap::setUnits(const std::vector<Unit *> &units)
{
    if (this->units != units)
    {
        this->units = units;
        emit unitsChanged();
    }
}

QVariantList CampaignMap::readUnits() const
{
    return toQVariantList(this->units);
}

std::vector<Faction *> CampaignMap::getFactions() const
{
    return this->factions;
}

void CampaignMap::setFactions(const std::vector<Faction *> &factions)
{
    if (this->factions != factions)
    {
        this->factions = factions;
        emit factionsChanged();
    }
}

QVariantList CampaignMap::readFactions() const
{
    return toQVariantList(this->factions);
}

void CampaignMap::addMapNode(MapNode *mapNode)
{
    mapNode->setParent(this);
    this->mapNodes.push_back(mapNode);

    emit mapNodesChanged();
}

void CampaignMap::removeMapNode(MapNode *mapNode)
{
    auto it = std::find(this->mapNodes.cbegin(), this->mapNodes.cend(), mapNode);
    if (it != this->mapNodes.end())
    {
        this->mapNodes.erase(it);
        mapNode->setParent(nullptr);

        emit mapNodesChanged();
    }
}

void CampaignMap::addSettlement(Settlement *settlement)
{
    settlement->setParent(this);
    this->settlements.push_back(settlement);

    emit settlementsChanged();
}

void CampaignMap::removeSettlement(Settlement *settlement)
{
    auto it = std::find(this->settlements.cbegin(), this->settlements.cend(), settlement);
    if (it != this->settlements.end())
    {
        this->settlements.erase(it);
        settlement->setParent(nullptr);

        emit settlementsChanged();
    }
}

void CampaignMap::addUnit(Unit *unit)
{
    unit->setParent(this);
    this->units.push_back(unit);

    emit unitsChanged();
}

void CampaignMap::removeUnit(Unit *unit)
{
    auto it = std::find(this->units.cbegin(), this->units.cend(), unit);
    if (it != this->units.end())
    {
        this->units.erase(it);
        unit->setParent(nullptr);

        emit unitsChanged();
    }
}

} // namespace core
} // namespace warmonger
