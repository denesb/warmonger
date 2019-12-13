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

#include "core/Settlement.h"

#include "core/Map.h"

namespace warmonger {
namespace core {

Settlement::Settlement(QObject* parent, ObjectId id)
    : WObject(parent, id)
{
}

Settlement::Settlement(ir::Value v, Map& map, QObject* parent)
    : WObject(parent, v.getObjectId())
{
    auto obj = std::move(v).asObject();
    this->type = std::move(obj["type"]).asString();
    this->position = obj["position"].asReference<MapNode>(&map);
    this->owner = obj["owner"].asReference<Faction>(&map);
}

ir::Value Settlement::serialize() const
{
    std::unordered_map<QString, ir::Value> obj;

    obj["id"] = this->getId().get();
    obj["type"] = this->type;
    obj["position"] = this->position;
    obj["owner"] = this->owner;

    return obj;
}

void Settlement::setType(QString type)
{
    if (this->type != type)
    {
        this->type = std::move(type);
        emit typeChanged();
    }
}

void Settlement::setPosition(MapNode* mapNode)
{
    if (this->position != mapNode)
    {
        this->position = mapNode;
        emit positionChanged();
    }
}

void Settlement::setOwner(Faction* owner)
{
    if (this->owner != owner)
    {
        this->owner = owner;
        emit ownerChanged();
    }
}

} // namespace core
} // namespace warmonger
