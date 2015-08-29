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
class Unit;
class Settlement;

class Map :
    public GameEntity
{
    Q_OBJECT
    Q_PROPERTY(QVariant world READ readWorld WRITE writeWorld NOTIFY worldChanged)
    Q_PROPERTY(QVariant mapNodes READ readMapNodes NOTIFY mapNodesChanged)
    Q_PROPERTY(QVariant players READ readPlayers)
    Q_PROPERTY(QVariant units READ readUnits)
    Q_PROPERTY(QVariant settlements READ readSettlements NOTIFY settlementsChanged)

public:
    static const QString mapNodeNameTemplate;

    Map(QObject *parent);
    ~Map();

    virtual QString specification(const QString &objectName) const;

    World * getWorld() const;
    void setWorld(World *world);
    QVariant readWorld() const;
    void writeWorld(QVariant world);

    void addMapNode(MapNode *mapNode);
    void removeMapNode(MapNode *mapNode);

    QList<MapNode *> getMapNodes() const;
    void setMapNodes(const QList<MapNode *> &mapNodes);
    QVariant readMapNodes() const;

    QList<Player *> getPlayers() const;
    void setPlayers(const QList<Player *> &units);
    QVariant readPlayers() const;

    QList<Unit *> getUnits() const;
    void setUnits(const QList<Unit *> &units);
    QVariant readUnits() const;

    QList<Settlement *> getSettlements() const;
    void setSettlements(const QList<Settlement *> &settlements);
    QVariant readSettlements() const;

    void createMapNode(TerrainType *terrainType, const QHash<MapNode::Direction, MapNode *> &neighbours);
    Q_INVOKABLE void createMapNode(QObject *terrainType, QVariantMap neighbours);

signals:
    void worldChanged();
    void mapNodesChanged();
    void mapNodeAdded(QObject *mapNode);
    void mapNodeRemoved(QObject *mapNode);
    void settlementsChanged();
    void settlementAdded(QObject *settlement);
    void settlementRemoved(QObject *settlement);

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    QList<MapNode *> mapNodesFromJson(const QJsonObject &obj);
    QJsonObject mapNodesToJson(const QList<MapNode *> &mapNodes) const;

    World *world;
    int mapNodeIndex;
    QList<MapNode *> mapNodes;
    QList<Player *> players;
    QList<Unit *> units;
    QList<Settlement *> settlements;
};

} // namespace core
} // namespace warmonger

#endif // CORE_MAP_H
