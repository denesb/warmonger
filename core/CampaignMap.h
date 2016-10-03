#ifndef CORE_CAMPAIGN_MAP_H
#define CORE_CAMPAIGN_MAP_H

#include <memory>
#include <vector>

#include <QObject>
#include <QString>
#include <QVariant>

#include "core/Army.h"
#include "core/Faction.h"
#include "core/MapNode.h"
#include "core/Settlement.h"
#include "core/Unit.h"
#include "core/World.h"

namespace warmonger {
namespace core {

class CampaignMap : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(World* world READ getWorld WRITE setWorld NOTIFY worldChanged)
    Q_PROPERTY(QVariantList mapNodes READ readMapNodes NOTIFY mapNodesChanged)
    Q_PROPERTY(QVariantList factions READ readFactions NOTIFY factionsChanged)
    Q_PROPERTY(QVariantList units READ readUnits)
    Q_PROPERTY(QVariantList settlements READ readSettlements NOTIFY settlementsChanged)

public:
    typedef std::tuple<MapNode*, Settlement*, Army*> Content;

    explicit CampaignMap(QObject* parent = nullptr);

    const QString& getDisplayName() const
    {
        return this->displayName;
    }

    void setDisplayName(const QString& displayName);

    World* getWorld() const
    {
        return this->world;
    }

    void setWorld(World* world);

    const std::vector<MapNode*>& getMapNodes() const
    {
        return this->mapNodes;
    }

    void setMapNodes(const std::vector<MapNode*>& mapNodes);
    QVariantList readMapNodes() const;

    const std::vector<Faction*>& getFactions() const
    {
        return this->factions;
    }

    void setFactions(const std::vector<Faction*>& units);
    QVariantList readFactions() const;

    const std::vector<Settlement*>& getSettlements() const
    {
        return this->settlements;
    }

    /**
     * Set settlements
     *
     * Destroys previous settlements
     */
    void setSettlements(const std::vector<Settlement*>& settlements);
    QVariantList readSettlements() const;

    const std::vector<Unit*>& getUnits() const
    {
        return this->units;
    }

    void setUnits(const std::vector<Unit*>& units);
    QVariantList readUnits() const;

    const std::vector<Army*>& getArmies() const
    {
        return this->armies;
    }

    void setArmies(const std::vector<Army*>& armies);
    QVariantList readArmies() const;

    MapNode* createMapNode(TerrainType* terrainType, const MapNodeNeighbours& neighbours);
    std::unique_ptr<MapNode> removeMapNode(MapNode* mapNode);

    /**
     * Create a new settlement and assume ownership
     *
     * The newly created settlement is added to the settlement list and it is
     * assigned a unique objectName (in the context of this map).
     *
     * @param[in] SettlementType* settlementType the type of the settlement
     *
     * @returns Settlement* the newly created settlement
     */
    Settlement* createSettlement(SettlementType* settlementType);

    /**
     * Remove settlement from the settlement list and renounce ownership
     *
     * The removed settlement is removed as an std::unique_ptr and will be
     * destroyed if the caller doesn't save it.
     *
     * @param[in] Settlement* settlement the settlement to remove
     *
     * @returns std::unique_ptr<Settlement> the removed settlement or an empty
     * pointer if the settlement  was not found
     */
    std::unique_ptr<Settlement> removeSettlement(Settlement* settlement);

    Unit* createUnit(UnitType* unitType);
    std::unique_ptr<Unit> removeUnit(Unit* unit);

    Army* createArmy(ArmyType* armyType);
    std::unique_ptr<Army> removeArmy(Army* army);

    const std::vector<Content>& getContents() const
    {
        return this->contents;
    }

private:
    void rebuildContents();

private slots:
    void settlementMapNodeChanged();
    void armyMapNodeChanged();

signals:
    void displayNameChanged();
    void worldChanged();
    void factionsChanged();
    void mapNodesChanged();
    void settlementsChanged();
    void unitsChanged();
    void armiesChanged();

private:
    QString displayName;
    World* world;
    int mapNodeIndex;
    int settlementIndex;
    int unitIndex;
    int armyIndex;
    std::vector<Faction*> factions;
    std::vector<MapNode*> mapNodes;
    std::vector<Settlement*> settlements;
    std::vector<Unit*> units;
    std::vector<Army*> armies;

    std::vector<Content> contents;
};

} // namespace core
} // namespace warmonger

#endif // CORE_CAMPAIGN_MAP_H
