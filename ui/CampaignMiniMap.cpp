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

#include <QColor>
#include <QSGTransformNode>

#include "core/Faction.h"
#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/Unit.h"
#include "ui/CampaignMapWatcher.h"
#include "ui/CampaignMiniMap.h"
#include "ui/MapUtil.h"
#include "utils/Logging.h"

namespace warmonger {
namespace ui {

CampaignMiniMap::CampaignMiniMap(QQuickItem* parent)
    : BasicMiniMap(parent)
    , worldSurface(nullptr)
    , campaignMap(nullptr)
    , watcher(nullptr)
{
}

core::CampaignMap* CampaignMiniMap::getCampaignMap() const
{
    return this->campaignMap;
}

void CampaignMiniMap::setCampaignMap(core::CampaignMap* campaignMap)
{
    if (this->campaignMap != campaignMap)
    {
        wInfo << "campaignMap `" << this->campaignMap << "' -> `" << campaignMap << "'";

        if (this->campaignMap != nullptr)
        {
            delete this->watcher;
            QObject::disconnect(this->campaignMap, nullptr, this, nullptr);
        }

        this->campaignMap = campaignMap;

        this->updateContent();

        if (this->campaignMap != nullptr)
        {
            this->watcher = new CampaignMapWatcher(this->campaignMap, this);
            QObject::connect(this->watcher, &CampaignMapWatcher::changed, this, &CampaignMiniMap::update);
            QObject::connect(
                this->campaignMap, &core::CampaignMap::mapNodesChanged, this, &CampaignMiniMap::onMapNodesChanged);
        }

        emit campaignMapChanged();
    }
}

WorldSurface* CampaignMiniMap::getWorldSurface() const
{
    return this->worldSurface;
}

void CampaignMiniMap::setWorldSurface(WorldSurface* worldSurface)
{
    if (this->worldSurface != worldSurface)
    {
        wInfo << "worldSurface `" << this->worldSurface << "' -> `" << worldSurface << "'";

        this->worldSurface = worldSurface;

        this->updateContent();
        emit worldSurfaceChanged();
    }
}

QSGNode* CampaignMiniMap::updatePaintNode(QSGNode* oldRootNode, UpdatePaintNodeData*)
{
    QSGTransformNode* rootNode;
    QSGNode* mapRootNode;

    if (oldRootNode != nullptr)
    {
        rootNode = static_cast<QSGTransformNode*>(oldRootNode);
        mapRootNode = rootNode->firstChild();

        drawRect(this->getWindowRect(), rootNode->lastChild());
    }
    else
    {
        rootNode = new QSGTransformNode();
        mapRootNode = new QSGNode;

        rootNode->appendChildNode(mapRootNode);
        rootNode->appendChildNode(drawRect(this->getWindowRect(), nullptr));
    }

    const QMatrix4x4& transform = this->getTransformMatrix();
    if (transform != rootNode->matrix())
    {
        rootNode->setMatrix(transform);
    }

    drawContents(this->campaignMap->getContents(), mapRootNode, *this);

    return rootNode;
}

QSGNode* CampaignMiniMap::drawContent(const core::CampaignMap::Content& content, QSGNode* oldNode)
{
    return drawMapNode(std::get<core::MapNode*>(content),
        this->worldSurface,
        this->window(),
        this->mapNodesPos.at(std::get<core::MapNode*>(content)),
        oldNode);
}

void CampaignMiniMap::updateContent()
{
    if (this->worldSurface == nullptr || this->campaignMap == nullptr || this->campaignMap->getMapNodes().empty() ||
        this->worldSurface->getWorld() != this->campaignMap->getWorld())
    {
        this->setFlags(0);
    }
    else
    {
        this->setFlags(QQuickItem::ItemHasContents);

        this->mapNodesPos = positionMapNodes(this->campaignMap->getMapNodes()[0], this->worldSurface->getTileSize());

        this->updateMapRect();

        this->update();
    }
}

void CampaignMiniMap::updateMapRect()
{
    if (this->worldSurface == nullptr || this->campaignMap == nullptr || this->campaignMap->getMapNodes().empty() ||
        this->worldSurface->getWorld() != this->campaignMap->getWorld())
    {
        this->setMapRect(QRect(0, 0, 0, 0));
    }
    else
    {
        this->setMapRect(calculateBoundingRect(this->mapNodesPos, this->worldSurface->getTileSize()));
        this->update();
    }
}

void CampaignMiniMap::onMapNodesChanged()
{
    this->mapNodesPos = positionMapNodes(this->campaignMap->getMapNodes()[0], this->worldSurface->getTileSize());
    this->updateMapRect();
}

} // namespace ui
} // namespace warmonger
