#include <algorithm>
#include <set>

#include <QMetaMethod>

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
    players(),
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
    this->settlements << settlement;

    emit settlementAdded(settlement);
}

void CampaignMap::removeSettlement(Settlement *settlement)
{
    settlement->setParent(nullptr);
    this->settlements.removeOne(settlement);

    emit settlementRemoved(settlement);
}

QList<Settlement *> CampaignMap::getSettlements() const
{
    return this->settlements;
}

void CampaignMap::setSettlements(const QList<Settlement *> &settlements)
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
    this->units << unit;

    wInfo(loggerName) << "Added unit " << unit;

    emit unitAdded(unit);
}

void CampaignMap::removeUnit(Unit *unit)
{
    unit->setParent(nullptr);
    this->units.removeOne(unit);

    emit unitRemoved(unit);
}

QList<Unit *> CampaignMap::getUnits() const
{
    return this->units;
}

void CampaignMap::setUnits(const QList<Unit *> &units)
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

QList<Player *> CampaignMap::getPlayers() const
{
    return this->players;
}

void CampaignMap::setPlayers(const QList<Player *> &players)
{
    if (this->players != players)
    {
        this->players = players;
        emit playersChanged();
    }
}

QVariantList CampaignMap::readPlayers() const
{
    return toQVariantList(this->players);
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
    Player *owner
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

Unit * CampaignMap::createUnit(UnitType *unitType, MapNode *mapNode, Player *owner)
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