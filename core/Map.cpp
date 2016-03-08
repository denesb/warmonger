#include <QMetaMethod>
#include <QSet>

#include "core/Map.h"
#include "core/QVariantUtil.h"
#include "core/TerrainType.h"
#include "core/WorldSurface.h"
#include "core/UnitClass.h"

using namespace warmonger::core;

static const QString loggerName{"core.Map"};

const QString Map::mapNodeNameTemplate{"mapNode%1"};
const QString Map::settlementNameTemplate{"settlement%1"};
const QString Map::unitNameTemplate{"unit%1"};

Map::Map(QObject *parent) :
    QObject(parent),
    world(nullptr),
    mapNodeIndex(0),
    settlementIndex(0),
    unitIndex(0),
    mapNodes(),
    players(),
    settlements(),
    units(),
    mapContent()
{
}

World * Map::getWorld() const
{
    return this->world;
}

QString Map::getDisplayName() const
{
    return this->displayName;
}

void Map::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

void Map::setWorld(World *world)
{
    if (this->world != world)
    {
        this->world = world;
        emit worldChanged();
    }
}

int Map::getMapNodeIndex() const
{
    return this->mapNodeIndex;
}

void Map::setMapNodeIndex(int mapNodeIndex)
{
    if (this->mapNodeIndex != mapNodeIndex)
    {
        this->mapNodeIndex = mapNodeIndex;
        emit mapNodeIndexChanged();
    }
}

int Map::getSettlementIndex() const
{
    return this->settlementIndex;
}

void Map::setSettlementIndex(int settlementIndex)
{
    if (this->settlementIndex != settlementIndex)
    {
        emit settlementIndexChanged();
        this->settlementIndex = settlementIndex;
    }
}

int Map::getUnitIndex() const
{
    return this->unitIndex;
}

void Map::setUnitIndex(int unitIndex)
{
    if (this->unitIndex != unitIndex)
    {
        emit unitIndexChanged();
        this->unitIndex = unitIndex;
    }
}

void Map::addMapNode(MapNode *mapNode)
{
    mapNode->setParent(this);
    this->mapNodes << mapNode;

    emit mapNodeAdded(mapNode);
}

void Map::removeMapNode(MapNode *mapNode)
{
    mapNode->setParent(nullptr);
    this->mapNodes.removeOne(mapNode);

    emit mapNodeRemoved(mapNode);
}

QList<MapNode *> Map::getMapNodes() const
{
    return this->mapNodes;
}

void Map::setMapNodes(const QList<MapNode *> &mapNodes)
{
    if (this->mapNodes != mapNodes)
    {
        emit mapNodesAboutToChange();
        this->mapNodes = mapNodes;
        emit mapNodesChanged();
    }
}

QVariantList Map::readMapNodes() const
{
    return toQVariantList(this->mapNodes);
}

void Map::addSettlement(Settlement *settlement)
{
    settlement->setParent(this);
    this->settlements << settlement;

    emit settlementAdded(settlement);
}

void Map::removeSettlement(Settlement *settlement)
{
    settlement->setParent(nullptr);
    this->settlements.removeOne(settlement);

    emit settlementRemoved(settlement);
}

QList<Settlement *> Map::getSettlements() const
{
    return this->settlements;
}

void Map::setSettlements(const QList<Settlement *> &settlements)
{
    if (this->settlements != settlements)
    {
        emit settlementsAboutToChange();
        this->settlements = settlements;
        emit settlementsChanged();
    }
}

QVariantList Map::readSettlements() const
{
    return toQVariantList(this->settlements);
}

void Map::addUnit(Unit *unit)
{
    unit->setParent(this);
    this->units << unit;

    wInfo(loggerName) << "Added unit " << unit;

    emit unitAdded(unit);
}

void Map::removeUnit(Unit *unit)
{
    unit->setParent(nullptr);
    this->units.removeOne(unit);

    emit unitRemoved(unit);
}

QList<Unit *> Map::getUnits() const
{
    return this->units;
}

void Map::setUnits(const QList<Unit *> &units)
{
    if (this->units != units)
    {
        emit unitsAboutToChange();
        this->units = units;
        emit unitsChanged();
    }
}

QVariantList Map::readUnits() const
{
    return toQVariantList(this->units);
}

QList<Player *> Map::getPlayers() const
{
    return this->players;
}

void Map::setPlayers(const QList<Player *> &players)
{
    if (this->players != players)
    {
        this->players = players;
        emit playersChanged();
    }
}

QVariantList Map::readPlayers() const
{
    return toQVariantList(this->players);
}

void Map::createMapNode(
    TerrainType *terrainType,
    const QHash<MapNode::Direction, MapNode *> &neighbours
)
{
    if (neighbours.empty())
    {
        wError(loggerName) << "neighbours is empty";
        throw MapEditingError("Cannot create map-node without neighbours");
    }

    MapNode *newMapNode = new MapNode(this);
    newMapNode->setObjectName(Map::mapNodeNameTemplate.arg(++this->mapNodeIndex));
    newMapNode->setTerrainType(terrainType);
    newMapNode->setNeighbours(neighbours);

    this->addMapNode(newMapNode);
}

