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

#include "ui/MapWatcher.h"
#include "core/Map.h"

namespace warmonger {
namespace ui {

MapWatcher::MapWatcher(const core::Map* const map, QObject* parent)
    : QObject(parent)
    , map(map)
    , mapNodeWatcher(new Watcher(this))
    , entityWatcher(new Watcher(this))
{
    QObject::connect(this->map, &core::Map::mapNodesChanged, this, &MapWatcher::onMapNodesChanged);
    QObject::connect(this->map, &core::Map::entitiesChanged, this, &MapWatcher::onEntitiesChanged);

    QObject::connect(this->mapNodeWatcher, &Watcher::changed, this, &MapWatcher::changed);
    QObject::connect(this->entityWatcher, &Watcher::changed, this, &MapWatcher::changed);

    this->connectMapNodeSignals();
    this->connectEntitySignals();
}

void MapWatcher::connectMapNodeSignals()
{
    const std::vector<core::MapNode*>& mapNodes = this->map->getMapNodes();
    for (core::MapNode* mapNode : mapNodes)
    {
        QObject::connect(mapNode, &core::MapNode::neighboursChanged, this->mapNodeWatcher, &Watcher::changed);
    }
}

void MapWatcher::connectEntitySignals()
{
    const std::vector<core::Entity*>& entities = this->map->getEntities();
    for (core::Entity* entity : entities)
    {
        QObject::connect(entity, &core::Entity::componentChanged, this->entityWatcher, &Watcher::changed);
    }
}

void MapWatcher::onMapNodesChanged()
{
    QObject::disconnect(this->map, nullptr, this->mapNodeWatcher, nullptr);
    this->connectMapNodeSignals();

    emit changed();
}

void MapWatcher::onEntitiesChanged()
{
    QObject::disconnect(this->map, nullptr, this->entityWatcher, nullptr);
    this->connectEntitySignals();

    emit changed();
}

} // namespace ui
} // namespace warmonger
