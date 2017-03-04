/** \file
 * ComponentType interface.
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
#ifndef W_CORE_COMPONENT_TYPE_H
#define W_CORE_COMPONENT_TYPE_H

#include <memory>
#include <vector>

#include <QObject>
#include <QString>
#include <QVariant>

namespace warmonger {
namespace core {

/**
 * The type of a component.
 *
 * For an overview of the Entity-Component-Systems design pattern (ECS) and how
 * warmonger implements it see \ref docs/ECS.md.
 * The component-type defines the set of properties a component has. Properties
 * have a name and a value and some metadata, like their type and
 * default-value.
 *
 * \see warmonger::core::Component
 */
class ComponentType : public QObject
{
    Q_OBJECT

public:
    /**
     * Get the name.
     *
     * \returns the name
     */
    virtual const QString& getName() const = 0;

    /**
     * Get the property names
     *
     * \returns the property names
     */
    virtual std::vector<QString> getPropertyNames() const = 0;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_COMPONENT_TYPE_H
