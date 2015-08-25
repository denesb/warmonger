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
    Q_PROPERTY(QVariant world READ readWorld NOTIFY worldChanged)
    Q_PROPERTY(QVariant mapNodes READ readMapNodes NOTIFY mapNodesChanged)
    Q_PROPERTY(QVariant players READ readPlayers)
    Q_PROPERTY(QVariant units READ readUnits)
    Q_PROPERTY(QVariant settlements READ readSettlements)

public:
    static const QString mapNodeNameTemplate;

    Map(QObject *parent);
    ~Map();

    virtual QString specification(const QString &objectName) const;

    const World * getWorld() const;
    void setWorld(const World *world);
    QVariant readWorld() const;

    QList<const MapNode *> getMapNodes() const;
    QList<MapNode *> getMapNodes();
    void setMapNodes(const QList<MapNode *> &mapNodes);
    QVariant readMapNodes() const;

    QList<const Player *> getPlayers() const;
    QList<Player *> getPlayers();
    void setPlayers(const QList<Player *> &units);
    QVariant readPlayers() const;

    QList<const Unit *> getUnits() const;
    QList<Unit *> getUnits();
    void setUnits(const QList<Unit *> &units);
    QVariant readUnits() const;

    QList<const Settlement *> getSettlements() const;
    QList<Settlement *> getSettlements();
    void setSettlements(const QList<Settlement *> &settlements);
    QVariant readSettlements() const;

    void createMapNode(const TerrainType *terrainType, const QHash<MapNode::Direction, MapNode *> &neighbours);
    Q_INVOKABLE void createMapNode(QObject *terrainType, QVariant neighbours);
    Q_INVOKABLE void changeMapNodeTerrainType(QObject *mapNode, QObject *newTerrainType);

signals:
    void worldChanged();
    void mapNodesChanged();
    void mapNodeCreated(QObject *mapNode);
    void mapNodeRemoved(QObject *mapNode);
    void mapNodeChanged(QObject *mapNode);

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    QList<MapNode *> mapNodesFromJson(const QJsonObject &obj);
    QJsonObject mapNodesToJson(const QList<MapNode *> &mapNodes) const;

    const World *world;
    int mapNodeIndex;
    QList<MapNode *> mapNodes;
    QList<Player *> players;
    QList<Unit *> units;
    QList<Settlement *> settlements;
};

} // namespace core
} // namespace warmonger

#endif // CORE_MAP_H
