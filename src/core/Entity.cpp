/** \file
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

#include "core/Entity.h"

#include "core/WorldRules.h"
#include "utils/Format.h"
#include "utils/Logging.h"

namespace warmonger {
namespace core {

Entity::Entity(QString name, WorldRules* rules, QObject* parent, ObjectId id)
    : WObject(parent, id)
    , name(std::move(name))
    , rules(rules)
{
}

Entity::Entity(ir::Value v, WorldRules* rules, QObject* parent)
    : WObject(parent, v.getObjectId())
    , rules(rules)
{
    auto obj = std::move(v).asObject();

    this->name = std::move(obj["name"]).asString();
    this->setObjectName(this->name);

    auto serializedComponents = std::move(obj["components"]).asMap();

    for (auto& c : serializedComponents)
    {
        if (c.first == PositionComponent::name)
        {
            this->components.emplace(c.first, new PositionComponent(std::move(c.second), this));
        }
        else if (c.first == GraphicsComponent::name)
        {
            this->components.emplace(c.first, new GraphicsComponent(std::move(c.second), this));
        }
        else
        {
            this->components.emplace(c.first, rules->createComponent(std::move(c.second), this).release());
        }
    }
}

ir::Value Entity::serialize() const
{
    std::unordered_map<QString, ir::Value> obj;

    obj["id"] = this->getId().get();

    std::unordered_map<QString, ir::Value> componentsMap;

    for (auto& c : this->components)
    {
        componentsMap.emplace(c.first, c.second->serialize());
    }

    obj["components"] = std::move(componentsMap);

    return obj;
}

void Entity::setParentEntity(Entity* entity)
{
    if (this->parentEntity)
    {
        auto it = std::remove(this->parentEntity->childEntities.begin(), this->parentEntity->childEntities.end(), this);
        if (it == this->parentEntity->childEntities.end())
        {
            wWarning.format("Supposed to remove this entity ({}) from parent's ({}) children but not found", *this, *this->parentEntity);
        }
        this->parentEntity->childEntities.erase(it, this->parentEntity->childEntities.end());
    }

    this->parentEntity = entity;

    if (this->parentEntity)
        entity->childEntities.emplace_back(this);
}

Component* Entity::getComponent(const QString& name)
{
    const auto it = this->components.find(name);

    if (it == this->components.end())
    {
        wTrace.format("Entity {} doesn't have a component with the name `{}'", *this, name);
        return nullptr;
    }
    else
    {
        return it->second;
    }
}

Component* Entity::createComponent(const QString& name)
{
    wTrace.format("Creating new component `{}' in entity {}", name, *this);

    Component* component{nullptr};
    if (name == PositionComponent::name)
    {
        component = new PositionComponent(this);
    }
    else if (name == GraphicsComponent::name)
    {
        component = new GraphicsComponent(this);
    }
    else
    {
        component = rules->createComponent(name, this).release();
    }

    const auto it = this->components.find(name);

    if (it == this->components.end())
    {
        this->components.emplace(name, component);
    }
    else
    {
        wTrace.format("Overwriting previously existing component with new one");
        delete it->second;
        it->second = component;
    }

    emit componentsChanged();

    return component;
}

std::unique_ptr<Component> Entity::removeComponent(const QString& name)
{
    const auto it = this->components.find(name);

    if (it == this->components.end())
    {
        return std::unique_ptr<Component>();
    }
    else
    {
        auto component = it->second;
        this->components.erase(it);

        wTrace.format("Removed component `{}' from entity {}", name, *this);

        emit componentsChanged();

        return std::unique_ptr<Component>(component);
    }
}

std::vector<Component*> Entity::getComponents() const
{
    std::vector<Component*> c;
    std::transform(this->components.begin(),
        this->components.end(),
        std::back_inserter(c),
        [](const std::pair<QString, Component*>& e) { return e.second; });
    return c;
}

} // namespace core
} // namespace warmonger
