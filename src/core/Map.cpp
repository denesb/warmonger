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
    description(),
    world(nullptr),
    maxWidth(0),
    maxHeight(0),
    mapNodes(),
    players(),
    units(),
    settlements()
{
}

Map::~Map()
{
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
    this->world = world;
}

int Map::getMaxWidth() const
{
    return this->maxWidth;
}

void Map::setMaxWidth(int maxWidth)
{
    this->maxWidth = maxWidth;
}

int Map::getMaxHeight() const
{
    return this->maxHeight;
}

void Map::setMaxHeight(int maxHeight)
{
    this->maxHeight = maxHeight;
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
    this->maxWidth = obj["maxWidth"].toInt();
    this->maxHeight = obj["maxHeight"].toInt();
    this->mapNodes = newListFromJson<MapNode>(obj["mapNodes"].toArray(), this);
    this->players = newListFromJson<Player>(obj["players"].toArray(), this);
    this->units = newListFromJson<Unit>(obj["units"].toArray(), this);
    this->settlements = newListFromJson<Settlement>(obj["settlements"].toArray(), this);
}

void Map::dataToJson(QJsonObject &obj) const
{
    obj["description"] = this->description;
    obj["world"] = this->world->objectName();
    obj["maxWidth"] = this->maxWidth;
    obj["maxHeight"] = this->maxHeight;
    obj["mapNodes"] = listToJson<MapNode>(this->mapNodes);
    obj["players"] = listToJson<Player>(this->players);
    obj["units"] = listToJson<Unit>(this->units);
    obj["settlements"] = listToJson<Settlement>(this->settlements);
}
