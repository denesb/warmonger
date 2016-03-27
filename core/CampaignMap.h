#ifndef CORE_CAMPAIGN_MAP_H
#define CORE_CAMPAIGN_MAP_H

#include <map>
#include <vector>

#include <QObject>
#include <QString>
#include <QVariant>

#include "core/MapNode.h"
#include "core/Player.h"
#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/World.h"
#include "core/Unit.h"
#include "core/UnitType.h"

namespace warmonger {
namespace core {

class CampaignMap :
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
    explicit CampaignMap(QObject *parent=nullptr);

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

    std::vector<MapNode *> getMapNodes() const;
    void setMapNodes(const std::vector<MapNode *> &mapNodes);
    QVariantList readMapNodes() const;

    std::vector<Player *> getPlayers() const;
    void setPlayers(const std::vector<Player *> &units);
    QVariantList readPlayers() const;

    void addSettlement(Settlement *settlement);
    void removeSettlement(Settlement *settlement);

    std::vector<Settlement *> getSettlements() const;
    void setSettlements(const std::vector<Settlement *> &settlements);
    QVariantList readSettlements() const;

    void addUnit(Unit *unit);
    void removeUnit(Unit *unit);

    std::vector<Unit *> getUnits() const;
    void setUnits(const std::vector<Unit *> &units);
    QVariantList readUnits() const;

    void createMapNode(
        TerrainType *terrainType,
        const std::map<Direction, MapNode *> &neighbours
    );
    void createSettlement(
        SettlementType *settlementType,
        MapNode *mapNode,
        Player *owner
    );
    Unit * createUnit(UnitType *unitType, MapNode *mapNode, Player *owner);

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

private:
    QString displayName;
    World *world;
    int mapNodeIndex;
    int settlementIndex;
    int unitIndex;
    std::vector<MapNode *> mapNodes;
    std::vector<Player *> players;
    std::vector<Settlement *> settlements;
    std::vector<Unit *> units;
};

} // namespace core
} // namespace warmonger

#endif // CORE_CAMPAIGN_MAP_H
