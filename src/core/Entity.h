/** \file
 * Entity class.
 *
 * \copyright (C) 2015-2018 Botond Dénes
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

class WorldRules;

/**
 * A game entity.
 *
 * For an overview of the Entity-Component-Systems design pattern (ECS) and how
 * warmonger implements it see \ref docs/ECS.md.
 *
 * Entities have a set of components, The components of an entity defines how it
 * interacts with the systems. An entity can have only one component of a given
 * type so retrieving them is possible by type or name.
 *
 * \see warmonger::core::Component
 * \see warmonger::core::World
 */
class Entity : public WObject, public ir::Serializable
{
    Q_OBJECT

    Q_PROPERTY(QString name READ getName CONSTANT)

public:
    /**
     * Create an empty entity.
     *
     * \param name the name of the entity
     * \param world-rules the world-rules of the world
     * \param parent the parent QObject.
     * \param id the id of the object
     *
     * \see WObject::WObject
     */
    Entity(QString name, WorldRules* rules, QObject* parent, ObjectId id = ObjectId::Invalid);

    /**
     * Construct the entity from the intermediate-representation.
     *
     * Unserializing constructor.
     *
     * \param v the intermediate-representation
     * \param world-rules the world-rules of the world
     * \param parent the parent QObject.
     */
    Entity(ir::Value v, WorldRules* rules, QObject* parent);

    ir::Value serialize() const override;

    QString getName() const
    {
        return this->name;
    }

    /**
     * The parent entity of this entity.
     *
     * \see setParentEntity()
     */
    Entity* getParentEntity() const
    {
        return this->parentEntity;
    }

    bool hasParentEntity() const
    {
        return this->parentEntity != nullptr;
    }

    /**
     * Set the parent entity of this entity.
     *
     * Allows for organizing entities into a hierarchy. This has an effect on
     * how the entity's components are interpreted:
     * * PositionComponent is inherited from parent but can be overriden.
     * * GraphicsComponent is rendered as a child of the parent's GraphicsComponent
     *
     * The effect on the components defined by the world-rules is, naturally,
     * defined by the world rules.
     *
     * \param entity the new parent entity of this entity, can be null.
     *
     * This entity is added to the parent's children list.
     * \see \ref getChildEntities().
     */
    void setParentEntity(Entity* entity);

    /**
     * Get the list of entities this entity is a parent of.
     */
    const std::vector<Entity*>& getChildEntities() const
    {
        return this->childEntities;
    }

    /**
     * Get the component with the given name of this entity.
     *
     * \return the component or nullptr if entity doesn't have the
     * component.
     */
    Component* getComponent(const QString& name);

    /**
     * Get the position-component of this entity.
     *
     * Convenience getter for C++ code.
     *
     * \returns the position-component or nullptr if it doesn't have it.
     */
    PositionComponent* getPositionComponent()
    {
        if (auto* c = this->getComponent(PositionComponent::name))
            return qobject_cast<PositionComponent*>(c);
        else if (this->parentEntity)
            return this->parentEntity->getPositionComponent();
        else
            return nullptr;
    }

    /**
     * Get the graphics-component of this entity.
     *
     * Convenience getter for C++ code.
     *
     * \returns the graphics-component or nullptr if it doesn't have it
     */
    GraphicsComponent* getGraphicsComponent()
    {
        return qobject_cast<GraphicsComponent*>(this->getComponent(GraphicsComponent::name));
    }

    /**
     * Create a new component with the given name.
     *
     * The Entity takes ownership of the created component. If the Entity
     * already has a component with the given name it's replaced with the
     * new component.
     * Will emit Entity::componentsChanged().
     *
     * \returns the newly created component
     *
     * \throws ValueError if cannot create a component with said name.
     * This depends on the world-rules, it can refuse to create a
     * component.
     */
    Component* createComponent(const QString& name);

    /**
     * Remove the component with the given name.
     *
     * If the Entity doesn't have a component with the given type nothing is
     * done. Either way the result of this method is that the Entity won't have
     * and entity with the given name.
     *
     * \returns the removed component or nullptr if no component was removed
     */
    std::unique_ptr<Component> removeComponent(const QString& name);

    /**
     * Get the components.
     *
     * \return the components
     */
    std::vector<Component*> getComponents() const;

signals:
    /**
     * Emitted when one of the components change.
     */
    void componentsChanged();

private:
    QString name;
    WorldRules* rules;
    std::unordered_map<QString, Component*> components;
    Entity* parentEntity{nullptr};
    std::vector<Entity*> childEntities;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_ENTITY_H
