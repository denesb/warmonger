/** \file
 * Component class.
 *
 * \copyright (C) 2015-2018 Botond Dénes
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

#include "core/IntermediateRepresentation.h"

namespace warmonger {
namespace core {

/**
 * The component of a game entity.
 *
 * For an overview of the Entity-Component-Systems design pattern (ECS) and how
 * warmonger implements it see \ref docs/ECS.md.
 *
 * Components have a name that defines their identity.
 */
class Component : public WObject, public ir::Serializable
{
    Q_OBJECT

    /**
     * The name of the component
     */
    Q_PROPERTY(QString name READ getName CONSTANT)

public:
    /**
     * Create a component with the specified name.
     *
     * \see WObject::WObject
     */
    Component(QObject* parent, ObjectId id = ObjectId::Invalid);

    virtual const QString& getName() const = 0;

    virtual bool isBuiltIn() const = 0;
};

class BuiltInComponent : public Component
{
public:
    using Component::Component;

    bool isBuiltIn() const override
    {
        return true;
    }
};

class MapNode;

/**
 * Position component-type.
 *
 * Defines the position of an entity on the map.
 * For an overview of the Entity-Component-Systems design pattern (ECS) and how
 * warmonger implements it see \ref docs/ECS.md.
 */
class PositionComponent : public BuiltInComponent
{
    Q_OBJECT

public:
    static const QString name;

    /**
     * Crate an empty component.
     */
    PositionComponent(QObject* parent);

    /**
     * Construct the component from the intermediate-representation.
     *
     * Unserializing constructor.
     *
     * \param v the intermediate-representation
     * \param parent the parent QObject.
     */
    PositionComponent(ir::Value v, QObject* parent);

    /**
     * Serialize the component.
     */
    ir::Value serialize() const override;

    const QString& getName() const override
    {
        return PositionComponent::name;
    }

    MapNode* getMapNode() const
    {
        return this->mapNode;
    }

    void setMapNode(MapNode* mapNode);

private:
    MapNode* mapNode{nullptr};
};

class Entity;

/**
 * Graphics component-type.
 *
 * Defines how the entity having this component can be rendered.
 * It has the following fields:
 * - path the path to the image.
 * - x the x offset from the upper-left corner of parent.
 * - y the y offset from the upper-left corner of parent.
 * - z depth-ordering, larger z values will be more to the foreground.
 * - container the entity this entity is graphically part of.
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
class GraphicsComponent : public BuiltInComponent
{
    Q_OBJECT

public:
    static const QString name;

    /**
     * Construct an empty component.
     */
    GraphicsComponent(QObject* parent);

    /**
     * Construct the component from the intermediate-representation.
     *
     * Unserializing constructor.
     *
     * \param v the intermediate-representation
     * \param parent the parent QObject.
     */
    GraphicsComponent(ir::Value v, QObject* parent);

    /**
     * Serialize the component.
     */
    ir::Value serialize() const override;

    const QString& getName() const override
    {
        return GraphicsComponent::name;
    }

    QString getPath() const
    {
        return this->path;
    }

    void setPath(QString path);

    int getX() const
    {
        return this->x;
    }

    void setX(int x);

    int getY() const
    {
        return this->y;
    }

    void setY(int y);

    int getZ() const
    {
        return this->z;
    }

    void setZ(int z);

private:
    QString path;
    int x{0};
    int y{0};
    int z{0};
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_COMPONENT_H
