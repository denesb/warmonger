#ifndef CORE_MAP_H
#define CORE_MAP_H

#include <map>
#include <tuple>

#include <QList>
#include <QObject>
#include <QString>

#include "core/MapNode.h"
#include "core/Player.h"
#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/World.h"
#include "core/Unit.h"
#include "core/UnitType.h"

namespace warmonger {
namespace core {

/*
 *  NW  /\  NE
 *    /    \
 * W |      | E
 *   |      |
 *    \    /
 *  SW  \/  SE
 */
enum class Direction
{
    West,
    NorthWest,
    NorthEast,
    East,
    SouthEast,
    SouthWest
};

/**
 * Converts Directions to QString.
 */
QString direction2str(Direction d);

/**
 * Converts QString to Direction.
 *
 * Will throw a ValueError if str is not a valid direction name.
 */
Direction str2direction(const QString &str);

/**
 * Return the opposite direction of d.
 */
Direction oppositeDirection(Direction d);

/*
 *   SW_NE
 *  /
 * /
 * ---> W_E
 * \
 *  \
 *   NW_SE
 */
enum class Axis
{
    West_East,
    SouthWest_NorthEast,
    NorthWest_SouthEast
};

/**
 * Converts Axis to QString.
 */
QString axis2str(Axis a);

/**
 * Converts QString to Axis.
 *
 * Throws a ValueError if str is not a valid axis name.
 */
Axis str2axis(const QString &str);

/**
 * Return the pair of directions for the axis.
 */
std::tuple<Direction, Direction> axisDirections(Axis a);

/**
 * Return the Axis for the Direction.
 */
Axis directionAxis(Direction d);


typedef std::tuple<MapNode *, MapNode *, Axis> MapNodeConnection;

class Map :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(World * world READ getWorld WRITE setWorld NOTIFY worldChanged)
    Q_PROPERTY(QVariantList mapNodes READ readMapNodes NOTIFY mapNodesChanged)
    Q_PROPERTY(QVariantList players READ readPlayers NOTIFY playersChanged)
    Q_PROPERTY(QVariantList units READ readUnits)
    Q_PROPERTY(QVariantList settlements READ readSettlements NOTIFY settlementsChanged)

public:
    explicit Map(QObject *parent=nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    World * getWorld() const;
    void setWorld(World *world);

    int getMapNodeIndex() const;
    void setMapNodeIndex(int mapNodeIndex);

    int getSettlementIndex() const;
    void setSettlementIndex(int settlementIndex);

    int getUnitIndex() const;
    void setUnitIndex(int unitIndex);

    void addMapNode(MapNode *mapNode);
    void removeMapNode(MapNode *mapNode);

    QList<MapNode *> getMapNodes() const;
    void setMapNodes(const QList<MapNode *> &mapNodes);
    QVariantList readMapNodes() const;

    std::vector<MapNodeConnection> getMapNodeConnections() const;
    void setMapNodeConnections(const std::vector<MapNodeConnection> &mapNodeConnections);

    void addMapNodeConnection(const MapNodeConnection &mapNodeConnection);
    void addMapNodeConnection(MapNode *mn0, MapNode *mn1, Axis axis);

    QList<Player *> getPlayers() const;
    void setPlayers(const QList<Player *> &units);
    QVariantList readPlayers() const;

    void addSettlement(Settlement *settlement);
    void removeSettlement(Settlement *settlement);

    QList<Settlement *> getSettlements() const;
    void setSettlements(const QList<Settlement *> &settlements);
    QVariantList readSettlements() const;

    void addUnit(Unit *unit);
    void removeUnit(Unit *unit);

    QList<Unit *> getUnits() const;
    void setUnits(const QList<Unit *> &units);
    QVariantList readUnits() const;

    void createMapNode(
        TerrainType *terrainType,
        const QMap<Direction, MapNode *> &neighbours
    );
    void createSettlement(
        SettlementType *settlementType,
        MapNode *mapNode,
        Player *owner
    );
    Unit * createUnit(UnitType *unitType, MapNode *mapNode, Player *owner);

    Settlement * getSettlementOn(const MapNode *mapNode) const;
    Unit * getUnitOn(const MapNode *mapNode) const;

    bool hasSettlement(const MapNode *mapNode) const;
    bool hasUnit(const MapNode *mapNode) const;

signals:
    void displayNameChanged();
    void worldChanged();
    void mapNodeIndexChanged();
    void settlementIndexChanged();
    void unitIndexChanged();
    void mapNodesAboutToChange();
    void mapNodesChanged();
    void mapNodeAdded(QObject *mapNode);
    void mapNodeRemoved(QObject *mapNode);
    void settlementsAboutToChange();
    void settlementsChanged();
    void settlementAdded(QObject *settlement);
    void settlementRemoved(QObject *settlement);
    void unitsAboutToChange();
    void unitsChanged();
    void unitAdded(Unit *unit);
    void unitRemoved(Unit *unit);
    void playersChanged();

protected:
    QList<MapNode *> mapNodesFromJson(const QJsonObject &obj);
    QJsonObject mapNodesToJson(const QList<MapNode *> &mapNodes) const;

    QString displayName;
    void onMapNodesAboutToChange();
    void onMapNodesChanged();
    void onMapNodeAdded(MapNode *n);
    void onMapNodeRemoved(MapNode *n);
    void onSettlementsAboutToChange();
    void onSettlementsChanged();
    void onSettlementAdded(Settlement *s);
    void onSettlementRemoved(Settlement *s);
    void onUnitsAboutToChange();
    void onUnitsChanged();
    void onUnitAdded(Unit *u);
    void onUnitRemoved(Unit *u);

    void setupContent();
    void updateContent();

    World *world;
    int mapNodeIndex;
    int settlementIndex;
    int unitIndex;
    QList<MapNode *> mapNodes;
    QList<Player *> players;
    QList<Settlement *> settlements;
    QList<Unit *> units;
    QMap<const MapNode *, QPair<Settlement *, Unit *>> mapContent;
    std::map<std::tuple<MapNode *, Direction>, MapNode *> mapNodesNeighbours;
};

} // namespace core
} // namespace warmonger

#endif // CORE_MAP_H
