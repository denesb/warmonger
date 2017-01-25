/**
 * Copyright (C) 2015-2016 Botond Dénes
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

#include <QGuiApplication>
#include <QSGSimpleTextureNode>

#include "core/Utils.hpp"
#include "ui/CampaignMapPreview.h"
#include "ui/CampaignMapWatcher.h"
#include "ui/MapUtil.h"
#include "utils/Constants.h"
#include "utils/Logging.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace ui {

CampaignMapPreview::CampaignMapPreview(QQuickItem* parent)
    : QQuickItem(parent)
    , campaignMap(nullptr)
    , worldSurface(nullptr)
    , watcher(nullptr)
{
    QObject::connect(this, &CampaignMapPreview::widthChanged, this, &CampaignMapPreview::updateTransform);
    QObject::connect(this, &CampaignMapPreview::heightChanged, this, &CampaignMapPreview::updateTransform);

    this->updateTransform();
}

void CampaignMapPreview::setCampaignMap(core::CampaignMap* campaignMap)
{
    if (this->campaignMap != campaignMap)
    {
        wInfo << "campaignMap `" << this->campaignMap << "' -> `" << campaignMap << "'";

        if (this->campaignMap != nullptr)
        {
            QObject::disconnect(this->campaignMap, nullptr, this, nullptr);
            delete this->watcher;
        }

        this->campaignMap = campaignMap;
        this->updateContent();

        if (this->campaignMap)
        {
            this->watcher = new CampaignMapWatcher(this->campaignMap, this);
            QObject::connect(this->watcher, &CampaignMapWatcher::changed, this, &CampaignMapPreview::update);
            QObject::connect(
                this->campaignMap, &core::CampaignMap::mapNodesChanged, this, &CampaignMapPreview::onMapNodesChanged);
        }

        emit campaignMapChanged();
    }
}

void CampaignMapPreview::setWorldSurface(WorldSurface* worldSurface)
{
    if (this->worldSurface != worldSurface)
    {
        wInfo << "worldSurface `" << this->worldSurface << "' -> `" << worldSurface << "'";

        this->worldSurface = worldSurface;
        this->updateContent();

        emit worldSurfaceChanged();
    }
}

QSGNode* CampaignMapPreview::updatePaintNode(QSGNode* oldRootNode, UpdatePaintNodeData*)
{
    QSGClipNode* rootNode;
    QSGTransformNode* transformNode;
    QSGNode* mapRootNode;
    if (oldRootNode == nullptr)
    {
        rootNode = new QSGClipNode();
        rootNode->setIsRectangular(true);

        transformNode = new QSGTransformNode();
        transformNode->setMatrix(this->transform);
        rootNode->appendChildNode(transformNode);

        mapRootNode = new QSGNode();
        transformNode->appendChildNode(mapRootNode);
    }
    else
    {
        rootNode = static_cast<QSGClipNode*>(oldRootNode);

        transformNode = static_cast<QSGTransformNode*>(rootNode->firstChild());
        if (this->transform != transformNode->matrix())
        {
            transformNode->setMatrix(this->transform);
        }

        mapRootNode = transformNode->firstChild();
    }

    rootNode->setClipRect(QRectF(0, 0, this->width(), this->height()));

    const std::vector<core::CampaignMap::Content> contents = visibleContents(this->campaignMap->getContents(),
        this->mapNodesPos,
        this->worldSurface->getTileSize(),
        this->mapRect);

    drawContents(contents, mapRootNode, *this);

    return rootNode;
}

QSGNode* CampaignMapPreview::drawContent(const core::CampaignMap::Content& content, QSGNode* oldNode)
{
    core::MapNode* mapNode = std::get<core::MapNode*>(content);
    const QPoint& pos = this->mapNodesPos.at(mapNode);

    QSGNode* mapSGNode = drawMapNode(mapNode, this->worldSurface, this->window(), pos, oldNode);

    if (std::get<core::Settlement*>(content) != nullptr)
    {
        QSGNode* settlementSGNode = drawSettlement(
            std::get<core::Settlement*>(content), this->worldSurface, this->window(), pos, mapSGNode->firstChild());

        if (mapSGNode->firstChild() == nullptr)
            mapSGNode->appendChildNode(settlementSGNode);
    }
    else if (std::get<core::Army*>(content) != nullptr)
    {
        QSGNode* armySGNode =
            drawArmy(std::get<core::Army*>(content), this->worldSurface, this->window(), pos, mapSGNode->firstChild());

        if (mapSGNode->firstChild() == nullptr)
            mapSGNode->appendChildNode(armySGNode);
    }

    return mapSGNode;
}

void CampaignMapPreview::updateContent()
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
        this->updateTransform();
    }
}

void CampaignMapPreview::updateMapRect()
{
    if (this->worldSurface == nullptr || this->campaignMap == nullptr || this->campaignMap->getMapNodes().empty() ||
        this->worldSurface->getWorld() != this->campaignMap->getWorld())
    {
        this->mapRect = QRect(0, 0, 0, 0);
    }
    else
    {
        this->mapRect = calculateBoundingRect(this->mapNodesPos, this->worldSurface->getTileSize());
        this->update();
    }
}

void CampaignMapPreview::onMapNodesChanged()
{
    this->mapNodesPos = positionMapNodes(this->campaignMap->getMapNodes()[0], this->worldSurface->getTileSize());
    this->updateMapRect();
    this->updateTransform();
}

void CampaignMapPreview::updateTransform()
{
    this->transform =
        ui::centerIn(this->mapRect, QRect(0, 0, this->width(), this->height()));
}

} // namespace ui
} // namespace warmonger
