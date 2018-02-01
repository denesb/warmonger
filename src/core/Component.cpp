/**
 * Copyright (C) 2015-2018 Botond Dénes
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

#include "core/Component.h"

#include "core/Entity.h"
#include "core/MapNode.h"
#include "utils/Format.h"

namespace warmonger {
namespace core {

static void checkName(const std::unordered_map<QString, ir::Value>& obj, const QString& name);

Component::Component(QObject* parent, ObjectId id)
    : WObject(parent, id)
{
}

const QString PositionComponent::name{"position"};

PositionComponent::PositionComponent(QObject* parent)
    : BuiltInComponent(parent)
{
}

PositionComponent::PositionComponent(ir::Value v, QObject* parent)
    : BuiltInComponent(parent, v.getObjectId())
{
    auto obj = std::move(v).asObject();
    checkName(obj, PositionComponent::name);
    this->mapNode = obj.at("mapNode").asReference<MapNode>(parent);
}

ir::Value PositionComponent::serialize() const
{
    std::unordered_map<QString, ir::Value> obj;
    obj["id"] = this->getId().get();
    obj["name"] = this->name;
    obj["mapNode"] = this->mapNode;
    return obj;
}

void PositionComponent::setMapNode(MapNode* mapNode)
{
    this->mapNode = mapNode;
}

const QString GraphicsComponent::name{"graphics"};

GraphicsComponent::GraphicsComponent(QObject* parent)
    : BuiltInComponent(parent)
{
}

GraphicsComponent::GraphicsComponent(ir::Value v, QObject* parent)
    : BuiltInComponent(parent, v.getObjectId())
{
    auto obj = std::move(v).asObject();
    checkName(obj, GraphicsComponent::name);
    this->path = std::move(obj["path"]).asString();
    this->x = obj.at("x").asInteger();
    this->y = obj.at("y").asInteger();
    this->z = obj.at("z").asInteger();
    this->container = obj.at("container").asReference<Entity>(parent);
}

ir::Value GraphicsComponent::serialize() const
{
    std::unordered_map<QString, ir::Value> obj;
    obj["id"] = this->getId().get();
    obj["name"] = this->name;
    obj["path"] = this->path;
    obj["x"] = this->x;
    obj["y"] = this->y;
    obj["z"] = this->z;
    obj["container"] = this->container;
    return obj;
}

void GraphicsComponent::setPath(QString path)
{
    this->path = std::move(path);
}

void GraphicsComponent::setX(int x)
{
    this->x = x;
}

void GraphicsComponent::setY(int y)
{
    this->y = y;
}

void GraphicsComponent::setZ(int z)
{
    this->z = z;
}

void GraphicsComponent::setContainer(Entity* container)
{
    this->container = container;
}

static void checkName(const std::unordered_map<QString, ir::Value>& obj, const QString& name)
{
    if (obj.at("name").asString() != name)
    {
        throw utils::ValueError(fmt::format("Name mismatch, should be {} but got {}", name, obj.at("name").asString()));
    }
}

} // namespace core
} // namespace warmonger
