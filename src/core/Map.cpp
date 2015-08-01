#include "core/Map.h"
#include "core/World.h"
#include "core/MapNode.h"
#include "core/Player.h"
#include "core/Unit.h"
#include "core/Settlement.h"
#include "core/JsonUtil.hpp"
#include "core/Loader.hpp"
#include "core/Util.h"
#include "core/Exception.h"

using namespace warmonger::core;

const QString Map::DefinitionFile = "map.json";

Map::Map(QObject *parent) :
    GameObject(parent),
    path(),
    description(),
    world(nullptr),
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
    this->mapNodes = mapNodes;
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
        wError("core.Map") << "Failed to load <World>" << worldName;
        throw Exception(Exception::ResourceLoadFailed, {"World", worldName});
    }

    this->world = world;
    this->mapNodes = this->mapNodesFromJson(obj["mapNodes"].toObject());
    this->players = newListFromJson<Player>(obj["players"].toArray(), this);
    this->units = newListFromJson<Unit>(obj["units"].toArray(), this);
    this->settlements = newListFromJson<Settlement>(obj["settlements"].toArray(), this);
}

void Map::dataToJson(QJsonObject &obj) const
{
    obj["description"] = this->description;
    obj["world"] = this->world->objectName();
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
    QJsonObject obj;
    obj["nodes"] = listToJson<MapNode>(this->mapNodes);

    return std::move(obj);
}
