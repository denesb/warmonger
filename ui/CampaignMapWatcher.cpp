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

#include "core/CampaignMap.h"
#include "ui/CampaignMapWatcher.h"

namespace warmonger {
namespace ui {

CampaignMapWatcher::CampaignMapWatcher(const core::CampaignMap* const campaignMap, QObject* parent)
    : QObject(parent)
    , campaignMap(campaignMap)
    , mapNodeWatcher(new Watcher(this))
    , entityWatcher(new Watcher(this))
{
    QObject::connect(
        this->campaignMap, &core::CampaignMap::mapNodesChanged, this, &CampaignMapWatcher::onMapNodesChanged);
    QObject::connect(
        this->campaignMap, &core::CampaignMap::entitiesChanged, this, &CampaignMapWatcher::onEntitiesChanged);

    QObject::connect(this->mapNodeWatcher, &Watcher::changed, this, &CampaignMapWatcher::changed);
    QObject::connect(this->entityWatcher, &Watcher::changed, this, &CampaignMapWatcher::changed);

    this->connectMapNodeSignals();
    this->connectEntitySignals();
}

void CampaignMapWatcher::connectMapNodeSignals()
{
    const std::vector<core::MapNode*>& mapNodes = this->campaignMap->getMapNodes();
    for (core::MapNode* mapNode : mapNodes)
    {
        QObject::connect(mapNode, &core::MapNode::neighboursChanged, this->mapNodeWatcher, &Watcher::changed);
    }
}

void CampaignMapWatcher::connectEntitySignals()
{
    const std::vector<core::Entity*>& entities = this->campaignMap->getEntities();
    for (core::Entity* entity : entities)
    {
        QObject::connect(entity, &core::Entity::componentChanged, this->entityWatcher, &Watcher::changed);
    }
}

void CampaignMapWatcher::onMapNodesChanged()
{
    QObject::disconnect(this->campaignMap, nullptr, this->mapNodeWatcher, nullptr);
    this->connectMapNodeSignals();

    emit changed();
}

void CampaignMapWatcher::onEntitiesChanged()
{
    QObject::disconnect(this->campaignMap, nullptr, this->entityWatcher, nullptr);
    this->connectEntitySignals();

    emit changed();
}

} // namespace ui
} // namespace warmonger
