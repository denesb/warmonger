/** \file
 * Map class.
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

#ifndef CORE_MAP_H
#define CORE_MAP_H

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
class Map : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(World* world READ getWorld WRITE setWorld NOTIFY worldChanged)
    Q_PROPERTY(QVariantList mapNodes READ readMapNodes NOTIFY mapNodesChanged)
    Q_PROPERTY(QVariantList factions READ readFactions NOTIFY factionsChanged)
    Q_PROPERTY(QVariantList entities READ readEntities NOTIFY entitiesChanged)

public:
    template <class Visitor>
    static auto describe(Visitor&& visitor)
    {
        return visitor.template visitParent<QObject>()
            .visitMember("name", &Map::getName)
            .visitMember("world", &Map::getWorld)
            .visitMember("mapNodes", &Map::getMapNodes)
            .visitMember("factions", &Map::getFactions)
            .visitMember("entities", &Map::getEntities);
    }

    /**
     * Constructs an empty Map.
     *
     * \param parent the parent QObject.
     */
    explicit Map(QObject* parent = nullptr);

    /**
     * Get the name.
     *
     * \returns the name
     */
    const QString& getName() const
    {
        return this->name;
    }

    /**
     * Set the name.
     *
     * Will emit the signal Map::nameChanged() if the newly set
     * value is different than the current one.
     *
     * \param name the new name
     */
    void setName(const QString& name);

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
     * Will emit the signal Map::worldChanged() if the newly set value
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
     * not supposed to be called from C++ code. Use Map::getMapNodes()
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
     * not supposed to be called from C++ code. Use Map::getFactions()
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
     * not supposed to be called from C++ code. Use Map::getEntitys()
     * instead.
     *
     * \returns the entities
     */
    QVariantList readEntities() const;

    /**
     * Create a new map-node and add it to the map.
     *
     * The map takes ownership of the created object.
     * Will emit the signal Map::mapNodesChanged().
     * An id value should only be passed when the factions is being
     * unserialized and it already has a priorly generated id.
     *
     * \param id the id
     *
     * \returns the new map-node
     */
    MapNode* createMapNode(int id = WObject::invalidId);

    /**
     * Remove an exising map-node and renounce ownership.
     *
     * The map-node is removed and it's returned as an std::unique_ptr
     * and will be destroyed if the caller doesn't save it.
     * If the map-node is not found, nothing happens. The map-node will
     * loose all its neighbours. The map-nodes former neighbours are also
     * updated.
     * Will emit the signal Map::mapNodesChanged().
     *
     * \param mapNode the map-node to be removed
     *
     * \returns the removed map-node or an empty pointer if the entity
     * was not found
     */
    std::unique_ptr<MapNode> removeMapNode(MapNode* mapNode);

    /**
     * Create a new entity and add it to the map.
     *
     * The map takes ownership of the created object.
     * Will emit the signal Map::entitiesChanged().
     * An id value should only be passed when the entity is being
     * unserialized and it already has a priorly generated id.
     *
     * \param id the id
     *
     * \returns the newly created entity
     */
    Entity* createEntity(int id = WObject::invalidId);

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
     * The map takes ownership of the created object.
     * Will emit the signal Map::factionsChanged().
     * An id value should only be passed when the factions is being
     * unserialized and it already has a priorly generated id.
     *
     * \param id the id
     *
     * \returns the newly created faction
     */
    Faction* createFaction(int id = WObject::invalidId);

    /**
     * Add a new faction to the map.
     *
     * The map must already own this faction, i.e. it must have been
     * created with the world as its parent.
     * Will emit the signal Map::factionsChanged().
     *
     * \returns the added faction
     */
    Faction* addFaction(std::unique_ptr<Faction> faction);

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
     * Generate a hexagonal map with the given radius.
     *
     * The radius is the number of map-nodes that make the shortest path from
     * the central map-node to any outermost one.
     * Generating map-nodes discards all existing map-nodes and will generate
     * new ones!
     *
     * \param radius the radius of the map
     */
    void generateMapNodes(unsigned int radius);

signals:
    /**
     * Emitted when the name changes.
     */
    void nameChanged();

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
    QString name;
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

#endif // CORE_MAP_H
