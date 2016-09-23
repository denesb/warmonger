#include <algorithm>

#include "core/CampaignMap.h"
#include "core/TerrainType.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

const QString mapNodeNameTemplate{"mapNode-%1"};
const QString settlementNameTemplate{"settlement-%1"};
const QString unitNameTemplate{"unit-%1"};
const QString armyNameTemplate{"army-%1"};

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

CampaignMap::CampaignMap(QObject* parent)
    : QObject(parent)
    , world(nullptr)
    , mapNodeIndex(0)
    , settlementIndex(0)
    , unitIndex(0)
    , armyIndex(0)
{
}

World* CampaignMap::getWorld() const
{
    return this->world;
}

QString CampaignMap::getDisplayName() const
{
    return this->displayName;
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

const std::vector<MapNode*>& CampaignMap::getMapNodes() const
{
    return this->mapNodes;
}

std::vector<MapNode*>& CampaignMap::getMapNodes()
{
    return this->mapNodes;
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

        emit mapNodesChanged();
    }
}

QVariantList CampaignMap::readMapNodes() const
{
    return utils::toQVariantList(this->mapNodes);
}

std::vector<Settlement*> CampaignMap::getSettlements() const
{
    return this->settlements;
}

void CampaignMap::setSettlements(const std::vector<Settlement*>& settlements)
{
    if (this->settlements != settlements)
    {
        std::for_each(this->settlements.begin(), this->settlements.end(), destroy);

        this->settlements = settlements;

        for (Settlement* settlement : this->settlements)
            settlement->setParent(this);

        this->settlementIndex = nextNameIndex(this->settlements);

        emit settlementsChanged();
    }
}

QVariantList CampaignMap::readSettlements() const
{
    return utils::toQVariantList(this->settlements);
}

std::vector<Unit*> CampaignMap::getUnits() const
{
    return this->units;
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

std::vector<Army*> CampaignMap::getArmies() const
{
    return this->armies;
}

void CampaignMap::setArmies(const std::vector<Army*>& armies)
{
    if (this->armies != armies)
    {
        std::for_each(this->armies.begin(), this->armies.end(), destroy);

        this->armies = armies;

        for (Army* army : this->armies)
            army->setParent(this);

        this->armyIndex = nextNameIndex(this->armies);

        emit armiesChanged();
    }
}

QVariantList CampaignMap::readArmies() const
{
    return utils::toQVariantList(this->armies);
}

std::vector<Faction*> CampaignMap::getFactions() const
{
    return this->factions;
}

void CampaignMap::setFactions(const std::vector<Faction*>& factions)
{
    if (this->factions != factions)
    {
        this->factions = factions;
        emit factionsChanged();
    }
}

QVariantList CampaignMap::readFactions() const
{
    return utils::toQVariantList(this->factions);
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

    emit mapNodesChanged();

    return mapNode;
}

void CampaignMap::removeMapNode(MapNode* mapNode)
{
    auto it = std::find(this->mapNodes.cbegin(), this->mapNodes.cend(), mapNode);
    if (it != this->mapNodes.end())
    {
        this->mapNodes.erase(it);
        mapNode->setParent(nullptr);

        emit mapNodesChanged();
    }
}

Settlement* CampaignMap::createSettlement(SettlementType* settlementType)
{
    Settlement* settlement = new Settlement(this);

    settlement->setObjectName(settlementNameTemplate.arg(this->settlementIndex++));
    settlement->setType(settlementType);

    this->settlements.push_back(settlement);

    emit settlementsChanged();

    return settlement;
}

void CampaignMap::removeSettlement(Settlement* settlement)
{
    auto it = std::find(this->settlements.cbegin(), this->settlements.cend(), settlement);
    if (it != this->settlements.end())
    {
        this->settlements.erase(it);
        settlement->setParent(nullptr);

        emit settlementsChanged();
    }
}

Unit* CampaignMap::createUnit(UnitType* unitType)
{
    Unit* unit = new Unit(this);

    unit->setObjectName(unitNameTemplate.arg(this->unitIndex++));

    unit->setType(unitType);

    this->units.push_back(unit);

    emit unitsChanged();

    return unit;
}

void CampaignMap::removeUnit(Unit* unit)
{
    auto it = std::find(this->units.cbegin(), this->units.cend(), unit);
    if (it != this->units.end())
    {
        this->units.erase(it);
        unit->setParent(nullptr);

        emit unitsChanged();
    }
}

Army* CampaignMap::createArmy(ArmyType* armyType)
{
    Army* army = new Army(this);

    army->setObjectName(armyNameTemplate.arg(this->armyIndex++));

    army->setType(armyType);

    this->armies.push_back(army);

    emit armiesChanged();

    return army;
}

void CampaignMap::removeArmy(Army* army)
{
    auto it = std::find(this->armies.cbegin(), this->armies.cend(), army);
    if (it != this->armies.end())
    {
        this->armies.erase(it);
        army->setParent(nullptr);

        emit armiesChanged();
    }
}

} // namespace core
} // namespace warmonger