void Map::createSettlement(
    SettlementType *settlementType,
    MapNode *mapNode,
    Player *owner
)
{
    Settlement * newSettlement = new Settlement(this);
    newSettlement->setObjectName(
        Map::settlementNameTemplate.arg(++this->settlementIndex)
    );
    newSettlement->setType(settlementType);
    newSettlement->setMapNode(mapNode);
    newSettlement->setOwner(owner);

    this->addSettlement(newSettlement);
}

Unit * Map::createUnit(UnitType *unitType, MapNode *mapNode, Player *owner)
{
    Unit * newUnit = new Unit(this);
    newUnit->setObjectName(
        Map::unitNameTemplate.arg(++this->unitIndex)
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

Settlement * Map::getSettlementOn(const MapNode *mapNode) const
{
    return this->mapContent[mapNode].first;
}

Unit * Map::getUnitOn(const MapNode *mapNode) const
{
    return this->mapContent[mapNode].second;
}

bool Map::hasSettlement(const MapNode *mapNode) const
{
    QPair<Settlement *, Unit *> content = this->mapContent[mapNode];
    return content.first != nullptr;
}

bool Map::hasUnit(const MapNode *mapNode) const
{
    QPair<Settlement *, Unit *> content = this->mapContent[mapNode];
    return content.second != nullptr;
}

void Map::onMapNodesAboutToChange()
{
    this->mapContent.clear();
}

void Map::onMapNodesChanged()
{
    std::for_each(
        this->mapNodes.constBegin(),
        this->mapNodes.constEnd(),
        std::bind(&Map::onMapNodeAdded, this, std::placeholders::_1)
    );
}

void Map::onMapNodeAdded(MapNode *n)
{
    this->mapContent.insert(
        n,
        QPair<Settlement *, Unit *>(nullptr, nullptr)
    );
}

void Map::onMapNodeRemoved(MapNode *n)
{
    this->mapContent.remove(n);
}

void Map::onSettlementsAboutToChange()
{
    std::for_each(
        this->settlements.constBegin(),
        this->settlements.constEnd(),
        std::bind(&Map::onSettlementRemoved, this, std::placeholders::_1)
    );
}

void Map::onSettlementsChanged()
{
    std::for_each(
        this->settlements.constBegin(),
        this->settlements.constEnd(),
        std::bind(&Map::onSettlementAdded, this, std::placeholders::_1)
    );
}

void Map::onSettlementAdded(Settlement *s)
{
    QObject::connect(
        s,
        &Settlement::mapNodeChanged,
        this,
        &Map::updateContent
    );

    if (s->getMapNode() != nullptr)
    {
        this->mapContent[s->getMapNode()].first = s;
    }
}

void Map::onSettlementRemoved(Settlement *s)
{
    QObject::disconnect(s, QMetaMethod(), this, QMetaMethod());

    if (s->getMapNode() != nullptr)
        this->mapContent[s->getMapNode()].first = nullptr;
}

void Map::onUnitsAboutToChange()
{
    std::for_each(
        this->units.constBegin(),
        this->units.constEnd(),
        std::bind(&Map::onUnitRemoved, this, std::placeholders::_1)
    );
}

void Map::onUnitsChanged()
{
    std::for_each(
        this->units.constBegin(),
        this->units.constEnd(),
        std::bind(&Map::onUnitAdded, this, std::placeholders::_1)
    );
}

void Map::onUnitAdded(Unit *u)
{
    QObject::connect(
        u,
        &Unit::mapNodeChanged,
        this,
        &Map::updateContent
    );

    if (u->getMapNode() != nullptr)
        this->mapContent[u->getMapNode()].second = u;
}

void Map::onUnitRemoved(Unit *u)
{
    QObject::disconnect(u, QMetaMethod(), this, QMetaMethod());

    if (u->getMapNode() != nullptr)
        this->mapContent[u->getMapNode()].second = nullptr;
}

void Map::setupContent()
{
    this->onMapNodesChanged();

    std::for_each(
        this->settlements.constBegin(),
        this->settlements.constEnd(),
        std::bind(&Map::onSettlementAdded, this, std::placeholders::_1)
    );
    std::for_each(
        this->units.constBegin(),
        this->units.constEnd(),
        std::bind(&Map::onUnitAdded, this, std::placeholders::_1)
    );

    QObject::connect(
        this,
        &Map::unitAdded,
        this,
        &Map::onUnitAdded
    );
}

void Map::updateContent()
{
    auto begin = this->mapContent.begin();
    auto end = this->mapContent.end();
    for (auto it = begin; it != end; it++)
    {
        QPair<Settlement *, Unit *> &p = *it;
        Settlement *s = p.first;
        Unit *u = p.second;
        if (s != nullptr && s->getMapNode() != it.key())
        {
            p.first = nullptr;
            this->mapContent[s->getMapNode()].first = s;
        }

        if (u != nullptr && u->getMapNode() != it.key())
        {
            p.second = nullptr;
            this->mapContent[u->getMapNode()].second = u;
        }
    }
}
