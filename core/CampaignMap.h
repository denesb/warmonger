/** \file
 * CampaignMap class.
 *
 * \copyright (C) 2015-2016 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

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

/**
 * A campaign-map.
 *
 * The campaign-map is were the game actually take place. The campaign-map
 * contain the graph of map-nodes that form the terrain. It also contains the list
 * of factions, armies and settlements that participate in the game. A
 * campaign-map is linked to a world which defines all aspects of its behaviour.
 *
 * \see warmonger::core::MapNode
 * \see warmonger::core::Army
 * \see warmonger::core::Settlement
 * \see warmonger::core::Faction
 * \see warmonger::core::World
 */
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

    /**
     * Constructs an empty CampaignMap.
     *
     *\param parent the parent QObject.
     */
    explicit CampaignMap(QObject* parent = nullptr);

    /**
     * Get the display-name.
     *
     *\returns the displayName
     */
    const QString& getDisplayName() const
    {
        return this->displayName;
    }

    /**
     * Set the display-name.
     *
     * Will emit the signal CampaignMap::displayNameChanged() if the newly set
     * value is different than the current one.
     *
     * \param displayName the new displayName
     */
    void setDisplayName(const QString& displayName);

    /**
     * Get the world.
     *
     * \return the world
     */
    World* getWorld() const
    {
        return this->world;
    }

    /**
     * Set the world.
     *
     * Will emit the signal CampaignMap::worldChanged() if the newly set value
     * is different than the current one.
     *
     * \param world the new world
     */
    void setWorld(World* world);

    /**
     * Get the map-nodes.
     *
     * \return the map-nodes
     */
    const std::vector<MapNode*>& getMapNodes() const
    {
        return this->mapNodes;
    }

    /**
     * Get the map-nodes as a QVariantList.
     *
     * This function is used as a read function for the mapNodes property and is
     * not supposed to be called from C++ code. Use CampaignMap::getMapNodes()
     * instead.
     *
     * \returns the map-nodes
     */
    QVariantList readMapNodes() const;

    /**
     * Set the map-nodes.
     *
     * Will emit the signal CampaignMap::mapNodesChanged() if the newly set
     * value is different than the current one.
     * Destroys previous map-node objects.
     *
     * \param mapNodes the map-nodes
     */
    void setMapNodes(const std::vector<MapNode*>& mapNodes);

    /**
     * Get the factions.
     *
     * \returns the factions
     */
    const std::vector<Faction*>& getFactions() const
    {
        return this->factions;
    }

    /**
     * Get the factions as a QVariantList.
     *
     * This function is used as a read function for the mapNodes property and is
     * not supposed to be called from C++ code. Use CampaignMap::getFactions()
     * instead.
     *
     * \returns the factions
     */
    QVariantList readFactions() const;

    /**
     * Set the factions.
     *
     * Will emit the signal CampaignMap::factionsChanged() if the newly set
     * value is different than the current one.
     * Destroys previous faction objects.
     *
     * \param factions the new factions
     */
    void setFactions(const std::vector<Faction*>& factions);

    /**
     * Get the settlements.
     *
     * \return the settlements
     */
    const std::vector<Settlement*>& getSettlements() const
    {
        return this->settlements;
    }

    /**
     * Get the settlements as a QVariantList.
     *
     * This function is used as a read function for the mapNodes property and is
     * not supposed to be called from C++ code. Use CampaignMap::getSettlements()
     * instead.
     *
     * \returns the settlements
     */
    QVariantList readSettlements() const;

    /**
     * Set settlements.
     *
     * Will emit the signal CampaignMap::factionsChanged() if the newly set
     * value is different than the current one.
     * Destroys previous settlement objects.
     *
     * \param settlements the new settlements
     */
    void setSettlements(const std::vector<Settlement*>& settlements);

    /**
     * Get the units.
     *
     * \return get the units
     */
    const std::vector<Unit*>& getUnits() const
    {
        return this->units;
    }

    /**
     * Get the units as a QVariantList.
     *
     * This function is used as a read function for the mapNodes property and is
     * not supposed to be called from C++ code. Use CampaignMap::getUnits()
     * instead.
     *
     * \returns the units
     */
    QVariantList readUnits() const;

    /**
     * Set the units.
     *
     * Will emit the signal CampaignMap::unitsChanged() if the newly set
     * value is different than the current one.
     * Destroys previous unit objects.
     *
     * \param units the new units
     */
    void setUnits(const std::vector<Unit*>& units);

    /**
     * Get the armies.
     *
     * \return the armies
     */
    const std::vector<Army*>& getArmies() const
    {
        return this->armies;
    }

    /**
     * Get the armies as a QVariantList.
     *
     * This function is used as a read function for the mapNodes property and is
     * not supposed to be called from C++ code. Use CampaignMap::getArmies()
     * instead.
     *
     * \returns the armies
     */
    QVariantList readArmies() const;

    /**
     * Set the armies.
     *
     * Will emit the signal CampaignMap::armiesChanged() if the newly set
     * value is different than the current one.
     * Destroys previous army objects.
     *
     * \param armies the new armies
     */
    void setArmies(const std::vector<Army*>& armies);

    /**
     * Create a new map-node and add it to the map.
     *
     * The newly created map-node is assigned a unique objectName (in the
     * context of this map). It is created with the given terrain-type
     * and neighbours. Neighbour relations are updated for all neighbouring
     * map-nodes as well. This method does not check for conflicts in map-node
     * positions. If a map-node is created with the same neighbours as an
     * existing one, or it is created with neighbours that are not really near
     * than the map will get into an inconsistent state. It is the callers
     * responsability to make sure the new map-node is sane.
     *
     * \param terrainType the terrain-type of the to-be created map-node
     * \param neighbours the neighbour relations of the to-be created map-node
     *
     * \returns the new map-node allowing the caller to make further
     * modifications
     */
    MapNode* createMapNode(TerrainType* terrainType, const MapNodeNeighbours& neighbours);

    /**
     * Remove an exising map-node and renounce ownership.
     *
     * The map-node is removed and it's returned as an std::unique_ptr
     * and will be destroyed if the caller doesn't save it.
     * If the map-node is not found, nothing happens. The map-node will
     * loose all its neighbours. The map-nodes former neighbours are also
     * updated.
     *
     * \param mapNode the map-node to be removed
     *
     * \returns the removed map-node or an empty pointer if the settlement
     * was not found
     */
    std::unique_ptr<MapNode> removeMapNode(MapNode* mapNode);

    /**
     * Create a new settlement and add it to the map.
     *
     * The newly created settlement is assigned a unique objectName (in the
     * context of this map). It is created with the given settlement-type.
     *
     * \param settlementType the type of the settlement
     *
     * \returns the newly created settlement
     */
    Settlement* createSettlement(SettlementType* settlementType);

    /**
     * Remove the settlement and renounce ownership.
     *
     * The settlement is removed and it's returned as an std::unique_ptr and
     * will be destroyed if the caller doesn't save it.
     * If the settlement is not found, nothing happens.
     *
     * \param settlement the settlement to remove
     *
     * \returns the removed settlement or an empty pointer if the settlement
     * was not found
     */
    std::unique_ptr<Settlement> removeSettlement(Settlement* settlement);

    /**
     * Create a new unit and add it to the map.
     *
     * The newly created unit is assigned a unique objectName (in the context
     * of this map). It is created with the given unit-type.
     *
     * \param unitType the type of the unit
     *
     * \returns the newly created unit
     */
    Unit* createUnit(UnitType* unitType);

    /**
     * Remove the unit and renounce ownership.
     *
     * The unit is removed and it's returned as an std::unique_ptr and
     * will be destroyed if the caller doesn't save it.
     * If the unit is not found, nothing happens.
     *
     * \param unit the unit to remove
     *
     * \returns the removed unit or an empty pointer if the unit
     * was not found
     */
    std::unique_ptr<Unit> removeUnit(Unit* unit);

    /**
     * Create a new army and add it to the map.
     *
     * The newly created army is assigned a unique objectName (in the context
     * of this map). It is created with the given army-type.
     *
     * \param armyType the type of the army
     *
     * \returns the newly created army
     */
    Army* createArmy(ArmyType* armyType);

    /**
     * Remove the army and renounce ownership.
     *
     * The army is removed and it's returned as an std::unique_ptr and
     * will be destroyed if the caller doesn't save it.
     * If the army is not found, nothing happens.
     *
     * \param army the army to remove
     *
     * \returns the removed army or an empty pointer if the army
     * was not found
     */
    std::unique_ptr<Army> removeArmy(Army* army);

    /**
     * Create a new faction and add it to the map.
     *
     * The newly created faction is assigned a unique objectName and banner
     * (in the context of this map). It is created with the given civilization.
     *
     * \param civilization the civilization of the faction
     *
     * \returns the newly created faction
     */
    Faction* createFaction(Civilization* civilization);

    /**
     * Remove the faction and renounce ownership.
     *
     * The faction is removed and it's returned as an std::unique_ptr and
     * will be destroyed if the caller doesn't save it.
     * If the faction is not found, nothing happens.
     *
     * \param faction the faction to remove
     *
     * \returns the removed faction or an empty pointer if the faction
     * was not found
     */
    std::unique_ptr<Faction> removeFaction(Faction* faction);

    /**
     * Get all content of this map.
     *
     * Get a list of tuples with all the map-nodes and the settlements and
     * armies that are on it. Every item in the list is a tuple of a map-node
     * and a settlement and/or army that is on that map-node. All map-nodes are
     * in the content list. Settlements and armies however that don't have an
     * assigned position (map-node property is not set) won't be included.
     *
     * \returns the contents
     */
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
    /**
     * Emitted when the display-name changes.
     */
    void displayNameChanged();

    /**
     * Emitted when the world changes.
     */
    void worldChanged();

    /**
     * Emitted when the factions change.
     */
    void factionsChanged();

    /**
     * Emitted when the map-nodes change.
     */
    void mapNodesChanged();

    /**
     * Emitted when the settlements change.
     */
    void settlementsChanged();

    /**
     * Emitted when the units change.
     */
    void unitsChanged();

    /**
     * Emitted when the armies change.
     */
    void armiesChanged();

private:
    QString displayName;
    World* world;
    unsigned int mapNodeIndex;
    unsigned int settlementIndex;
    unsigned int unitIndex;
    unsigned int armyIndex;
    unsigned int factionIndex;
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
