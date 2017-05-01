/**
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

#include "core/EntityType.h"

#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

EntityType::EntityType(QObject* parent, int id)
    : WObject(parent, id)
{
}

void EntityType::setName(const QString& name)
{
    if (this->name != name)
    {
        this->name = name;
        emit nameChanged();
    }
}

void EntityType::addComponentType(ComponentType* componentType)
{
    if (std::find(this->componentTypes.cbegin(), this->componentTypes.cend(), componentType) !=
        this->componentTypes.cend())
        return;

    this->componentTypes.push_back(componentType);

    emit componentTypesChanged();
}

void EntityType::removeComponentType(ComponentType* componentType)
{
    auto it = std::remove(this->componentTypes.begin(), this->componentTypes.end(), componentType);

    if (it == this->componentTypes.end())
        return;

    this->componentTypes.erase(it);

    emit componentTypesChanged();
}

QVariantList EntityType::readComponentTypes() const
{
    return utils::toQVariantList(this->componentTypes);
}

} // namespace core
} // namespace warmonger
