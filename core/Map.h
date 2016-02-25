#ifndef CORE_MAP_H
#define CORE_MAP_H

#include <QList>
#include <QString>

#include "core/GameEntity.h"
#include "core/GameObject.h"
#include "core/MapNode.h"
#include "core/Player.h"
#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/World.h"
#include "core/Unit.h"
#include "core/UnitType.h"

namespace warmonger {
namespace core {

class Map :
    public GameEntity
{
    Q_OBJECT
    Q_PROPERTY(World * world READ getWorld WRITE setWorld NOTIFY worldChanged)
    Q_PROPERTY(QVariantList mapNodes READ readMapNodes NOTIFY mapNodesChanged)
    Q_PROPERTY(QVariantList players READ readPlayers NOTIFY playersChanged)
    Q_PROPERTY(QVariantList units READ readUnits)
    Q_PROPERTY(QVariantList settlements READ readSettlements NOTIFY settlementsChanged)

public:
    static const QString fileExtension;
    static const QString mapNodeNameTemplate;
    static const QString settlementNameTemplate;
    static const QString unitNameTemplate;

    Map(QObject *parent=nullptr);
    ~Map();

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
        const QHash<MapNode::Direction, MapNode *> &neighbours
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
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    QList<MapNode *> mapNodesFromJson(const QJsonObject &obj);
    QJsonObject mapNodesToJson(const QList<MapNode *> &mapNodes) const;

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
    QHash<const MapNode *, QPair<Settlement *, Unit *>> mapContent;
};

} // namespace core
} // namespace warmonger

#endif // CORE_MAP_H
