/**
 * Copyright (C) 2015-2017 Botond Dénes
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

#include "core/BuiltInComponentTypes.h"

#include "utils/Logging.h"

namespace warmonger {
namespace core {

template <class T>
struct ConcreteComponentTypeFactory
{
    static std::tuple<QString, std::function<ComponentType*(QObject*, ObjectId)>> create()
    {
        return std::make_tuple(T::staticMetaObject.className(), ConcreteComponentTypeFactory<T>());
    }

    ComponentType* operator()(QObject* parent, ObjectId id) const
    {
        return new T(parent, id);
    }
};

std::vector<std::tuple<QString, std::function<ComponentType*(QObject*, ObjectId)>>> getBuiltInComponentTypesFactories()
{
    return std::vector<std::tuple<QString, std::function<ComponentType*(QObject*, ObjectId)>>>{
        ConcreteComponentTypeFactory<PositionComponentType>::create(),
        ConcreteComponentTypeFactory<EditComponentType>::create(),
        ConcreteComponentTypeFactory<GraphicsComponentType>::create()};
}

const QString PositionComponentType::Name{"position"};
const QString PositionComponentType::FieldName::MapNode{"mapNode"};

QString PositionComponentType::getName() const
{
    return Name;
}

std::vector<Field*> PositionComponentType::getFields() const
{
    static const FieldsHelper fieldsHelper{{FieldName::MapNode, Field::Type::Reference}};

    return fieldsHelper.getFields();
}

std::unique_ptr<Component> PositionComponentType::createComponent(ObjectId id)
{
    return std::make_unique<PositionComponent>(this, nullptr, id);
}

PositionComponent::PositionComponent(PositionComponentType* type, QObject* parent, ObjectId id)
    : Component(type, parent, id)
    , mapNode(Field::Type::Reference)
{
}

FieldValue* PositionComponent::field(const QString& name)
{
    if (name == Type::FieldName::MapNode)
        return &this->mapNode;

    wWarning << "Attempt to get value of non-existing field `" << name << "' from " << this->type->getName()
             << " component";

    return nullptr;
}

const FieldValue* PositionComponent::field(const QString& name) const
{
    if (name == Type::FieldName::MapNode)
        return &this->mapNode;

    wWarning << "Attempt to get value of non-existing field `" << name << "' from " << this->type->getName()
             << " component";

    return nullptr;
}

std::unordered_map<QString, FieldValue> PositionComponent::getFields() const
{
    return {{Type::FieldName::MapNode, this->mapNode}};
}

void PositionComponent::setFields(std::unordered_map<QString, FieldValue> fields)
{
    this->checkAndSetFields(std::move(fields), {&this->mapNode});
}

const QString EditComponentType::Name{"edit"};
const QString EditComponentType::FieldName::EditableComponents{"editableComponents"};

EditComponent::EditComponent(EditComponentType* type, QObject* parent, ObjectId id)
    : Component(type, parent, id)
    , editableComponents(Field::Type::List)
{
}

QString EditComponentType::getName() const
{
    return Name;
}

std::vector<Field*> EditComponentType::getFields() const
{
    static const FieldsHelper fieldsHelper{{FieldName::EditableComponents, Field::Type::List}};

    return fieldsHelper.getFields();
}

std::unique_ptr<Component> EditComponentType::createComponent(ObjectId id)
{
    return std::make_unique<EditComponent>(this, nullptr, id);
}

FieldValue* EditComponent::field(const QString& name)
{
    if (name == Type::FieldName::EditableComponents)
        return &this->editableComponents;

    wWarning << "Attempt to get value of non-existing field `" << name << "' from " << this->type->getName()
             << " component";

    return nullptr;
}

const FieldValue* EditComponent::field(const QString& name) const
{
    if (name == Type::FieldName::EditableComponents)
        return &this->editableComponents;

    wWarning << "Attempt to get value of non-existing field `" << name << "' from " << this->type->getName()
             << " component";

    return nullptr;
}

std::unordered_map<QString, FieldValue> EditComponent::getFields() const
{
    return {{Type::FieldName::EditableComponents, this->editableComponents}};
}

void EditComponent::setFields(std::unordered_map<QString, FieldValue> fields)
{
    this->checkAndSetFields(std::move(fields), {&this->editableComponents});
}

const QString GraphicsComponentType::Name{"graphics"};
const QString GraphicsComponentType::FieldName::Path{"path"};
const QString GraphicsComponentType::FieldName::X{"x"};
const QString GraphicsComponentType::FieldName::Y{"y"};
const QString GraphicsComponentType::FieldName::Z{"z"};
const QString GraphicsComponentType::FieldName::Parent{"parent"};

GraphicsComponent::GraphicsComponent(GraphicsComponentType* type, QObject* parent, ObjectId id)
    : Component(type, parent, id)
    , path(Field::Type::String)
    , x(Field::Type::Integer)
    , y(Field::Type::Integer)
    , z(Field::Type::Integer)
    , parent(Field::Type::Reference)
{
}

QString GraphicsComponentType::getName() const
{
    return Name;
}

std::vector<Field*> GraphicsComponentType::getFields() const
{
    static const FieldsHelper fieldsHelper{{FieldName::Path, Field::Type::String},
        {FieldName::X, Field::Type::Integer},
        {FieldName::Y, Field::Type::Integer},
        {FieldName::Z, Field::Type::Integer},
        {FieldName::Parent, Field::Type::Reference}};

    return fieldsHelper.getFields();
}

std::unique_ptr<Component> GraphicsComponentType::createComponent(ObjectId id)
{
    return std::make_unique<GraphicsComponent>(this, nullptr, id);
}

FieldValue* GraphicsComponent::field(const QString& name)
{
    if (name == Type::FieldName::Path)
    {
        return &this->path;
    }
    else if (name == Type::FieldName::X)
    {
        return &this->x;
    }
    else if (name == Type::FieldName::Y)
    {
        return &this->y;
    }
    else if (name == Type::FieldName::Z)
    {
        return &this->z;
    }
    else if (name == Type::FieldName::Parent)
    {
        return &this->parent;
    }
    else
    {
        wWarning << "Attempt to get value of non-existing field `" << name << "' from " << this->type->getName()
                 << " component";

        return nullptr;
    }
}

const FieldValue* GraphicsComponent::field(const QString& name) const
{
    return const_cast<GraphicsComponent*>(this)->field(name);
}

std::unordered_map<QString, FieldValue> GraphicsComponent::getFields() const
{
    return {{Type::FieldName::Path, this->path},
        {Type::FieldName::X, this->x},
        {Type::FieldName::Y, this->y},
        {Type::FieldName::Z, this->z},
        {Type::FieldName::Parent, this->parent}};
}

void GraphicsComponent::setFields(std::unordered_map<QString, FieldValue> fields)
{
    this->checkAndSetFields(std::move(fields), {&this->path, &this->x, &this->y, &this->z, &this->parent});
}

} // namespace core
} // namespace warmonger
