/** \file
 * Built-in component-types.
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
#ifndef W_CORE_BUILTIN_COMPONENT_TYPES_H
#define W_CORE_BUILTIN_COMPONENT_TYPES_H

#include <functional>

#include "core/Component.h"
#include "core/ComponentType.h"
#include "core/MapNode.h"

namespace warmonger {
namespace core {

std::vector<std::tuple<QString, std::function<ComponentType*(QObject*, ObjectId)>>> getBuiltInComponentTypesFactories();

class BuiltInComponentType : public ComponentType
{
public:
    using ComponentType::ComponentType;

    bool isBuiltIn() const override
    {
        return true;
    }
};

/**
 * Position component-type.
 *
 * Defines the position of an entity on the map.
 * For an overview of the Entity-Component-Systems design pattern (ECS) and how
 * warmonger implements it see \ref docs/ECS.md.
 */
class PositionComponentType : public BuiltInComponentType
{
    Q_OBJECT

public:
    static const QString Name;
    struct FieldName
    {
        static const QString MapNode;
    };

    using BuiltInComponentType::BuiltInComponentType;

    QString getName() const override;
    std::vector<Field*> getFields() const override;
    std::unique_ptr<Component> createComponent(QObject* parent, ObjectId id = ObjectId::Invalid) override;
};

/**
 * Position component.
 *
 * \see PositionComponentType.
 */
class PositionComponent : public Component
{
    Q_OBJECT
    using Type = PositionComponentType;

public:
    PositionComponent(PositionComponentType* type, QObject* parent, ObjectId id = ObjectId::Invalid);

    FieldValue* field(const QString& name) override;
    const FieldValue* field(const QString& name) const override;
    std::unordered_map<QString, FieldValue> getFields() const override;
    void setFields(std::unordered_map<QString, FieldValue> fields) override;

private:
    FieldValue mapNode;
};

/**
 * Edit component-type.
 *
 * Defines how the entity having this component can be edited by the map
 * editor.
 */
class EditComponentType : public BuiltInComponentType
{
    Q_OBJECT

public:
    static const QString Name;
    struct FieldName
    {
        static const QString EditableComponents;
    };

    using BuiltInComponentType::BuiltInComponentType;

    QString getName() const override;
    std::vector<Field*> getFields() const override;
    std::unique_ptr<Component> createComponent(QObject* parent, ObjectId id = ObjectId::Invalid) override;
};

/**
 * Edit component.
 *
 * \see EditComponentType.
 */
class EditComponent : public Component
{
    Q_OBJECT
    using Type = EditComponentType;

public:
    EditComponent(EditComponentType* type, QObject* parent, ObjectId id = ObjectId::Invalid);

    FieldValue* field(const QString& name) override;
    const FieldValue* field(const QString& name) const override;
    std::unordered_map<QString, FieldValue> getFields() const override;
    void setFields(std::unordered_map<QString, FieldValue> fields) override;

private:
    FieldValue editableComponents;
};

/**
 * Graphics component-type.
 *
 * Defines how the entity having this component can be rendered.
 * It has the following fields:
 * - path the path to the image.
 * - x the x offset from the upper-left corner of parent.
 * - y the y offset from the upper-left corner of parent.
 * - z depth-ordering, larger z values will be more to the foreground.
 * - parent the parent ebtity of this entity.
 *
 * Note:
 * Offsets are in pixels.
 * The parent allows entities to be organized in a tree. When the root
 * object changes position, all its children also change position
 * automatically. If the entity doesn't have a parent its position
 * component will be used to determine the position. If it doesn't have
 * a position component either it won't be rendered at all.
 * The path is meant to be an opaque handle to a graphic resource that
 * both the world-rules and the rendering layer understands.
 */
class GraphicsComponentType : public BuiltInComponentType
{
    Q_OBJECT

public:
    static const QString Name;
    struct FieldName
    {
        static const QString Path;
        static const QString X;
        static const QString Y;
        static const QString Z;
        static const QString Parent;
    };

    using BuiltInComponentType::BuiltInComponentType;

    QString getName() const override;
    std::vector<Field*> getFields() const override;
    std::unique_ptr<Component> createComponent(QObject* parent, ObjectId id = ObjectId::Invalid) override;
};

/**
 * Graphics component.
 *
 * \see GraphicsComponentType.
 */
class GraphicsComponent : public Component
{
    Q_OBJECT
    using Type = GraphicsComponentType;

public:
    GraphicsComponent(GraphicsComponentType* type, QObject* parent, ObjectId id = ObjectId::Invalid);

    FieldValue* field(const QString& name) override;
    const FieldValue* field(const QString& name) const override;
    std::unordered_map<QString, FieldValue> getFields() const override;
    void setFields(std::unordered_map<QString, FieldValue> fields) override;

private:
    FieldValue path;
    FieldValue x;
    FieldValue y;
    FieldValue z;
    FieldValue parent;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_BUILTIN_COMPONENT_TYPES_H
