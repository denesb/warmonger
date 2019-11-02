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

#include "core/Settlement.h"
#include "ui/MapUtil.h"
#include "ui/Render.h"
#include "utils/Logging.h"

namespace warmonger {
namespace ui {

Map::Map(QQuickItem* parent)
    : BasicMap(parent)
{
    this->setFlags(nullptr);

    QObject::connect(this, &BasicMap::windowRectChanged, this, &Map::onWindowRectChanged);
}

void Map::setMap(core::Map* map)
{
    if (map != this->map)
    {
        wInfo.format("map `{}` -> `{}`", *this->map, *map);

        this->map = map;

        QObject::connect(this->map, &core::Map::currentPlayerChanged, this, &Map::restorePlayerContext);

        this->maybeUpdateContent();

        emit mapChanged();
    }
}

void Map::setWorldSurface(WorldSurface* worldSurface)
{
    if (worldSurface != this->worldSurface)
    {
        wInfo.format("worldSurface `{}` -> `{}`", *this->worldSurface, *worldSurface);

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

        wTrace.format("windowRect={}, mapRect={}", this->getWindowRect(), this->getMapRect());

        try
        {
            this->graphicMap = this->worldSurface->getRules().renderMap(*this->map);
        }
        catch (utils::Exception& e)
        {
            // FIXME: we need a way to communicate this to the user.
            wError.format("Failed to render map `{}': {}", *this->map, e.what());
        }
        this->restorePlayerContext();
    }
}

void Map::updatePlayerLastPosition()
{
    auto currentPlayer = this->map->getCurrentPlayer();
    if (!currentPlayer)
        return;

    const auto window = this->getWindowRect();
    const auto center = window.topLeft() + QPoint(window.width() / 2, window.height() / 2);
    this->playerLastWindowPosition[currentPlayer] = center;
}

void Map::restorePlayerContext()
{
    auto currentPlayer = this->map->getCurrentPlayer();

    wTrace.format("currentPlayer={}", *currentPlayer);

    if (auto it = this->playerLastWindowPosition.find(currentPlayer); it != this->playerLastWindowPosition.end())
    {
        wTrace.format("Center on {}", it->second);
        this->centerWindow(it->second);
        return;
    }

    const auto& settlements = this->map->getSettlements();
    auto it = std::find_if(settlements.cbegin(), settlements.cend(), [currentPlayer](const core::Settlement* const s) {
        return s->getOwner() == currentPlayer;
    });

    if (it == settlements.end())
    {
        wWarning.format("Player `{}` has no last window position, nor any owned settlements, don't know what to show",
            currentPlayer);
        return;
    }

    this->centerWindow(this->mapNodesPos.at((*it)->getPosition()));
}

void Map::onWindowRectChanged()
{
    updatePlayerLastPosition();
}

} // namespace ui
} // namespace warmonger