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

#include <QString>
#include <QVariant>

#include "core/ComponentType.h"
#include "core/WObject.h"

namespace warmonger {
namespace core {

/**
 * The component of a game entity.
 *
 * For an overview of the Entity-Component-Systems design pattern (ECS) and how
 * warmonger implements it see \ref docs/ECS.md.
 *
 * Components have a set of fields defined by their component-type.
 *
 * \see warmonger::core::ComponentType
 */
class Component : public WObject
{
    Q_OBJECT

    /**
     * The type of the component
     */
    Q_PROPERTY(ComponentType* type READ getType NOTIFY typeChanged)

public:
    /**
     * Create an empty component with the given id.
     *
     * \param id the id of the object
     */
    explicit Component(long id);

    /**
     * Create an empty component with the given parent.
     *
     * \param parent the parent QObject.
     */
    explicit Component(QObject* parent = nullptr);

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
     * Set the type.
     *
     * Will emit the signal Component::componentChanged() if the newly set value
     * is different than the current one.
     *
     * \param type the type
     */
    void setType(ComponentType* type);

    /**
     * Get the field wth the given name.
     *
     * If this component doesn't have a field with the given name an invalid
     * QVariant will be returned.
     *
     * \param name the name of the field
     *
     * \returns the property value
     */
    QVariant getField(const QString& name) const;

    /**
     * Set the field with the given name.
     *
     * Will emit the signal Component::fieldChanged() if the newly set value
     * is different than the current one.
     *
     * \param name the name of the field
     * \param value the new value of the field
     */
    void setField(const QString& name, const QVariant& value);

signals:
    /**
     * Emitted when the type changes.
     */
    void typeChanged();

    /**
     * Emitted when a field's value changes.
     */
    void fieldChanged();

private:
    ComponentType* type;
    std::map<const QString, QVariant> fields;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_COMPONENT_TYPE_H
