#include <algorithm>
#include <functional>
#include <limits>
#include <typeinfo>

#include "core/Game.h"
#include "core/MapNode.h"
#include "core/UnitClass.h"
#include "core/UnitType.h"
#include "core/Unit.h"

using namespace warmonger::core;

static const QString loggerName{"core.Game"};
const QString Game::fileExtension{"wgd"};

/**
 * Find the shortest path with the dijstra algorithm
 */
static QMap<MapNode *, MapNode *> dijkstraPath(
    MapNode *source,
    MapNode *target,
    QList<MapNode *> graph,
    std::function<double(const MapNode *, const MapNode*)> edgeCostFunc
);

Game::Game(QObject *parent) :
    Map(parent),
    turn(0),
    playerIndex(0)
{
}

Game::~Game()
{
}

int Game::getTurn() const
{
    return this->turn;
}

void Game::setTurn(int turn)
{
    if (this->turn != turn)
    {
        this->turn = turn;
        emit turnChanged();
    }
}

QSet<MapNode *> Game::reachableMapNodes(Unit *unit) const
{
    QSet<MapNode *> reachableNodes;

    // Always include the current node
    reachableNodes.insert(unit->getMapNode());

    this->reachableMapNodes(
        reachableNodes,
        unit->getMapNode(),
        unit,
        unit->getMovementPoints()
    );

    return std::move(reachableNodes);
}

QList<MapNode *> Game::shortestPath(
    Unit *unit,
    MapNode *node1,
    MapNode *node2
) const
{
    std::function<double(const MapNode *, const MapNode *)> edgeCostFunc =
        std::bind(
            &Game::movementCost,
            this,
            unit,
            std::placeholders::_1,
            std::placeholders::_2
        );

    QMap<MapNode *, MapNode *> paths =
        dijkstraPath(node1, node2, this->mapNodes, edgeCostFunc);

    QList<MapNode *> path;
    MapNode *node = node2;
    while (node != nullptr)
    {
        path.prepend(node);
        node = paths[node];
    }

    if (path.first() != node1 || path.last() != node2)
        path.clear(); // the path is no good

    return path;
}

QList<MapNode *> Game::moveUnitAlongPath(
    Unit *unit,
    const QList<MapNode *> &path
)
{
    double mp = unit->getMovementPoints();
    QList<MapNode*> travelledPath;

    // The first node in the path is actually
    travelledPath.append(path[0]);

    for (int i = 1; i < path.size(); i++)
    {
        MapNode *n0 = path[i - 1];
        MapNode *n1 = path[i];

        const double cost = this->movementCost(unit, n0, n1);
        if (mp < cost)
            break;

        mp -= cost;
        travelledPath.append(n1);
    }

    unit->setMovementPoints(mp);
    unit->setMapNode(travelledPath.last());

    return std::move(travelledPath);
}

QList<MapNode *> Game::moveUnitToNode(Unit *unit, MapNode *node)
{
    QList<MapNode *> desiredPath = this->shortestPath(
        unit,
        unit->getMapNode(),
        node
    );

    QList<MapNode *> path;

    // If there is no path to the destination return empty path
    if (desiredPath.size() == 0)
        return path;

    path = this->moveUnitAlongPath(unit, desiredPath);

    return std::move(path);
}

bool Game::canRecruitUnit(UnitType *unitType, MapNode *node) const
{
    return this->checkUnitRecruitmentRules(unitType, node).isNull();
}

void Game::recruitUnit(UnitType *unitType, MapNode *node)
{
    Player *owner = this->players[this->playerIndex];

    const QString error = this->checkUnitRecruitmentRules(unitType, node);

    if (!error.isNull())
        throw UnitRecruitError(error);

    Unit *newUnit = this->createUnit(unitType, node, owner);
    newUnit->setMovementPoints(0);

    wInfo(loggerName) << "Unit " << newUnit << " recruited";
}

