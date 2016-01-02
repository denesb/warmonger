#include <QMetaMethod>
#include <QSet>

#include "core/EntityManager.h"
#include "core/Map.h"
#include "core/QJsonUtil.h"
#include "core/QVariantUtil.h"
#include "core/TerrainType.h"
#include "core/WorldSurface.h"
#include "core/UnitClass.h"

using namespace warmonger::core;

static const QString loggerName{"core.Map"};

const QString Map::fileExtension{"wmd"};
const QString Map::mapNodeNameTemplate{"mapNode%1"};
const QString Map::settlementNameTemplate{"settlement%1"};
const QString Map::unitNameTemplate{"unit%1"};

Map::Map(QObject *parent) :
    GameEntity(parent),
    world(nullptr),
    mapNodeIndex(0),
    settlementIndex(0),
    unitIndex(0),
    mapNodes(),
    settlements(),
    units(),
    players(),
    neutralPlayer(new Player(this)),
    mapContent()
{
    this->neutralPlayer->setObjectName("neutral");
    this->neutralPlayer->setDisplayName("Neutral");
}

Map::~Map()
{
}

World * Map::getWorld() const
{
    return this->world;
}

void Map::setWorld(World *world)
{
    if (this->world != world)
    {
        this->world = world;
        emit worldChanged();
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

QVariantList Map::readAllPlayers() const
{
    QList<Player *> allPlayers;
    allPlayers << this->neutralPlayer;
    allPlayers << this->players;
    return toQVariantList(allPlayers);
}

Player * Map::getNeutralPlayer() const
{
    return this->neutralPlayer;
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

    if (owner == nullptr)
        owner = this->neutralPlayer;

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

    if (owner == nullptr)
        owner = this->neutralPlayer;

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

void Map::onSurfaceChanged()
{
    QColor c(this->world->getSurface()->getColor("neutral"));
    this->neutralPlayer->setColor(c);
}

void Map::dataFromJson(const QJsonObject &obj)
{
    const QString worldName(obj["world"].toString());
    World *world = EntityManager::getInstance()->getEntity<World>(worldName);
    QObject::connect(
        world,
        &World::surfaceChanged,
        this,
        &Map::onSurfaceChanged
    );

    this->world = world;
    this->mapNodeIndex = obj["mapNodeIndex"].toInt();
    this->settlementIndex = obj["settlementIndex"].toInt();
    this->unitIndex = obj["unitIndex"].toInt();
    this->mapNodes = this->mapNodesFromJson(obj["mapNodes"].toObject());
    this->players = fromQJsonArray<QList<Player *>>(
        obj["players"].toArray(),
        ObjectConstructor<Player>(this)
    );
    this->units = fromQJsonArray<QList<Unit *>>(
        obj["units"].toArray(),
        ObjectConstructor<Unit>(this)
    );
    this->settlements = fromQJsonArray<QList<Settlement *>>(
        obj["settlements"].toArray(),
        ObjectConstructor<Settlement>(this)
    );

    this->setupContent();
}

void Map::dataToJson(QJsonObject &obj) const
{
    obj["world"] = this->world->objectName();
    obj["mapNodeIndex"] = this->mapNodeIndex;
    obj["settlementIndex"] = this->settlementIndex;
    obj["unitIndex"] = this->unitIndex;
    obj["mapNodes"] = this->mapNodesToJson(this->mapNodes);
    obj["players"] = toQJsonArray(this->players, objectToQJsonObject<Player>);
    obj["units"] = toQJsonArray(this->units, objectToQJsonObject<Unit>);
    obj["settlements"] = toQJsonArray(
        this->settlements,
        objectToQJsonObject<Settlement>
    );
}

QList<MapNode *> Map::mapNodesFromJson(const QJsonObject &obj)
{
    QList<MapNode *> mapNodes;
    mapNodes = fromQJsonArray<QList<MapNode *>>(
        obj["nodes"].toArray(),
        ObjectConstructor<MapNode>(this)
    );

    QHash<QString, MapNode *> mapNodeLookup;
    for (MapNode *mapNode : mapNodes)
    {
        mapNodeLookup.insert(mapNode->objectName(), mapNode);
    }

    QJsonArray connections = obj["connections"].toArray();
    for (const QJsonValue connection : connections)
    {
        const QJsonObject connectionObj = connection.toObject();
        const QString nodeNameA = connectionObj["nodeA"].toString();
        const QString nodeNameB = connectionObj["nodeB"].toString();
        const MapNode::Direction nodeDirectionA =
            MapNode::str2direction[connectionObj["directionA"].toString()];

        if (!mapNodeLookup.contains(nodeNameA))
        {
            wError(loggerName) << "Node " << nodeNameA << " does not exists";
            throw UnresolvedReferenceError(nodeNameA);
        }

        if (!mapNodeLookup.contains(nodeNameB))
        {
            wError(loggerName) << "Node " << nodeNameB << " does not exists";
            throw UnresolvedReferenceError(nodeNameB);
        }

        MapNode *nodeA = mapNodeLookup[nodeNameA];
        MapNode *nodeB = mapNodeLookup[nodeNameB];

        nodeA->setNeighbour(nodeDirectionA, nodeB);
    }

    return std::move(mapNodes);
}

QJsonObject Map::mapNodesToJson(const QList<MapNode *> &mapNodes) const
{
    static const QString connectionStrTemplate("%1_%2_%3");

    QJsonObject obj;
    obj["nodes"] = toQJsonArray(this->mapNodes, objectToQJsonObject<MapNode>);

    QJsonArray connections;
    QSet<QString> connectionStrs;

    for (const MapNode *mapNodeA : mapNodes)
    {
        QHash<MapNode::Direction, MapNode *> neighbours = mapNodeA->getNeighbours();
        QHash<MapNode::Direction, MapNode *>::ConstIterator it;
        for (it = neighbours.constBegin(); it != neighbours.constEnd(); it++)
        {
            const MapNode *mapNodeB = it.value();

            if (mapNodeB == nullptr)
                continue;

            const QString directionA(MapNode::direction2str[it.key()]);
            const QString directionB(MapNode::direction2str[MapNode::oppositeDirections[it.key()]]);

            const QString connectionStrA = connectionStrTemplate
                .arg(mapNodeA->objectName())
                .arg(directionA)
                .arg(mapNodeB->objectName());

            const QString connectionStrB = connectionStrTemplate
                .arg(mapNodeB->objectName())
                .arg(directionB)
                .arg(mapNodeA->objectName());

            if (connectionStrs.contains(connectionStrA) || connectionStrs.contains(connectionStrB))
                continue;

            connectionStrs << connectionStrA;
            connectionStrs << connectionStrB;

            QJsonObject connection;
            connection["nodeA"] = mapNodeA->objectName();
            connection["nodeB"] = mapNodeB->objectName();
            connection["directionA"] = directionA;
            connection["directionB"] = directionB;

            connections.append(connection);
        }
    }

    obj["connections"] = connections;

    return std::move(obj);
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
