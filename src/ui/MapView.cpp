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

#include "ui/MapView.h"

#include <QGuiApplication>
#include <QSGSimpleTextureNode>

#include "ui/MapUtil.h"
#include "ui/MapWatcher.h"
#include "ui/Render.h"
#include "utils/Constants.h"
#include "utils/Logging.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace ui {

MapView::MapView(QQuickItem* parent)
    : QQuickItem(parent)
    , map(nullptr)
    , worldSurface(nullptr)
    , watcher(nullptr)
{
    QObject::connect(this, &MapView::widthChanged, this, &MapView::updateTransform);
    QObject::connect(this, &MapView::heightChanged, this, &MapView::updateTransform);

    this->updateTransform();
}

void MapView::setMap(core::Map* map)
{
    if (this->map != map)
    {
        wInfo << "map `" << this->map << "' -> `" << map << "'";

        if (this->map != nullptr)
        {
            QObject::disconnect(this->map, nullptr, this, nullptr);
            delete this->watcher;
        }

        this->map = map;
        this->updateContent();

        if (this->map)
        {
            this->watcher = new MapWatcher(this->map, this);
            QObject::connect(this->watcher, &MapWatcher::changed, this, &MapView::update);
            QObject::connect(this->map, &core::Map::mapNodesChanged, this, &MapView::onMapNodesChanged);
        }

        emit mapChanged();
    }
}

void MapView::setWorldSurface(WorldSurface* worldSurface)
{
    if (this->worldSurface != worldSurface)
    {
        wInfo << "worldSurface `" << this->worldSurface << "' -> `" << worldSurface << "'";

        this->worldSurface = worldSurface;
        this->updateContent();

        emit worldSurfaceChanged();
    }
}

QSGNode* MapView::updatePaintNode(QSGNode* oldRootNode, UpdatePaintNodeData*)
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

    RenderContext ctx{this->worldSurface, this->window(), this->mapNodesPos, this->mapRect};
    mapRootNode = renderMap(this->graphicMap, mapRootNode, ctx);

    return rootNode;
}

void MapView::updateContent()
{
    if (this->worldSurface == nullptr || this->map == nullptr || this->map->getMapNodes().empty() ||
        this->worldSurface->getWorld() != this->map->getWorld())
    {
        this->setFlags(0);
    }
    else
    {
        this->setFlags(QQuickItem::ItemHasContents);
        this->mapNodesPos = positionMapNodes(this->map->getMapNodes()[0], this->worldSurface->getTileSize());
        this->updateMapRect();
        this->updateTransform();
        this->graphicMap = this->worldSurface->getRules().renderMap(*this->map);
    }
}

void MapView::updateMapRect()
{
    if (this->worldSurface == nullptr || this->map == nullptr || this->map->getMapNodes().empty() ||
        this->worldSurface->getWorld() != this->map->getWorld())
    {
        this->mapRect = QRect(0, 0, 0, 0);
    }
    else
    {
        this->mapRect = calculateBoundingRect(this->mapNodesPos, this->worldSurface->getTileSize());
        this->update();
    }
}

void MapView::onMapNodesChanged()
{
    this->mapNodesPos = positionMapNodes(this->map->getMapNodes()[0], this->worldSurface->getTileSize());
    this->updateMapRect();
    this->updateTransform();
}

void MapView::updateTransform()
{
    this->transform = ui::centerIn(this->mapRect, QRect(0, 0, this->width(), this->height()));
}

} // namespace ui
} // namespace warmonger
