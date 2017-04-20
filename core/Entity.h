/** \file
 * Entity class.
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
#ifndef W_CORE_ENTITY_H
#define W_CORE_ENTITY_H

#include <map>
#include <memory>

#include "core/Component.h"
#include "core/EntityType.h"
#include "core/WObject.h"

namespace warmonger {
namespace core {

/**
 * A game entity.
 *
 * For an overview of the Entity-Component-Systems design pattern (ECS) and how
 * warmonger implements it see \ref docs/ECS.md.
 *
 * Entities have a set of components, defined by their entity-types. The
 * components of an entity defines how it interacts with the systems.
 *
 * \see warmonger::core::Component
 * \see warmonger::core::ComponentType
 * \see warmonger::core::EntityType
 * \see warmonger::core::World
 */
class Entity : public WObject
{
    Q_OBJECT

public:
    /**
     * Create an empty entity.
     *
     * \param parent the parent QObject
     * \param id the id
     *
     * \see WObject::WObject
     */
    Entity(QObject* parent, long id = WObject::invalidId);

    /**
     * Get the type
     *
     * \returns the entity-type
     */
    EntityType* getType() const
    {
        return this->type;
    }

    /**
     * Set the type.
     *
     * If the type is changed all current components are discarded and new ones
     * are created according to the new type.
     * Will emit the signal Entity::typeChanged() if the newly set value
     * is different than the current one.
     *
     * \param type the type
     */
    void setType(EntityType* type);

    /**
     * Get the component with the given type
     *
     * An entity can have only one component of a given type so retrieving them
     * is possible by type.
     * Changing the component will trigger the Entity::componentChanged() signal.
     *
     * \param componentType the component-type
     *
     * \return the component or nullptr if entity doesn't have componentType
     */
    Component* operator[](const ComponentType* const componentType);

    /**
     * Get the component with the given type by it's name
     *
     * An entity can have only one component of a given type so retrieving them
     * is possible by type.
     * Changing the component will trigger the Entity::componentChanged() signal.
     *
     * \param componentTypeName the component-type's name
     *
     * \return the component or nullptr if entity doesn't have componentType
     */
    Component* operator[](const QString& componentTypeName);

signals:
    /**
     * Emitted when the type changes.
     */
    void typeChanged();

    /**
     * Emitted when one of the components change.
     */
    void componentChanged();

private:
    EntityType* type;
    std::map<const ComponentType*, std::unique_ptr<Component>> components;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_ENTITY_H
