#include <algorithm>
#include <functional>
#include <limits>
#include <typeinfo>

#include "core/Game.h"
#include "core/MapNode.h"
#include "core/UnitClass.h"
#include "core/UnitType.h"
#include "core/Unit.h"
#include "core/Util.h"

using namespace warmonger::core;

static const QString category{"core"};

static double movementCost(
    const UnitClass *klass,
    const MapNode *node1,
    const MapNode *node2
);

static QMap<MapNode *, MapNode *> dijkstraPath(
    MapNode *source,
    MapNode *target,
    QList<MapNode *> graph,
    std::function<double(const MapNode *, const MapNode*)> edgeCostFunc
);

Game::Game() :
    Map(),
    turnNumber(0),
    playerIndex(0)
{
}

Game::~Game()
{
}

QString Game::fileExtension() const
{
    return "wgd";
}

QSet<MapNode *> Game::reachableMapNodes(Unit *unit) const
{
    QSet<MapNode *> reachableNodes;
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
    UnitClass *klass = unit->getUnitType()->getUnitClass();
    std::function<double(const MapNode *, const MapNode *)> edgeCostFunc =
        std::bind(
            movementCost,
            klass,
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

void Game::reachableMapNodes(
    QSet<MapNode *> &reachedNodes,
    MapNode *node,
    Unit *unit,
    double mp
) const
{
    UnitClass *klass = unit->getUnitType()->getUnitClass();

    QHash<MapNode::Direction, MapNode *> neighbours = node->getNeighbours();
    for (MapNode *neighbour : neighbours)
    {
        if (neighbour == nullptr)
            continue;

        double cost = movementCost(klass, node, neighbour);
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
    this->turnNumber = 0;
    this->playerIndex = 0;
}

void Game::dataFromJson(const QJsonObject &obj)
{
    Map::dataFromJson(obj);
    this->turnNumber = obj["turnNumber"].toInt();
    this->playerIndex = obj["playerIndex"].toInt();
}

void Game::dataToJson(QJsonObject &obj) const
{
    Map::dataToJson(obj);
}

double movementCost(
    const UnitClass *klass,
    const MapNode *node1,
    const MapNode *node2
)
{
    int cost1i = klass->getMovementCost(node1->getTerrainType());
    int cost2i = klass->getMovementCost(node2->getTerrainType());

    if (cost1i <= 0 || cost2i <= 0)
        return std::numeric_limits<double>::max();

    double cost1 = static_cast<double>(cost1i);
    double cost2 = static_cast<double>(cost2i);

    return (cost1 + cost2)/2.0;
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
