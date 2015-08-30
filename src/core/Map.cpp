#include <QSet>

#include "core/Map.h"
#include "core/World.h"
#include "core/Player.h"
#include "core/Settlement.h"
#include "core/TerrainType.h"
#include "core/UnitType.h"
#include "core/UnitClass.h"
#include "core/Unit.h"
#include "core/JsonUtil.h"

using namespace warmonger::core;

static const QString category{"core"};
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
    players()
{
}

Map::~Map()
{
}

QString Map::specification(const QString &objectName) const
{
    return "maps:" + objectName + ".wmd";
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

QObject * Map::readWorld() const
{
    return this->world;
}

void Map::writeWorld(QObject *world)
{
    World *w = qobject_cast<World *>(world);
    if (w == nullptr)
    {
        wError("core") << "world is null or has wrong type";
        throw Exception(Exception::InvalidValue);
    }

    this->setWorld(w);
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
        this->mapNodes = mapNodes;
        emit mapNodesChanged();
    }
}

QVariantList Map::readMapNodes() const
{
    return toQVariantList<MapNode>(this->mapNodes);
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
        this->settlements = settlements;
        emit settlementsChanged();
    }
}

QVariantList Map::readSettlements() const
{
    return toQVariantList<Settlement>(this->settlements);
}

void Map::addUnit(Unit *unit)
{
    unit->setParent(this);
    this->units << unit;

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
        this->units = units;
        emit unitsChanged();
    }
}

QVariantList Map::readUnits() const
{
    return toQVariantList<Unit>(this->units);
}

QList<Player *> Map::getPlayers() const
{
    return this->players;
}

void Map::setPlayers(const QList<Player *> &players)
{
    this->players = players;
}

QVariantList Map::readPlayers() const
{
    return toQVariantList<Player>(this->players);
}

void Map::createMapNode(TerrainType *terrainType, const QHash<MapNode::Direction, MapNode *> &neighbours)
{
    if (neighbours.empty())
    {
        wError(category) << "neighbours is empty";
        throw Exception(Exception::InvalidValue);
    }

    MapNode *newMapNode = new MapNode(this);
    newMapNode->setObjectName(Map::mapNodeNameTemplate.arg(++this->mapNodeIndex));
    newMapNode->setTerrainType(terrainType);
    newMapNode->setNeighbours(neighbours);

    this->addMapNode(newMapNode);
}

void Map::createMapNode(QObject *terrainType, QVariantMap neighbours)
{
    if (neighbours.empty())
    {
        wError(category) << "neighbours is empty";
        throw Exception(Exception::InvalidValue);
    }

    std::unique_ptr<MapNode> newMapNode(new MapNode(nullptr));
    newMapNode->setObjectName(
        Map::mapNodeNameTemplate.arg(++this->mapNodeIndex)
    );
    newMapNode->writeTerrainType(terrainType);
    newMapNode->writeNeighbours(neighbours);

    this->addMapNode(newMapNode.release());
}

void Map::createSettlement(SettlementType *settlementType, MapNode *mapNode)
{
    Settlement * newSettlement = new Settlement(this);
    newSettlement->setObjectName(
        Map::settlementNameTemplate.arg(++this->settlementIndex)
    );
    newSettlement->setSettlementType(settlementType);
    newSettlement->setMapNode(mapNode);

    this->addSettlement(newSettlement);
}

Q_INVOKABLE void Map::createSettlement(QObject *settlementType, QObject *mapNode)
{
    std::unique_ptr<Settlement> newSettlement(new Settlement(nullptr));
    newSettlement->setObjectName(
        Map::settlementNameTemplate.arg(++this->settlementIndex)
    );
    newSettlement->writeSettlementType(settlementType);
    newSettlement->writeMapNode(mapNode);

    this->addSettlement(newSettlement.release());
}

void Map::createUnit(UnitType *unitType, MapNode *mapNode)
{
    Unit * newUnit = new Unit(this);
    newUnit->setObjectName(
        Map::unitNameTemplate.arg(++this->unitIndex)
    );
    newUnit->setUnitType(unitType);
    newUnit->setMapNode(mapNode);
    newUnit->setHitPoints(unitType->getHitPoints());
    newUnit->setMovementPoints(unitType->getUnitClass()->getMovementPoints());

    this->addUnit(newUnit);
}

Q_INVOKABLE void Map::createUnit(QObject *unitType, QObject *mapNode)
{
    std::unique_ptr<Unit> newUnit(new Unit(nullptr));
    newUnit->setObjectName(
        Map::unitNameTemplate.arg(++this->unitIndex)
    );
    newUnit->writeUnitType(unitType);
    newUnit->writeMapNode(mapNode);
    UnitType *ut = newUnit->getUnitType();
    UnitClass *uc = ut->getUnitClass();
    newUnit->setHitPoints(ut->getHitPoints());
    newUnit->setMovementPoints(uc->getMovementPoints());

    this->addUnit(newUnit.release());
}

void Map::dataFromJson(const QJsonObject &obj)
{
    const QString worldName(obj["world"].toString());
    World *world = new World(this);
    world->load(world->specification(worldName));

    this->world = world;
    this->mapNodeIndex = obj["mapNodeIndex"].toInt();
    this->settlementIndex = obj["settlementIndex"].toInt();
    this->unitIndex = obj["unitIndex"].toInt();
    this->mapNodes = this->mapNodesFromJson(obj["mapNodes"].toObject());
    this->players = newListFromJson<Player>(obj["players"].toArray(), this);
    this->units = newListFromJson<Unit>(obj["units"].toArray(), this);
    this->settlements = newListFromJson<Settlement>(obj["settlements"].toArray(), this);
}

void Map::dataToJson(QJsonObject &obj) const
{
    obj["world"] = this->world->objectName();
    obj["mapNodeIndex"] = this->mapNodeIndex;
    obj["settlementIndex"] = this->settlementIndex;
    obj["unitIndex"] = this->unitIndex;
    obj["mapNodes"] = this->mapNodesToJson(this->mapNodes);
    obj["players"] = listToJson<Player>(this->players);
    obj["units"] = listToJson<Unit>(this->units);
    obj["settlements"] = listToJson<Settlement>(this->settlements);
}

QList<MapNode *> Map::mapNodesFromJson(const QJsonObject &obj)
{
    QList<MapNode *> mapNodes;
    mapNodes = newListFromJson<MapNode>(obj["nodes"].toArray(), this);

    QJsonArray connections = obj["connections"].toArray();
    for (const QJsonValue connection : connections)
    {
        const QJsonObject connectionObj = connection.toObject();
        const QString nodeNameA = connectionObj["nodeA"].toString();
        const QString nodeNameB = connectionObj["nodeB"].toString();
        const MapNode::Direction nodeDirectionA =
            MapNode::str2direction[connectionObj["directionA"].toString()];

        MapNode *nodeA = resolveReference<MapNode>(nodeNameA, this);
        MapNode *nodeB = resolveReference<MapNode>(nodeNameB, this);

        nodeA->setNeighbour(nodeDirectionA, nodeB);
    }

    return std::move(mapNodes);
}

QJsonObject Map::mapNodesToJson(const QList<MapNode *> &mapNodes) const
{
    static const QString connectionStrTemplate("%1_%2_%3");

    QJsonObject obj;
    obj["nodes"] = listToJson<MapNode>(this->mapNodes);

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