double Game::movementCost(
    const Unit *unit,
    const MapNode *node1,
    const MapNode *node2
) const
{
    // Cannot move to an already occupied node
    if (this->hasUnit(node2))
        return std::numeric_limits<double>::max();

    const UnitClass *klass = unit->getType()->getClass();
    int cost1i = klass->getMovementCost(node1->getTerrainType());
    int cost2i = klass->getMovementCost(node2->getTerrainType());

    // Less than 0 cost means impassable terrain for this unit
    if (cost1i <= 0 || cost2i <= 0)
        return std::numeric_limits<double>::max();

    double cost1 = static_cast<double>(cost1i);
    double cost2 = static_cast<double>(cost2i);

    return (cost1 + cost2)/2.0;
}

void Game::reachableMapNodes(
    QSet<MapNode *> &reachedNodes,
    MapNode *node,
    Unit *unit,
    double mp
) const
{
    QHash<MapNode::Direction, MapNode *> neighbours = node->getNeighbours();
    for (MapNode *neighbour : neighbours)
    {
        if (neighbour == nullptr)
            continue;

        double cost = this->movementCost(unit, node, neighbour);
        if (mp >= cost)
        {
            reachedNodes << neighbour;
            this->reachableMapNodes(
                reachedNodes,
                neighbour,
                unit,
                mp - cost
            );
        }
    }
}

void Game::fromMapJson(const QJsonObject &obj)
{
    Map::fromJson(obj);
    this->turn = 0;
    this->playerIndex = 0;
}

void Game::dataFromJson(const QJsonObject &obj)
{
    Map::dataFromJson(obj);
    this->turn = obj["turn"].toInt();
    this->playerIndex = obj["playerIndex"].toInt();
}

void Game::dataToJson(QJsonObject &obj) const
{
    Map::dataToJson(obj);
}

QString Game::checkUnitRecruitmentRules(
    UnitType *unitType,
    MapNode *node
) const
{
    bool hasRecruitingNeighbour = false;
    QHash<MapNode::Direction, MapNode *> neighbours = node->getNeighbours();

    for (MapNode *neighbour : neighbours)
    {
        if (!this->hasSettlement(neighbour) || !this->hasUnit(neighbour))
            continue;

        Settlement *s = this->getSettlementOn(neighbour);
        Unit *u = this->getUnitOn(neighbour);
        QList<UnitType *> recruits = s->getType()->getRecruits();

        if (u->getRank() >= Unit::Officer && recruits.contains(unitType))
        {
            hasRecruitingNeighbour = true;
            break;
        }
    }

    if (!hasRecruitingNeighbour)
    {
        return QStringLiteral("No settlement with an officer were found in the neighbourhood where this unit can be trained");
    }

    //TODO: fund check (not yet implemented)

    if (this->hasUnit(node))
    {
        return QStringLiteral("This map-node is already occupied by another unit");
    }

    UnitClass *klass = unitType->getClass();
    if (klass->getMovementCost(node->getTerrainType()) < 0)
    {
        return QStringLiteral("This map-node is impassable for this unit-type");
    }

    // All seems fine
    return QString();
}

QMap<MapNode *, MapNode *> dijkstraPath(
    MapNode *source,
    MapNode *target,
    QList<MapNode *> graph,
    std::function<double(const MapNode *, const MapNode*)> edgeCostFunc
)
{
    const double infinity = std::numeric_limits<double>::max();
    QMap<const MapNode *, double> distances;
    QMap<MapNode *, MapNode *> paths;
    QSet<MapNode *> unvisited;

    for (MapNode *node : graph)
    {
        distances[node] = infinity;
        paths[node] = nullptr;
        unvisited << node;
    }
    distances[source] = 0;

    auto cmp = [&](const MapNode * n1, const MapNode * n2)
    {
        return distances[n1] < distances[n2];
    };

    while (!unvisited.empty())
    {
        auto it = std::min_element(
            unvisited.constBegin(),
            unvisited.constEnd(),
            cmp
        );
        MapNode *node = *it;
        unvisited.remove(node);

        if (node == target)
            break;

        double currentDistance = distances[node];
        if (currentDistance == infinity)
            break;

        QHash<MapNode::Direction, MapNode *> neighbours = node->getNeighbours();
        for (MapNode *neighbour : neighbours)
        {
            if (neighbour == nullptr || !unvisited.contains(neighbour))
                continue;

            double distance = currentDistance + edgeCostFunc(node, neighbour);
            if (distance < distances[neighbour])
            {
                distances[neighbour] = distance;
                paths[neighbour] = node;
            }
        }
    }

    return paths;
}
