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

QVariantList CampaignMap::readMapNodes() const
{
    return utils::toQVariantList(this->mapNodes);
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

QVariantList CampaignMap::readSettlements() const
{
    return utils::toQVariantList(this->settlements);
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

    this->contents.push_back(std::tuple<MapNode*, Settlement*, Army*>(mapNode, nullptr, nullptr));

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

    emit settlementsChanged();

    QObject::connect(settlement, &Settlement::mapNodeChanged, this, &CampaignMap::settlementMapNodeChanged);

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

    emit armiesChanged();

    return army;
}

std::unique_ptr<Army> CampaignMap::removeArmy(Army* army)
{
    auto it = std::find(this->armies.cbegin(), this->armies.cend(), army);
    if (it != this->armies.end())
    {
        this->armies.erase(it);
        army->setParent(nullptr);

        emit armiesChanged();

        QObject::disconnect(army, nullptr, this, nullptr);

        return std::unique_ptr<Army>(army);
    }
    else
    {
        return std::unique_ptr<Army>();
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

        const auto it = std::find_if(this->armies.begin(),
            this->armies.end(),
            [&](const Army* army) { return army->getMapNode() == mapNode; });

        Army* army = (it == this->armies.end()) ? nullptr : *it;

        this->contents.emplace_back(mapNode, settlement, army);
    }
}

} // namespace core
} // namespace warmonger
