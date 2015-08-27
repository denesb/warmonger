#include <QSet>

#include "core/Map.h"
#include "core/World.h"
#include "core/Player.h"
#include "core/Settlement.h"
#include "core/TerrainType.h"
#include "core/Unit.h"
#include "core/JsonUtil.h"

using namespace warmonger::core;

static const QString category{"core"};
const QString Map::mapNodeNameTemplate{"mapNode%1"};

Map::Map(QObject *parent) :
    GameEntity(parent),
    world(nullptr),
    mapNodeIndex(0),
    mapNodes(),
    players(),
    units(),
    settlements()
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

QVariant Map::readWorld() const
{
    World *o = const_cast<World *>(this->world);
    return QVariant::fromValue<QObject *>(o);
}

void Map::writeWorld(QVariant world)
{
    if (!world.canConvert<World *>())
    {
        wError("core") << "world has wrong type";
        throw Exception(Exception::WrongType);
    }
    World *w = world.value<World *>();

    this->setWorld(w);
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

QVariant Map::readMapNodes() const
{
    return QVariant::fromValue(toQObjectList<MapNode>(this->mapNodes));
}

QList<Player *> Map::getPlayers() const
{
    return this->players;
}

void Map::setPlayers(const QList<Player *> &players)
{
    this->players = players;
}

QVariant Map::readPlayers() const
{
    return QVariant::fromValue(toQObjectList<Player>(this->players));
}

QList<Unit *> Map::getUnits() const
{
    return this->units;
}

void Map::setUnits(const QList<Unit *> &units)
{
    this->units = units;
}

QVariant Map::readUnits() const
{
    return QVariant::fromValue(toQObjectList<Unit>(this->units));
}

QList<Settlement *> Map::getSettlements() const
{
    return this->settlements;
}

void Map::setSettlements(const QList<Settlement *> &settlements)
{
    this->settlements = settlements;
}

QVariant Map::readSettlements() const
{
    return QVariant::fromValue(toQObjectList<Settlement>(this->settlements));
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

    QHash<MapNode::Direction, MapNode *>::ConstIterator it;
    for (it = neighbours.constBegin(); it != neighbours.constEnd(); it++)
    {
        MapNode *neighbour = it.value();
        MapNode::Direction direction = it.key();

        newMapNode->setNeighbour(direction, neighbour);
        neighbour->setNeighbour(MapNode::oppositeDirections[direction], newMapNode);
    }

    this->mapNodes << newMapNode;

    emit mapNodeCreated(newMapNode);
}

void Map::createMapNode(QObject *terrainType, QVariant neighbours)
{
    TerrainType *tt = qobject_cast<TerrainType *>(terrainType);
    if (tt == nullptr)
    {
        wError(category) << "terrainType is null or has wrong type";
        throw Exception(Exception::NullPointer);
    }

    if (!neighbours.canConvert(QMetaType::QVariantMap))
    {
        wError(category) << "neighbours has wrong type";
        throw Exception(Exception::WrongType);
    }
    QVariantMap neighboursMap = neighbours.toMap();
    QHash<MapNode::Direction, MapNode *> nm;

    QVariantMap::ConstIterator it;
    for (it = neighboursMap.constBegin(); it != neighboursMap.constEnd(); it++)
    {
        QString directionName = it.key();
        if (!MapNode::str2direction.contains(directionName))
        {
            wError(category) << "Uknown direction";
            throw Exception(Exception::InvalidValue);
        }
        MapNode::Direction direction = MapNode::str2direction[directionName];
        
        QVariant v = it.value();
        if (!v.canConvert<MapNode *>())
        {
            wError(category) << "mapNode has wrong type";
            throw Exception(Exception::WrongType);

        }

        MapNode *neighbour = v.value<MapNode *>();
        nm.insert(direction, neighbour);
    }

    this->createMapNode(tt, nm);
}

void Map::changeMapNodeTerrainType(QObject *mapNode, QObject *newTerrainType)
{
    MapNode *mn = qobject_cast<MapNode *>(mapNode);
    if (mn == nullptr)
    {
        wError(category) << "mapNode is null";
        throw Exception(Exception::NullPointer);
    }

    TerrainType *tt = qobject_cast<TerrainType *>(newTerrainType);
    if (tt == nullptr)
    {
        wError(category) << "terrainType is null";
        throw Exception(Exception::NullPointer);
    }

    mn->setTerrainType(tt);
}

void Map::dataFromJson(const QJsonObject &obj)
{
    const QString worldName(obj["world"].toString());
    World *world = new World(this);
    world->load(world->specification(worldName));

    this->world = world;
    this->mapNodeIndex = obj["mapNodeIndex"].toInt();
    this->mapNodes = this->mapNodesFromJson(obj["mapNodes"].toObject());
    this->players = newListFromJson<Player>(obj["players"].toArray(), this);
    this->units = newListFromJson<Unit>(obj["units"].toArray(), this);
    this->settlements = newListFromJson<Settlement>(obj["settlements"].toArray(), this);
}

void Map::dataToJson(QJsonObject &obj) const
{
    obj["world"] = this->world->objectName();
    obj["mapNodeIndex"] = this->mapNodeIndex;
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
        const MapNode::Direction nodeDirectionA = MapNode::str2direction[connectionObj["directionA"].toString()];
        const MapNode::Direction nodeDirectionB = MapNode::str2direction[connectionObj["directionB"].toString()];

        MapNode *nodeA = resolveReference<MapNode>(nodeNameA, this);
        MapNode *nodeB = resolveReference<MapNode>(nodeNameB, this);

        nodeA->setNeighbour(nodeDirectionA, nodeB);
        nodeB->setNeighbour(nodeDirectionB, nodeA);
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
