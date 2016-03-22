#include <algorithm>
#include <functional>
#include <limits>
#include <typeinfo>

#include "core/Exception.h"
#include "core/Game.h"
#include "core/MapNode.h"
#include "core/UnitClass.h"
#include "core/UnitType.h"
#include "core/Unit.h"

using namespace warmonger::core;

static const QString loggerName{"core.Game"};

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

    return reachableNodes;
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

    return travelledPath;
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

    return path;
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

    const int newGoldBalance = owner->getGoldBalance()
        - unitType->getRecruitmentCost();
    owner->setGoldBalance(newGoldBalance);

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
    /*
    QHash<Map::Direction, MapNode *> neighbours = node->getNeighbours();
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
    */
}

QString Game::checkUnitRecruitmentRules(
    UnitType *unitType,
    MapNode *node
) const
{
    Player *owner = this->players[this->playerIndex];
    if (unitType->getRecruitmentCost() > owner->getGoldBalance())
    {
        return QStringLiteral("Insufficient funds");
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

        /*
        QHash<Map::Direction, MapNode *> neighbours = node->getNeighbours();
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
        */
    }

    return paths;
}
