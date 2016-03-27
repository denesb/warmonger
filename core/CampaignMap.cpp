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

namespace warmonger {
namespace core {

CampaignMap::CampaignMap(QObject *parent) :
    QObject(parent),
    world(nullptr),
    mapNodeIndex(0),
    settlementIndex(0),
    unitIndex(0),
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
        emit settlementIndexChanged();
        this->settlementIndex = settlementIndex;
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
        emit unitIndexChanged();
        this->unitIndex = unitIndex;
    }
}

void CampaignMap::addMapNode(MapNode *mapNode)
{
    mapNode->setParent(this);
    this->mapNodes.push_back(mapNode);

    emit mapNodeAdded(mapNode);
}

void CampaignMap::removeMapNode(MapNode *mapNode)
{
    mapNode->setParent(nullptr);

    auto it = std::find(this->mapNodes.cbegin(), this->mapNodes.cend(), mapNode);
    if (it != this->mapNodes.end())
    {
        this->mapNodes.erase(it);
        emit mapNodeRemoved(mapNode);
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
        emit mapNodesAboutToChange();
        this->mapNodes = mapNodes;
        emit mapNodesChanged();
    }
}

QVariantList CampaignMap::readMapNodes() const
{
    return toQVariantList(this->mapNodes);
}

void CampaignMap::addSettlement(Settlement *settlement)
{
    settlement->setParent(this);
    this->settlements.push_back(settlement);

    emit settlementAdded(settlement);
}

void CampaignMap::removeSettlement(Settlement *settlement)
{
    settlement->setParent(nullptr);

    auto it = std::find(this->settlements.cbegin(), this->settlements.cend(), settlement);
    if (it != this->settlements.end())
    {
        this->settlements.erase(it);
    }

    emit settlementRemoved(settlement);
}

std::vector<Settlement *> CampaignMap::getSettlements() const
{
    return this->settlements;
}

void CampaignMap::setSettlements(const std::vector<Settlement *> &settlements)
{
    if (this->settlements != settlements)
    {
        emit settlementsAboutToChange();
        this->settlements = settlements;
        emit settlementsChanged();
    }
}

QVariantList CampaignMap::readSettlements() const
{
    return toQVariantList(this->settlements);
}

void CampaignMap::addUnit(Unit *unit)
{
    unit->setParent(this);
    this->units.push_back(unit);

    wInfo(loggerName) << "Added unit " << unit;

    emit unitAdded(unit);
}

void CampaignMap::removeUnit(Unit *unit)
{
    unit->setParent(nullptr);

    auto it = std::find(this->units.cbegin(), this->units.cend(), unit);
    if (it != this->units.end())
    {
        this->units.erase(it);
    }

    emit unitRemoved(unit);
}

std::vector<Unit *> CampaignMap::getUnits() const
{
    return this->units;
}

void CampaignMap::setUnits(const std::vector<Unit *> &units)
{
    if (this->units != units)
    {
        emit unitsAboutToChange();
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

void CampaignMap::createMapNode(
    TerrainType *terrainType,
    const std::map<Direction, MapNode *> &neighbours
)
{
    if (neighbours.empty())
    {
        wError(loggerName) << "neighbours is empty";
        throw MapEditingError("Cannot create map-node without neighbours");
    }

    MapNode *newMapNode = new MapNode(this);
    newMapNode->setObjectName(mapNodeNameTemplate.arg(++this->mapNodeIndex));
    newMapNode->setTerrainType(terrainType);
    newMapNode->setNeighbours(neighbours);

    this->addMapNode(newMapNode);
}

void CampaignMap::createSettlement(
    SettlementType *settlementType,
    MapNode *mapNode,
    Faction *owner
)
{
    Settlement * newSettlement = new Settlement(this);
    newSettlement->setObjectName(
        settlementNameTemplate.arg(++this->settlementIndex)
    );
    newSettlement->setType(settlementType);
    newSettlement->setMapNode(mapNode);
    newSettlement->setOwner(owner);

    this->addSettlement(newSettlement);
}

Unit * CampaignMap::createUnit(UnitType *unitType, MapNode *mapNode, Faction *owner)
{
    Unit * newUnit = new Unit(this);
    newUnit->setObjectName(
        unitNameTemplate.arg(++this->unitIndex)
    );
    newUnit->setType(unitType);
    newUnit->setMapNode(mapNode);
    newUnit->setHitPoints(unitType->getHitPoints());
    newUnit->setMovementPoints(unitType->getClass()->getMovementPoints());
    newUnit->setOwner(owner);

    wInfo(loggerName) << "Created unit " << newUnit;

    this->addUnit(newUnit);

    return newUnit;
}

} // namespace core
} // namespace warmonger
