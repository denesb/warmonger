#include <QSet>

#include "core/Map.h"
#include "core/World.h"
#include "core/Player.h"
#include "core/Settlement.h"
#include "core/TerrainType.h"
#include "core/Unit.h"
#include "core/JsonUtil.hpp"
#include "core/Loader.hpp"
#include "core/Util.h"
#include "core/Exception.h"

using namespace warmonger::core;

const QString Map::DefinitionFile{"map.json"};
const QString Map::mapNodeNameTemplate{"mapNode%1"};

Map::Map(QObject *parent) :
    GameObject(parent),
    path(),
    description(),
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

QString Map::getPath() const
{
    return this->path;
}

void Map::setPath(const QString &path)
{
    this->path = path;
}

QString Map::getDescription()const
{
    return this->description;
}

void Map::setDescription(const QString &description)
{
    this->description = description;
}

const World * Map::getWorld() const
{
    return this->world;
}

void Map::setWorld(const World *world)
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

QList<const MapNode *> Map::getMapNodes() const
{
    return listConstClone(this->mapNodes);
}

QList<MapNode *> Map::getMapNodes()
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

QList<const Player *> Map::getPlayers() const
{
    return listConstClone(this->players);
}

QList<Player *> Map::getPlayers()
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

QList<const Unit *> Map::getUnits() const
{
    return listConstClone(this->units);
}

QList<Unit *> Map::getUnits()
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

QList<const Settlement *> Map::getSettlements() const
{
    return listConstClone(this->settlements);
}

QList<Settlement *> Map::getSettlements()
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

void Map::createMapNode(const TerrainType *terrainType, const QHash<MapNode::Direction, MapNode *> &neighbours)
{
    if (neighbours.empty())
    {
        Exception e(Exception::InvalidValue, {"empty QMap", "neighbours"});
        wError("core.Map") << e.getMessage();
        throw e;
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
        Exception e(Exception::NullPointer, {"terrainType"});
        wError("core.Map") << e.getMessage();
        throw e;
    }

    if (!neighbours.canConvert(QMetaType::QVariantMap))
    {
        Exception e(Exception::WrongType, {"neighbours", "QVariantMap", "?"});
        wError("core.Map") << e.getMessage();
        throw e;
    }
    QVariantMap neighboursMap = neighbours.toMap();
    QHash<MapNode::Direction, MapNode *> nm;

    QVariantMap::ConstIterator it;
    for (it = neighboursMap.constBegin(); it != neighboursMap.constEnd(); it++)
    {
        QString directionName = it.key();
        if (!MapNode::str2direction.contains(directionName))
        {
            Exception e(Exception::InvalidValue, {directionName, "MapNode::Direction"});
            wError("core.Map") << e.getMessage();
            throw e;
        }
        MapNode::Direction direction = MapNode::str2direction[directionName];
        
        QVariant v = it.value();
        if (!v.canConvert<MapNode *>())
        {
            Exception e(Exception::WrongType, {"mapNode", "MapNode *", "?"});
            wError("core.Map") << e.getMessage();
            throw e;
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
        Exception e(Exception::NullPointer, {"mapNode"});
        wError("core.Map") << e.getMessage();
        throw e;
    }

    TerrainType *tt = qobject_cast<TerrainType *>(newTerrainType);
    if (tt == nullptr)
    {
        Exception e(Exception::NullPointer, {"terrainType"});
        wError("core.Map") << e.getMessage();
        throw e;
    }

    mn->setTerrainType(tt);
}

void Map::dataFromJson(const QJsonObject &obj)
{
    this->description = obj["description"].toString();

    const QString worldName(obj["world"].toString());

    Loader<World> worldLoader(this);
    QStringList worldSearchPath;
    worldSearchPath << "worlds";
    worldLoader.setSearchPath(worldSearchPath);

    World *world = worldLoader.load(worldName);
    if (world == nullptr)
    {
        Exception e(Exception::ResourceLoadFailed, {"World", worldName});
        wError("core.Map") << e.getMessage();
        throw e;
    }

    this->world = world;
    this->mapNodeIndex = obj["mapNodeIndex"].toInt();
    this->mapNodes = this->mapNodesFromJson(obj["mapNodes"].toObject());
    this->players = newListFromJson<Player>(obj["players"].toArray(), this);
    this->units = newListFromJson<Unit>(obj["units"].toArray(), this);
    this->settlements = newListFromJson<Settlement>(obj["settlements"].toArray(), this);
}

void Map::dataToJson(QJsonObject &obj) const
{
    obj["description"] = this->description;
    obj["world"] = this->world->objectName();
    obj["mapNodeIndex"] = QString::number(this->mapNodeIndex);
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

        MapNode *nodeA = resolveReference<MapNode>(nodeNameA, this, "core.MapNode");
        MapNode *nodeB = resolveReference<MapNode>(nodeNameB, this, "core.MapNode");

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
        QHash<MapNode::Direction, const MapNode *> neighbours = mapNodeA->getNeighbours();
        QHash<MapNode::Direction, const MapNode *>::ConstIterator it;
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
