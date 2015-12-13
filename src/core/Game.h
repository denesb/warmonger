#ifndef CORE_GAME_H
#define CORE_GAME_H

#include <QSet>

#include "core/Map.h"

namespace warmonger {
namespace core {

class MapNode;
class Unit;

class Game :
    public Map
{
    Q_OBJECT

    Q_PROPERTY(int turn READ getTurn WRITE setTurn NOTIFY turnChanged)

public:

    Game();
    ~Game();

    Q_INVOKABLE virtual QString fileExtension() const;

    int getTurn() const;
    void setTurn(int turn);

    /**
     * Get all reachable nodes for the unit.
     *
     * A node is reachable if the unit can travel to that node with its
     * currently available movement points.
     * The current node of the unit is always included.
     */
    QSet<MapNode *> reachableMapNodes(Unit *unit) const;

    /**
     * Get the shortest path for the unit from node1 to node2.
     *
     * The shortest path is calculated from the unit's perspective,
     * that is the shortest path is that which requires the least
     * movement points for the given unit.
     * N.B.: The first node in the path is always the starting point!
     * If no path can be found return an empty list.
     */
    QList<MapNode *> shortestPath(
        Unit *unit,
        MapNode *node1,
        MapNode *node2
    ) const;

    /**
     * Move the unit along the path.
     *
     * The unit will move until all its movement points are exhausted or
     * it reached the last node in the path.
     * Return the path actually travelled by the unit.
     * N.B.: The first node in the path should always the starting
     * point, that is the node the unit is on!
     */
    QList<MapNode *> moveUnitAlongPath(
        Unit *unit,
        const QList<MapNode *> &path
    );

    /**
     * Convenience method to move the unit to the destination node.
     *
     * The unit will be moved along the shortest path from its current
     * node to the destination node. The unit will move unitl either
     * all of its movement points are exhausted or it reached the
     * destination node.
     * Return the path actually travelled by the unit.
     */
    QList<MapNode *> moveUnitToNode(Unit *unit, MapNode *node);

    void fromMapJson(const QJsonObject &obj);

signals:
    void turnChanged();

protected:
    /**
     * Get the movement cost for travelling from node1 to node2.
     */
    double movementCost(
        const Unit *unit,
        const MapNode *node1,
        const MapNode *node2
    ) const;

private:
    void reachableMapNodes(
        QSet<MapNode *> &reachedNodes,
        MapNode *node,
        Unit *unit,
        double mp
    ) const;

    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    int turn;
    int playerIndex;
};

} // namespace core
} // namespace warmonger

#endif // CORE_GAME_H
