/** \file
 * EntityType class.
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
#ifndef W_CORE_ENTITY_TYPE_H
#define W_CORE_ENTITY_TYPE_H

#include "core/ComponentType.h"

namespace warmonger {
namespace core {

/**
 * The type of an entity.
 *
 * EntityType provides a way to categorise entities based on their role
 * and purpose. It defines the type of components it has, this cannot be
 * changed once the type object is created. This allows world developers
 * to define all the different kind of entities that exists in their world
 * up front.
 *
 * \see core::warmonger::Component
 * \see core::warmonger::ComponentType
 * \see core::warmonger::Entity
 * \see core::warmonger::World
 */
class EntityType
{
public:
    /**
     * Create a new entity-type with the given name and component-types.
     *
     * \param name the name
     * \param componentTypes the types of components this entity-type has
     */
    EntityType(const QString& name, const std::vector<ComponentType*>& componentTypes);

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
     * Get the component-types.
     *
     * \returns the component-types
     */
    const std::vector<ComponentType*>& getComponentTypes() const
    {
        return this->componentTypes;
    }

private:
    QString name;
    std::vector<ComponentType*> componentTypes;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_ENTITY_TYPE_H
