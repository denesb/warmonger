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
    static const QString fileExtension;

    Game(QObject *parent);
    ~Game();

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
     * node to the destination node. The unit will move until either
     * all of its movement points are exhausted or it reached the
     * destination node.
     * Return the path actually travelled by the unit.
     */
    QList<MapNode *> moveUnitToNode(Unit *unit, MapNode *node);

    /**
     * Check if a unit of type `unitType` can be recruited on the node
     * `node`.
     *
     * Check all applying game rules before actually recruiting the
     * unit. That is:
     * - One of the `node`'s neighbours must contain a settlement _and_
     *   a unit of at least `Officer` rank.
     * - The player's faction must be able to recruit `unitType` from
     *   the settlement (see above).
     * - The player must have the funds to recruit the unit.
     * - The `node` must not be occupied (cannot contain a unit).
     * - The `node` must be passable for `unitType`.
     */
    bool canRecruitUnit(UnitType *unitType, MapNode *node) const;

    /**
     * Recruit a new unit of type `unitType` on the node `node`.
     *
     * Check all applying game rules before actually recruiting the
     * unit. See `canRecruitUnit`.
     *
     * If any of the rules are violated an exception will be thrown and
     * no unit will be recruited.
     *
     * The unit will have no movement points when recruited.
     */
    void recruitUnit(UnitType *unitType, MapNode *node);

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

    QString checkUnitRecruitmentRules(UnitType *unitType, MapNode *node) const;

    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    int turn;
    int playerIndex;
};

} // namespace core
} // namespace warmonger

#endif // CORE_GAME_H
