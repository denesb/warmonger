/**
 * \copyright (C) 2015-2019 Botond Dénes
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

#include "Map.h"

#include <QSGClipNode>

#include "ui/MapUtil.h"
#include "ui/Render.h"
#include "utils/Logging.h"

namespace warmonger {
namespace ui {

Map::Map(QQuickItem* parent)
    : BasicMap(parent)
{
    this->setFlags(nullptr);
}

void Map::setMap(core::Map* map)
{
    if (map != this->map)
    {
        this->map = map;
        this->maybeUpdateContent();
        emit mapChanged();
    }
}

void Map::setWorldSurface(WorldSurface* worldSurface)
{
    if (worldSurface != this->worldSurface)
    {
        this->worldSurface = worldSurface;
        this->maybeUpdateContent();
        emit worldSurfaceChanged();
    }
}

QSGNode* Map::updatePaintNode(QSGNode* oldRootNode, UpdatePaintNodeData*)
{
    QSGClipNode* rootNode;
    QSGNode* mapRootNode;
    if (oldRootNode == nullptr)
    {
        rootNode = new QSGClipNode();
        rootNode->setIsRectangular(true);

        mapRootNode = new QSGNode();
        rootNode->appendChildNode(mapRootNode);
    }
    else
    {
        rootNode = static_cast<QSGClipNode*>(oldRootNode);

        mapRootNode = rootNode->firstChild();
    }

    rootNode->setClipRect(QRectF(0, 0, this->width(), this->height()));

    RenderContext ctx{this->worldSurface, this->window(), this->mapNodesPos, this->getWindowRect()};
    mapRootNode = renderMap(this->graphicMap, mapRootNode, ctx);

    return rootNode;
}

void Map::maybeUpdateContent()
{
    if (this->worldSurface == nullptr || this->map == nullptr || this->map->getMapNodes().empty() ||
        this->worldSurface->getWorld() != this->map->getWorld())
    {
        this->setFlags(nullptr);
    }
    else
    {
        this->setFlags(QQuickItem::ItemHasContents);
        this->mapNodesPos = positionMapNodes(this->map->getMapNodes()[0], this->worldSurface->getTileSize());
        this->setMapRect(calculateBoundingRect(this->mapNodesPos, this->worldSurface->getTileSize()));
        try
        {
            this->graphicMap = this->worldSurface->getRules().renderMap(*this->map);
        }
        catch (utils::Exception& e)
        {
            // FIXME: we need a way to communicate this to the user.
            wError.format("Failed to render map `{}': {}", *this->map, e.what());
        }
    }
}

} // namespace ui
} // namespace warmonger