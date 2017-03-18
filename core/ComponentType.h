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

#include <vector>

#include <QObject>
#include <QString>
#include <QVariant>

#include "core/Field.h"

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

    /**
     * The name of the component-type.
     */
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)

    /**
     * The fields of the component-type.
     */
    Q_PROPERTY(QVariantList fields READ readFields NOTIFY fieldsChanged)

public:
    /**
     * Constructs an empty ComponentType.
     *
     * \param parent the parent QObject.
     */
    ComponentType(QObject* parent = nullptr);

    /**
     * Is this component-type built-in?
     *
     * Built-in component-types are the ones that the engine's systems know
     * about. These are non-editable, with a pre-defined set of fields
     * and name.
     *
     * \return is the component-type built-in
     */
    virtual bool isBuiltIn() const = 0;

    /**
     * Get the name.
     *
     * \returns the name
     */
    virtual QString getName() const = 0;

    /**
     * Get the fields.
     *
     * \returns the fields
     */
    virtual std::vector<Field*> getFields() const = 0;

    /**
     * Get the fields as a QVariantList
     *
     * This function is used as a read function for the fields property and is
     * not supposed to be called from C++ code. Use ComponentType::getFields()
     * instead.
     *
     * \returns the fields
     */
    QVariantList readFields() const;

signals:
    /**
     * Emitted when the name changes.
     */
    void nameChanged();

    /**
     * Emitted when the property-names change.
     */
    void fieldsChanged();
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_COMPONENT_TYPE_H