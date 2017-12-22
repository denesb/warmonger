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

#include <memory>

#include "core/ComponentType.h"

namespace warmonger {
namespace core {

class WorldRules;

/**
 * Component-type defined by the world.
 *
 * For an overview of the Entity-Component-Systems design pattern (ECS) and how
 * warmonger implements it see \ref docs/ECS.md.
 * World component-type is a dynamic component-type (as opposed to built-in)
 * created at runtime, defined by the loaded world. These component-types are
 * used solely by world systems and are ignored by all built-in systems.
 *
 * \see warmonger::core::ComponentType
 */
class WorldComponentType : public ComponentType
{
    Q_OBJECT

public:
    template <class Visitor>
    static auto describe(Visitor&& visitor)
    {
        return visitor.template visitParent<WObject>()
            .visitMember("name", &WorldComponentType::getName, &WorldComponentType::setName)
            .visitMember("fields", &WorldComponentType::getFields, &WorldComponentType::addField)
            .template visitConstructor<QObject*, int>("parent", "id");
    }

    /**
     * Constructs an empty WorldComponentType.
     *
     * \param parent the parent QObject.
     * \param id the id
     *
     * \see WObject::WObject
     */
    WorldComponentType(QObject* parent, long id = WObject::invalidId);

    /**
     * Set the name.
     *
     * Will emit the signal ComponentType::nameChanged() if the newly set
     * value is different than the current one.
     *
     * \param name the new name
     */
    void setName(const QString& name);

    /**
     * Create a new field.
     *
     * The WorldComponentType takes ownership over the field.
     * Will emit the signal ComponentType::fieldsChanged().
     *
     * \return the new field
     */
    Field* createField();

    /**
     * Add a new field.
     *
     * The WorldComponentType must already own this field.
     * Will emit the signal ComponentType::fieldsChanged().
     *
     * \return the added field
     */
    Field* addField(std::unique_ptr<Field> field);

    /**
     * Remove an exising field and renounce ownership.
     *
     * The field is removed and it's returned as an std::unique_ptr
     * and will be destroyed if the caller doesn't save it.
     * If the field is not found, nothing happens.
     * Will emit the signal WorldComponentType::fieldsChanged().
     *
     * \param field the field to be removed
     *
     * \returns the removed field or an empty pointer if the entity
     * was not found
     */
    std::unique_ptr<Field> removeField(Field* field);

    /**
     * Set the world rules.
     *
     * It is used for creating the appropriate components for the rules.
     * TODO: pass it in the constructor once unserializer is fixed.
     */
    void setWorldRules(WorldRules* worldRules);

    bool isBuiltIn() const override
    {
        return false;
    }

    QString getName() const override
    {
        return this->name;
    }

    std::vector<Field*> getFields() const override
    {
        return fields;
    }

    std::unique_ptr<Component> createComponent(int id = WObject::invalidId) override;

private:
    QString name;
    std::vector<Field*> fields;
    WorldRules* worldRules;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_WORLD_COMPONENT_TYPE_H
