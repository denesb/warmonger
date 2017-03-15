/** \file
 * Component class.
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
#ifndef W_CORE_COMPONENT_H
#define W_CORE_COMPONENT_H

#include <vector>

#include <QObject>
#include <QString>
#include <QVariant>

#include "core/ComponentType.h"

namespace warmonger {
namespace core {

/**
 * The component of a game entity.
 *
 * For an overview of the Entity-Component-Systems design pattern (ECS) and how
 * warmonger implements it see \ref docs/ECS.md.
 *
 * Components have a set of properties defined by their component-type.
 *
 * TODO: QVariant's do not allow for value change tracking,
 * need a better solution.
 *
 * \see warmonger::core::ComponentType
 */
class Component : public QObject
{
    Q_OBJECT

public:
    /**
     * Create a component with the given type.
     *
     * \param type the type
     */
    explicit Component(ComponentType* type, QObject* parent = nullptr);

    /**
     * Get the type.
     *
     * \returns the type
     */
    ComponentType* getType() const
    {
        return this->type;
    }

    /**
     * Get the property wth the given name.
     *
     * If this component doesn't have a property with the given name an invelid
     * QVariant will be returned.
     *
     * \param name the name of the property
     *
     * \returns the property value
     */
    QVariant& operator[](const QString& name);

signals:
    /**
     * Emitted when a property's value changes.
     */
    void propertyChanged();

private:
    ComponentType* type;
    std::map<const QString, QVariant> properties;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_COMPONENT_TYPE_H
