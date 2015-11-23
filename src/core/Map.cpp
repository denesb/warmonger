#include <QMetaMethod>
#include <QSet>

#include "core/Map.h"
#include "core/World.h"
#include "core/WorldSurface.h"
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

Map::Map() :
    GameEntity(),
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

QString Map::fileExtension() const
{
    return QString("wmd");
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
        emit mapNodesAboutToChange();
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
        emit settlementsAboutToChange();
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
        emit unitsAboutToChange();
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
    if (this->players != players)
    {
        this->players = players;
        emit playersChanged();
    }
}

QVariantList Map::readPlayers() const
{
    return toQVariantList<Player>(this->players);
}

QVariantList Map::readAllPlayers() const
{
    QList<Player *> allPlayers;
    allPlayers << this->neutralPlayer;
    allPlayers << this->players;
    return toQVariantList<Player>(allPlayers);
}

QObject * Map::readNeutralPlayer() const
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
        wError(category) << "neighbours is empty";
        throw Exception(Exception::InvalidValue);
    }

    MapNode *newMapNode = new MapNode(this);
    newMapNode->setObjectName(Map::mapNodeNameTemplate.arg(++this->mapNodeIndex));
    newMapNode->setTerrainType(terrainType);
    newMapNode->setNeighbours(neighbours);

    this->addMapNode(newMapNode);
}

void Map::createSettlement(SettlementType *settlementType, MapNode *mapNode)
{
    Settlement * newSettlement = new Settlement(this);
    newSettlement->setObjectName(
        Map::settlementNameTemplate.arg(++this->settlementIndex)
    );
    newSettlement->setSettlementType(settlementType);
    newSettlement->setMapNode(mapNode);
    newSettlement->setOwner(this->neutralPlayer);

    this->addSettlement(newSettlement);
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
    newUnit->setOwner(this->neutralPlayer);

    this->addUnit(newUnit);
}

GameObject * Map::resolveReference(const QString &objectName) const
{
    GameObject *gobject = this->findChild<GameObject *>(objectName);

    if (gobject == nullptr)
    {
        gobject = this->world->findChild<GameObject *>(objectName);
    }

    if (gobject == nullptr)
    {
        wError(category) << "Unable to resolve reference to " << objectName;
        throw Exception(Exception::UnresolvedReference);
    }

    return gobject;
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
    GameEntity *entity = GameEntity::get(worldName, &World::staticMetaObject);
    World *world = qobject_cast<World *>(entity);
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
    this->players = newListFromJson<Player>(obj["players"].toArray(), this);
    this->units = newListFromJson<Unit>(obj["units"].toArray(), this);
    this->settlements = newListFromJson<Settlement>(obj["settlements"].toArray(), this);

    this->setupContent();
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
            wError(category) << "Node " << nodeNameA << " does not exists";
            throw Exception(Exception::UnresolvedReference);
        }

        if (!mapNodeLookup.contains(nodeNameB))
        {
            wError(category) << "Node " << nodeNameB << " does not exists";
            throw Exception(Exception::UnresolvedReference);
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
