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

#include <unordered_map>
#include <vector>

#include <QString>

#include "core/Field.h"
#include "core/WObject.h"
#include "utils/Hash.h"

namespace warmonger {
namespace core {

class ComponentType;

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
    template <class Visitor>
    static auto describe(Visitor&& visitor)
    {
        return visitor.template visitParent<WObject>()
            .visitMember("type", &Component::getType, &Component::setType)
            .visitMember("fields", &Component::getFields, &Component::setFields)
            .template visitConstructor<QObject*, int>("parent", "id");
    }

    /**
     * Create an empty component.
     *
     * \param parent the parent QObject.
     * \param id the id
     *
     * \see WObject::WObject
     */
    Component(QObject* parent, int id = WObject::invalidId);

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
     * Get the field value with the given name.
     *
     * If this component doesn't have a field with the given name a nullptr
     * will be returned.
     * TODO: Consider using std::string_view as the key, as QString
     * conversion to-from char* is expensive.
     *
     * \param name the name of the field
     *
     * \returns the property value
     */
    FieldValue* field(const QString& name);

    const FieldValue* field(const QString& name) const;

    /**
     * Get all fields of this component.
     */
    const std::unordered_map<QString, FieldValue>& getFields() const
    {
        return this->fields;
    }

    /**
     * Set all fields of this component.
     *
     * All previous fields are discarded.
     * Will emit fieldsChanged().
     */
    void setFields(std::unordered_map<QString, FieldValue> fields);

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
    std::unordered_map<QString, FieldValue> fields;
};

/**
 * Convenience wrapper over Component for C++ users.
 *
 * C++ users will almost always work with built-in component-types,
 * hence we can assume that lookups will always succeed and dereference
 * results unconditionally.
 */
class ComponentWrapper
{
public:
    ComponentWrapper(Component* component)
        : component(component)
    {
    }

    ComponentType* getType() const
    {
        return this->component->getType();
    }

    void setType(ComponentType* type)
    {
        this->component->setType(type);
    }

    FieldValue& operator[](const QString& name)
    {
        return *this->component->field(name);
    }

    const FieldValue& operator[](const QString& name) const
    {
        return *this->component->field(name);
    }

    bool operator!() const
    {
        return !this->component;
    }

    Component* wrapped()
    {
        return component;
    }

private:
    Component* component;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_COMPONENT_TYPE_H
