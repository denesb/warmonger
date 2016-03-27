#ifndef CORE_CAMPAIGN_MAP_H
#define CORE_CAMPAIGN_MAP_H

#include <map>
#include <vector>

#include <QObject>
#include <QString>
#include <QVariant>

#include "core/MapNode.h"
#include "core/Faction.h"
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
    Q_PROPERTY(QVariantList factions READ readFactions NOTIFY factionsChanged)
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

    int getArmyIndex() const;
    void setArmyIndex(int armyIndex);

    std::vector<MapNode *> getMapNodes() const;
    void setMapNodes(const std::vector<MapNode *> &mapNodes);
    QVariantList readMapNodes() const;

    std::vector<Faction *> getFactions() const;
    void setFactions(const std::vector<Faction *> &units);
    QVariantList readFactions() const;

    std::vector<Settlement *> getSettlements() const;
    void setSettlements(const std::vector<Settlement *> &settlements);
    QVariantList readSettlements() const;

    std::vector<Unit *> getUnits() const;
    void setUnits(const std::vector<Unit *> &units);
    QVariantList readUnits() const;

    void addMapNode(MapNode *mapNode);
    void removeMapNode(MapNode *mapNode);

    void addSettlement(Settlement *settlement);
    void removeSettlement(Settlement *settlement);

    void addUnit(Unit *unit);
    void removeUnit(Unit *unit);

signals:
    void displayNameChanged();
    void worldChanged();
    void mapNodeIndexChanged();
    void settlementIndexChanged();
    void unitIndexChanged();
    void armyIndexChanged();
    void mapNodesChanged();
    void settlementsChanged();
    void unitsChanged();
    void unitRemoved(Unit *unit);
    void factionsChanged();

private:
    QString displayName;
    World *world;
    int mapNodeIndex;
    int settlementIndex;
    int unitIndex;
    int armyIndex;
    std::vector<MapNode *> mapNodes;
    std::vector<Faction *> factions;
    std::vector<Settlement *> settlements;
    std::vector<Unit *> units;
};

} // namespace core
} // namespace warmonger

#endif // CORE_CAMPAIGN_MAP_H
