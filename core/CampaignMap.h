/** \file
 * CampaignMap class.
 *
 * \copyright (C) 2015-2017 Botond Dénes
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

#include "core/Component.h"
#include "core/Entity.h"
#include "core/Faction.h"
#include "core/MapNode.h"
#include "core/World.h"

namespace warmonger {
namespace core {

/**
 * A campaign-map.
 *
 * The campaign-map is were the game actually take place. The campaign-map
 * contain the graph of map-nodes that form the map. It also contains the list
 * of factions and entities that participate in the game. A campaign-map is
 * linked to a world which defines all aspects of its behaviour.
 *
 * \see warmonger::core::Component
 * \see warmonger::core::Entity
 * \see warmonger::core::Faction
 * \see warmonger::core::MapNode
 * \see warmonger::core::World
 */
class CampaignMap : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(World* world READ getWorld WRITE setWorld NOTIFY worldChanged)
    Q_PROPERTY(QVariantList mapNodes READ readMapNodes NOTIFY mapNodesChanged)
    Q_PROPERTY(QVariantList factions READ readFactions NOTIFY factionsChanged)
    Q_PROPERTY(QVariantList entities READ readEntities NOTIFY entitiesChanged)

public:
    /**
     * Constructs an empty CampaignMap.
     *
     * \param parent the parent QObject.
     */
    explicit CampaignMap(QObject* parent = nullptr);

    /**
     * Get the display-name.
     *
     * \returns the displayName
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
     * Get the entities.
     *
     * \return the entities
     */
    const std::vector<Entity*>& getEntities() const
    {
        return this->entities;
    }

    /**
     * Get the entities as a QVariantList.
     *
     * This function is used as a read function for the mapNodes property and is
     * not supposed to be called from C++ code. Use CampaignMap::getEntitys()
     * instead.
     *
     * \returns the entities
     */
    QVariantList readEntities() const;

    /**
     * Add a map-node to the map.
     *
     * This method does not check for conflicts in map-node
     * positions. If a map-node is added with the same neighbours as an
     * existing one, or it is added with neighbours that are not really near
     * then the map will get into an inconsistent state. It is the caller's
     * responsability to make sure the added map-node is sane.
     * Will emit the signal CampaignMap::mapNodesChanged().
     *
     * \param mapNode the map-node
     */
    void addMapNode(std::unique_ptr<MapNode>&& mapNode);

    /**
     * Create a new map-node and add it to the map.
     *
     * The newly created map-node is assigned a unique objectName (in the
     * context of this map). It is created with the given and neighbours.
     * Neighbour relations are updated for all neighbouring
     * map-nodes as well. This method does not check for conflicts in map-node
     * positions. If a map-node is created with the same neighbours as an
     * existing one, or it is created with neighbours that are not really near
     * then the map will get into an inconsistent state. It is the caller's
     * responsability to make sure the new map-node is sane.
     * Will emit the signal CampaignMap::mapNodesChanged().
     *
     * \param neighbours the neighbour relations of the to-be created map-node
     *
     * \returns the new map-node allowing the caller to make further
     * modifications
     */
    MapNode* createMapNode(const MapNodeNeighbours& neighbours);

    /**
     * Remove an exising map-node and renounce ownership.
     *
     * The map-node is removed and it's returned as an std::unique_ptr
     * and will be destroyed if the caller doesn't save it.
     * If the map-node is not found, nothing happens. The map-node will
     * loose all its neighbours. The map-nodes former neighbours are also
     * updated.
     * Will emit the signal CampaignMap::mapNodesChanged().
     *
     * \param mapNode the map-node to be removed
     *
     * \returns the removed map-node or an empty pointer if the entity
     * was not found
     */
    std::unique_ptr<MapNode> removeMapNode(MapNode* mapNode);

    /**
     * Add the entity to the map.
     *
     * The map takes ownership over the entity.
     * Will emit the signal CampaignMap::entitiesChanged().
     *
     * \param entity the entity
     */
    void addEntity(std::unique_ptr<Entity>&& entity);

    /**
     * Create a new entity and add it to the map.
     *
     * The newly created entity is assigned a unique objectName (in the
     * context of this map). It is created with the given entity-type.
     *
     * \param entityType the type of the entity
     *
     * \returns the newly created entity
     */
    Entity* createEntity(EntityType* entityType);

    /**
     * Remove the entity and renounce ownership.
     *
     * The entity is removed and it's returned as an std::unique_ptr and
     * will be destroyed if the caller doesn't save it.
     * If the entity is not found, nothing happens.
     *
     * \param entity the entity to remove
     *
     * \returns the removed entity or an empty pointer if the entity
     * was not found
     */
    std::unique_ptr<Entity> removeEntity(Entity* entity);

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
     * Add the faction to the map.
     *
     * The map takes ownership over the faction.
     * Will emit the signal CampaignMap::factionsChanged().
     *
     * \param faction the faction
     */
    void addFaction(std::unique_ptr<Faction>&& faction);

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
     * Emitted when the entities change.
     */
    void entitiesChanged();

private:
    QString displayName;
    World* world;
    unsigned int mapNodeIndex;
    unsigned int entityIndex;
    unsigned int factionIndex;
    std::vector<Faction*> factions;
    std::vector<MapNode*> mapNodes;
    std::vector<Entity*> entities;
};

} // namespace core
} // namespace warmonger

#endif // CORE_CAMPAIGN_MAP_H
