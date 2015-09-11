#ifndef CORE_MAP_H
#define CORE_MAP_H

#include <QList>
#include <QString>

#include "core/GameEntity.h"
#include "core/MapNode.h"

namespace warmonger {
namespace core {

class World;
class Player;
class UnitType;
class Unit;
class SettlementType;
class Settlement;

class Map :
    public GameEntity
{
    Q_OBJECT
    Q_PROPERTY(QObject * world READ readWorld WRITE writeWorld NOTIFY worldChanged)
    Q_PROPERTY(QVariantList mapNodes READ readMapNodes NOTIFY mapNodesChanged)
    Q_PROPERTY(QVariantList players READ readPlayers NOTIFY playersChanged)
    Q_PROPERTY(QVariantList allPlayers READ readAllPlayers NOTIFY playersChanged)
    Q_PROPERTY(QObject * neutralPlayer READ readNeutralPlayer NOTIFY neutralPlayerChanged)
    Q_PROPERTY(QVariantList units READ readUnits)
    Q_PROPERTY(QVariantList settlements READ readSettlements NOTIFY settlementsChanged)

public:
    static const QString mapNodeNameTemplate;
    static const QString settlementNameTemplate;
    static const QString unitNameTemplate;

    Q_INVOKABLE Map(QObject *parent);
    ~Map();

    Q_INVOKABLE virtual QString getFileExtension() const;

    World * getWorld() const;
    void setWorld(World *world);
    QObject * readWorld() const;
    void writeWorld(QObject *world);

    void addMapNode(MapNode *mapNode);
    void removeMapNode(MapNode *mapNode);

    QList<MapNode *> getMapNodes() const;
    void setMapNodes(const QList<MapNode *> &mapNodes);
    QVariantList readMapNodes() const;

    QList<Player *> getPlayers() const;
    void setPlayers(const QList<Player *> &units);
    QVariantList readPlayers() const;
    QVariantList readAllPlayers() const;
    QObject * readNeutralPlayer() const;

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

    void createMapNode(TerrainType *terrainType, const QHash<MapNode::Direction, MapNode *> &neighbours);
    Q_INVOKABLE void createMapNode(QObject *terrainType, QVariantMap neighbours);

    void createSettlement(SettlementType *settlementType, MapNode *mapNode);
    Q_INVOKABLE void createSettlement(QObject *settlementType, QObject *mapNode);

    void createUnit(UnitType *unitType, MapNode *mapNode);
    Q_INVOKABLE void createUnit(QObject *unitType, QObject *mapNode);

signals:
    void worldChanged();
    void mapNodesChanged();
    void mapNodeAdded(QObject *mapNode);
    void mapNodeRemoved(QObject *mapNode);
    void playersChanged();
    void neutralPlayerChanged();
    void settlementsChanged();
    void settlementAdded(QObject *settlement);
    void settlementRemoved(QObject *settlement);
    void unitsChanged();
    void unitAdded(QObject *unit);
    void unitRemoved(QObject *unit);

private slots:
    void onSurfaceChanged();

protected:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    QList<MapNode *> mapNodesFromJson(const QJsonObject &obj);
    QJsonObject mapNodesToJson(const QList<MapNode *> &mapNodes) const;

    Settlement * getSettlementOn(MapNode *mapNode) const;
    Unit * getUnitOn(MapNode *mapNode) const;

    World *world;
    int mapNodeIndex;
    int settlementIndex;
    int unitIndex;
    QList<MapNode *> mapNodes;
    QList<Settlement *> settlements;
    QList<Unit *> units;
    QList<Player *> players;
    Player *neutralPlayer;
};

} // namespace core
} // namespace warmonger

#endif // CORE_MAP_H
