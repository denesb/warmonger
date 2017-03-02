/** \file
 * WorldComponentType class.
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
#ifndef W_CORE_WORLD_COMPONENT_TYPE_H
#define W_CORE_WORLD_COMPONENT_TYPE_H

#include "core/ComponentType.h"

namespace warmonger {
namespace core {

/**
 * The type of a component.
 *
 * For an overview of the Entity-Component-Systems design pattern (ECS) and how
 * warmonger implements it see \ref docs/ECS.md.
 * Component-type used by worlds. This is a dynamic component-type created at
 * runtime, defined by the loaded world.
 *
 * \see warmonger::core::ComponentType
 */
class WorldComponentType : public ComponentType
{
public:
    /**
     * Create a world component-type with the name and properties.
     *
     * \param name the name
     * \param propertyNames the names of the properties
     */
    WorldComponentType(const QString& name, const std::vector<QString>& propertyNames);

    const QString& getName() const override
    {
        return this->name;
    }

    std::vector<QString> getPropertyNames() const override
    {
        return this->propertyNames;
    }

private:
    QString name;
    std::vector<QString> propertyNames;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_WORLD_COMPONENT_TYPE_H
