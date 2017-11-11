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

namespace warmonger {
namespace core {

template <class T>
struct ConcreteComponentTypeFactory
{
    static std::tuple<QString, std::function<ComponentType*(QObject*, int)>> create()
    {
        return std::make_tuple(T::staticMetaObject.className(), ConcreteComponentTypeFactory<T>());
    }

    ComponentType* operator()(QObject* parent, int id) const
    {
        return new T(parent, id);
    }
};

std::vector<std::tuple<QString, std::function<ComponentType*(QObject*, int)>>> getBuiltInComponentTypesFactories()
{
    return std::vector<std::tuple<QString, std::function<ComponentType*(QObject*, int)>>>{
        ConcreteComponentTypeFactory<PositionComponentType>::create(),
        ConcreteComponentTypeFactory<EditComponentType>::create(),
        ConcreteComponentTypeFactory<GraphicsComponentType>::create()};
}

std::vector<Field*> PositionComponentType::getFields() const
{
    static const FieldsHelper fieldsHelper{{"mapNode", Field::Type::Reference}};

    return fieldsHelper.getFields();
}

std::vector<Field*> EditComponentType::getFields() const
{
    static const FieldsHelper fieldsHelper{{"editableComponents", Field::Type::List}};

    return fieldsHelper.getFields();
}

std::vector<Field*> GraphicsComponentType::getFields() const
{
    static const FieldsHelper fieldsHelper{{"path", Field::Type::String},
        {"x", Field::Type::Integer},
        {"y", Field::Type::Integer},
        {"z", Field::Type::Integer},
        {"parent", Field::Type::Reference}};

    return fieldsHelper.getFields();
}

} // namespace core
} // namespace warmonger
