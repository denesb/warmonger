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
#include "core/WObject.h"

namespace warmonger {
namespace core {

/**
 * The type of an entity.
 *
 * For an overview of the Entity-Component-Systems design pattern (ECS) and how
 * warmonger implements it see \ref docs/ECS.md.
 * EntityType provides a way to categorise entities based on their role
 * and purpose. It defines the type of components it has, this cannot be
 * changed once the type object is created. This allows world developers
 * to define all the different kind of entities that exists in their world
 * up front.
 *
 * \see warmonger::core::Component
 * \see warmonger::core::ComponentType
 * \see warmonger::core::Entity
 * \see warmonger::core::World
 */
class EntityType : public WObject
{
    Q_OBJECT

    /**
     * The name of the entity-type
     */
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)

    /**
     * The types of component this entity-type can have
     */
    Q_PROPERTY(QVariantList componentTypes READ readComponentTypes NOTIFY componentTypesChanged)

public:
    /**
     * Construct an empty entity-type.
     *
     * \param parent the parent QObject
     * \param id the id
     *
     * \see WObject::WObject
     */
    EntityType(QObject* parent, long id = WObject::invalidId);

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
     * Will emit the signal EntityType::nameChanged() if the newly set value
     * is different than the current one.
     *
     * \param name the new name
     */
    void setName(const QString& name);

    /**
     * Get the component-types.
     *
     * \returns the component-types
     */
    const std::vector<ComponentType*>& getComponentTypes() const
    {
        return this->componentTypes;
    }

    /**
     * Get the component-types as a QVariantList.
     *
     * This function is used as a read function for the mapNodes property and is
     * not supposed to be called from C++ code. Use EntityType::getComponentTypes()
     * instead.
     *
     * \returns the component-types
     */
    QVariantList readComponentTypes() const;

    /**
     * Add the component-type.
     *
     * Will emit the signal EntityType::componentTypesChanged().
     * Has no effect if the entity-type already has the component-type.
     *
     * \param component-type the new component-type
     */
    void addComponentType(ComponentType* componentType);

    /**
     * Remove the component-type.
     *
     * Will emit the signal EntityType::componentTypesChanged().
     * Has no effect if the entity-type doesn't has the component-type.
     *
     * \param component-type the component-type
     */
    void removeComponentType(ComponentType* componentType);

signals:
    /**
     * Emitted when the name changes.
     */
    void nameChanged();

    /**
     * Emitted when the component-types change.
     */
    void componentTypesChanged();

private:
    QString name;
    std::vector<ComponentType*> componentTypes;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_ENTITY_TYPE_H
