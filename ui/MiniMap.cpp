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
#include "ui/MapWatcher.h"
#include "ui/MiniMap.h"
#include "ui/MapUtil.h"
#include "utils/Logging.h"

namespace warmonger {
namespace ui {

MiniMap::MiniMap(QQuickItem* parent)
    : BasicMiniMap(parent)
    , worldSurface(nullptr)
    , map(nullptr)
    , watcher(nullptr)
{
}

core::Map* MiniMap::getMap() const
{
    return this->map;
}

void MiniMap::setMap(core::Map* map)
{
    if (this->map != map)
    {
        wInfo << "map `" << this->map << "' -> `" << map << "'";

        if (this->map != nullptr)
        {
            delete this->watcher;
            QObject::disconnect(this->map, nullptr, this, nullptr);
        }

        this->map = map;

        this->updateContent();

        if (this->map != nullptr)
        {
            this->watcher = new MapWatcher(this->map, this);
            QObject::connect(this->watcher, &MapWatcher::changed, this, &MiniMap::update);
            QObject::connect(
                this->map, &core::Map::mapNodesChanged, this, &MiniMap::onMapNodesChanged);
        }

        emit mapChanged();
    }
}

WorldSurface* MiniMap::getWorldSurface() const
{
    return this->worldSurface;
}

void MiniMap::setWorldSurface(WorldSurface* worldSurface)
{
    if (this->worldSurface != worldSurface)
    {
        wInfo << "worldSurface `" << this->worldSurface << "' -> `" << worldSurface << "'";

        this->worldSurface = worldSurface;

        this->updateContent();
        emit worldSurfaceChanged();
    }
}

QSGNode* MiniMap::updatePaintNode(QSGNode* oldRootNode, UpdatePaintNodeData*)
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

    // TODO: invoke GraphicsSystem

    return rootNode;
}

void MiniMap::updateContent()
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

        this->update();
    }
}

void MiniMap::updateMapRect()
{
    if (this->worldSurface == nullptr || this->map == nullptr || this->map->getMapNodes().empty() ||
        this->worldSurface->getWorld() != this->map->getWorld())
    {
        this->setMapRect(QRect(0, 0, 0, 0));
    }
    else
    {
        this->setMapRect(calculateBoundingRect(this->mapNodesPos, this->worldSurface->getTileSize()));
        this->update();
    }
}

void MiniMap::onMapNodesChanged()
{
    this->mapNodesPos = positionMapNodes(this->map->getMapNodes()[0], this->worldSurface->getTileSize());
    this->updateMapRect();
}

} // namespace ui
} // namespace warmonger
