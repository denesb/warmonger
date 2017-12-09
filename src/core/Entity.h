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

namespace warmonger {
namespace core {

/**
 * A game entity.
 *
 * For an overview of the Entity-Component-Systems design pattern (ECS) and how
 * warmonger implements it see \ref docs/ECS.md.
 *
 * Entities have a set of components, The components of an entity defines how it
 * interacts with the systems.  An entity can have only one component of a given
 * type so retrieving them is possible by type or name.
 *
 * \see warmonger::core::Component
 * \see warmonger::core::ComponentType
 * \see warmonger::core::World
 */
class Entity : public WObject
{
    Q_OBJECT

public:
    template <class Visitor>
    static auto describe(Visitor&& visitor)
    {
        return visitor.template visitParent<WObject>().visitMember("components", &Entity::getComponents);
    }

    /**
     * Create an empty entity.
     *
     * \param parent the parent QObject
     * \param id the id
     *
     * \see WObject::WObject
     */
    Entity(QObject* parent, int id = WObject::invalidId);

    /**
     * Get the component with the given type
     *
     * Changing the component will trigger the Entity::componentChanged()
     * signal.
     *
     * \param componentType the component-type
     *
     * \return the component or nullptr if entity doesn't have componentType
     */
    Component* getComponent(const ComponentType* const componentType);

    /**
     * Get the component with the given type by it's name
     *
     * Changing the component will trigger the Entity::componentChanged()
     * signal.
     *
     * \param componentTypeName the component-type's name
     *
     * \return the component or nullptr if entity doesn't have componentType
     */
    Component* getComponent(const QString& componentTypeName);

    /**
     * Create a new component with the given type.
     *
     * The Entity takes ownership of the created component. If the Entity
     * already has a component with the given type it's replaced with the
     * new component.
     * Will emit Entity::componentsChanged().
     *
     * \param componentType the type of the component
     *
     * \returns the newly created component
     */
    Component* createComponent(ComponentType* const componentType);

    /**
     * Remove the component with the given type.
     *
     * If the Entity doesn't have a component with the given type nothing is
     * done. Either way tha result of this method is that the Entity won't have
     * and entity with the given type.
     *
     * \param componentType the type of the component
     *
     * \returns the removed component or nullptr if no component was removed
     */
    std::unique_ptr<Component> removeComponent(const ComponentType* const componentType);

    /**
     * Remove the component with the given type by it's name.
     *
     * If the Entity doesn't have a component with the given type nothing is
     * done. Either way tha result of this method is that the Entity won't have
     * and entity with the given type.
     *
     * \param componentType the type of the component
     *
     * \returns the removed component or nullptr if no component was removed
     */
    std::unique_ptr<Component> removeComponent(const QString& componentTypeName);

    /**
     * Get the components.
     *
     * \return the components
     */
    const std::vector<Component*>& getComponents() const
    {
        return this->components;
    }

signals:
    /**
     * Emitted when one of the components change.
     */
    void componentChanged();

private:
    std::vector<Component*> components;
};

/**
 * Convenience wrapper over Entity for C++ users.
 */
class EntityWrapper
{
public:
    EntityWrapper(Entity* entity)
        : entity(entity)
    {
    }

    ComponentWrapper operator[](const ComponentType* const componentType)
    {
        return this->entity->getComponent(componentType);
    }

    ComponentWrapper operator[](const QString& componentTypeName)
    {
        return this->entity->getComponent(componentTypeName);
    }

    bool hasComponent(const QString& componentTypeName)
    {
        return this->entity->getComponent(componentTypeName);
    }

    ComponentWrapper createComponent(ComponentType* const componentType)
    {
        return this->entity->createComponent(componentType);
    }

    std::unique_ptr<Component> removeComponent(const ComponentType* const componentType)
    {
        return this->entity->removeComponent(componentType);
    }

    std::unique_ptr<Component> removeComponent(const QString& componentTypeName)
    {
        return this->entity->removeComponent(componentTypeName);
    }

    const std::vector<Component*>& getComponents() const
    {
        return this->entity->getComponents();
    }

    Entity* wrapped()
    {
        return this->entity;
    }

private:
    Entity* entity;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_ENTITY_H
