#include "core/Game.h"
#include "core/MapNode.h"
#include "core/UnitClass.h"
#include "core/UnitType.h"
#include "core/Unit.h"
#include "core/Util.h"

using namespace warmonger::core;

static const QString category{"core"};

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

QVariantList Game::reachableMapNodes(QObject *unit) const
{
    Unit *u = qobject_cast<Unit *>(unit);
    if (u == nullptr)
    {
        wError(category) << "unit is null or has wrong type";
        throw Exception(Exception::InvalidValue);
    }
    QList<MapNode *> reachableNodes = this->reachableMapNodes(u);

    return toQVariantList<MapNode>(reachableNodes);
}

QList<MapNode *> Game::reachableMapNodes(Unit *unit) const
{
    QSet<MapNode *> reachableNodes;
    this->reachableMapNodes(
        reachableNodes,
        unit->getMapNode(),
        unit,
        unit->getMovementPoints()
    );

    return std::move(reachableNodes.toList());
}

void Game::reachableMapNodes(
    QSet<MapNode *> &reachedNodes,
    MapNode *node,
    Unit *unit,
    double mp
) const
{
    UnitClass *klass = unit->getUnitType()->getUnitClass();
    int cost1i = klass->getMovementCost(node->getTerrainType());
    double cost1 = static_cast<double>(cost1i);

    QHash<MapNode::Direction, MapNode *> neighbours = node->getNeighbours();
    QHash<MapNode::Direction, MapNode *>::ConstIterator it;
    for (it = neighbours.constBegin(); it != neighbours.constEnd(); it++)
    {
        MapNode *neighbour = it.value();
        if (neighbour == nullptr)
            continue;

        int cost2i = klass->getMovementCost(neighbour->getTerrainType());
        if (cost2i <= 0)
            continue; // 0 or less means impassable terrain for the unit

        double cost2 = static_cast<double>(cost2i);
        double cost = (cost1 + cost2)/2.0;
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
