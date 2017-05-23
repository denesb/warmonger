/** \file
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

#include "core/Entity.h"

#include "utils/Logging.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

Entity::Entity(QObject* parent, int id)
    : WObject(parent, id)
    , type(nullptr)
{
}

void Entity::setType(EntityType* type)
{
    if (this->type == type)
        return;

    this->type = type;

    qDeleteAll(this->components.begin(), this->components.end());
    this->components.clear();

    for (const auto& componentType : this->type->getComponentTypes())
    {
        auto component = new Component(this);
        component->setType(componentType);

        QObject::connect(component, &Component::fieldChanged, this, &Entity::componentChanged);

        this->components.push_back(component);
    }

    emit typeChanged();
}

Component* Entity::getComponent(const ComponentType* const componentType)
{
    const auto it = std::find_if(this->components.begin(),
        this->components.end(),
        [componentType](const auto& component) { return component->getType() == componentType; });

    if (it == this->components.end())
    {
        wDebug << "Entity " << this << " doesn't have a component of type " << componentType->getName();
        return nullptr;
    }
    else
    {
        return *it;
    }
}

Component* Entity::getComponent(const QString& componentTypeName)
{
    const auto it = std::find_if(this->components.begin(),
        this->components.end(),
        [&componentTypeName](const auto& component) { return component->getType()->getName() == componentTypeName; });

    if (it == this->components.end())
    {
        wDebug << "Entity " << this << " doesn't have a component of type " << componentTypeName;
        return nullptr;
    }
    else
    {
        return *it;
    }
}

} // namespace core
} // namespace warmonger
