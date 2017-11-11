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

#include "core/ComponentType.h"

namespace warmonger {
namespace core {

std::vector<std::tuple<QString, std::function<ComponentType*(QObject*, int)>>> getBuiltInComponentTypesFactories();

/**
 * Position component-type.
 *
 * Defines the position of an entity on the map.
 * For an overview of the Entity-Component-Systems design pattern (ECS) and how
 * warmonger implements it see \ref docs/ECS.md.
 */
class PositionComponentType : public ComponentType
{
    Q_OBJECT

public:
    using ComponentType::ComponentType;

    bool isBuiltIn() const override
    {
        return true;
    }

    QString getName() const override
    {
        return "position";
    }

    std::vector<Field*> getFields() const override;
};

/**
 * Edit component-type.
 *
 * Defines how the entity having this component can be edited by the map
 * editor.
 */
class EditComponentType : public ComponentType
{
    Q_OBJECT

public:
    using ComponentType::ComponentType;

    bool isBuiltIn() const override
    {
        return true;
    }

    QString getName() const override
    {
        return "edit";
    }

    std::vector<Field*> getFields() const override;
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
class GraphicsComponentType : public ComponentType
{
    Q_OBJECT

public:
    using ComponentType::ComponentType;

    bool isBuiltIn() const override
    {
        return true;
    }

    QString getName() const override
    {
        return "graphics";
    }

    std::vector<Field*> getFields() const override;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_BUILTIN_COMPONENT_TYPES_H
