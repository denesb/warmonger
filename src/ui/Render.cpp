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

#include "ui/Render.h"

#include <algorithm>
#include <iterator>

#include <QSGNode>
#include <QSGSimpleTextureNode>
#include <QSGTransformNode>
#include <QString>

#include "core/MapNode.h"
#include "ui/WorldSurface.h"

namespace warmonger {
namespace ui {

struct MapNodeContents
{
    const graphics::MapNode* mapNode;
    QPoint pos;

    MapNodeContents(const graphics::MapNode& mapNode, QPoint pos)
        : mapNode(&mapNode)
        , pos(pos)
    {
    }
};

static bool isVisible(const QPoint& position, const RenderContext& ctx);
static std::vector<MapNodeContents> depthSorted(std::vector<MapNodeContents> mapNodeContents);
static std::vector<MapNodeContents> visibleMapNodes(const graphics::Map& map, const RenderContext& ctx);
static QSGNode* drawMapNodeContent(const MapNodeContents& mapNodeContents, QSGNode* oldNode, const RenderContext& ctx);
static QSGNode* drawMapNodeLayer(const graphics::MapNodeLayer& layer, QSGNode* oldNode, const RenderContext& ctx);
static QSGNode* drawGridTile(const graphics::GridTile& gridTile, QSGNode* oldNode, const RenderContext& ctx);

template <typename Source, typename Func>
static void syncChildNodesWithSource(QSGNode* rootNode, Source&& source, const RenderContext& ctx, Func&& func)
{
    const int sourceCount = static_cast<int>(source.size());
    const int nodesCount = rootNode->childCount();
    const int n = std::min(sourceCount, nodesCount);

    for (int i = 0; i < n; ++i)
    {
        // returned node ignored, since it will always be oldNode
        func(source[i], rootNode->childAtIndex(i), ctx);
    }

    if (sourceCount < nodesCount)
    {
        for (int i = sourceCount; i < sourceCount; ++i)
            rootNode->removeChildNode(rootNode->lastChild());
    }
    else if (sourceCount > nodesCount)
    {
        for (int i = nodesCount; i < sourceCount; ++i)
        {
            QSGNode* newNode = func(source[i], nullptr, ctx);

            if (newNode != nullptr)
                rootNode->appendChildNode(newNode);
        }
    }
}

QSGNode* renderMap(const graphics::Map& map, QSGNode* oldNode, const RenderContext& ctx)
{
    const std::vector<MapNodeContents> mapNodeContents = depthSorted(visibleMapNodes(map, ctx));

    QSGNode* rootNode;
    if (oldNode)
    {
        rootNode = oldNode;
    }
    else
    {
        rootNode = new QSGNode();
    }

    syncChildNodesWithSource(rootNode, mapNodeContents, ctx, drawMapNodeContent);

    return rootNode;
}

static bool isVisible(const QPoint& position, const RenderContext& ctx)
{
    const auto tileSize = ctx.surface->getTileSize();
    const QRect nodeRect(position, QSize(tileSize, tileSize));
    return ctx.renderWindow.intersects(nodeRect);
}

static std::vector<MapNodeContents> visibleMapNodes(const graphics::Map& map, const RenderContext& ctx)
{
    std::vector<MapNodeContents> mapNodeContents;

    for (auto& mapNode : map.mapNodes)
    {
        auto pos = ctx.mapNodesPos.at(mapNode.mapNode);
        if (isVisible(pos, ctx))
        {
            mapNodeContents.emplace_back(mapNode, pos);
        }
    }

    return mapNodeContents;
}

static std::vector<MapNodeContents> depthSorted(std::vector<MapNodeContents> mapNodeContents)
{
    std::sort(mapNodeContents.begin(), mapNodeContents.end(), [](const MapNodeContents& a, const MapNodeContents& b) {
        if (a.pos.y() == b.pos.y())
            return a.pos.x() < b.pos.x();
        else
            return a.pos.y() < b.pos.y();
    });

    return mapNodeContents;
}

static QSGNode* drawMapNodeContent(const MapNodeContents& mapNodeContents, QSGNode* oldNode, const RenderContext& ctx)
{
    QSGTransformNode* node;
    if (oldNode == nullptr)
    {
        node = new QSGTransformNode();
    }
    else
    {
        // if not nullptr, it can only be a texture node
        node = static_cast<QSGTransformNode*>(oldNode);
    }

    QMatrix4x4 matrix;
    matrix.translate(mapNodeContents.pos.x(), mapNodeContents.pos.y());
    if (matrix != node->matrix())
    {
        node->setMatrix(matrix);
    }

    syncChildNodesWithSource(node, mapNodeContents.mapNode->layers, ctx, drawMapNodeLayer);

    return node;
}

static QSGNode* drawMapNodeLayer(const graphics::MapNodeLayer& layer, QSGNode* oldNode, const RenderContext& ctx)
{
    QSGNode* rootNode;
    if (oldNode)
    {
        rootNode = oldNode;
    }
    else
    {
        rootNode = new QSGNode();
    }

    syncChildNodesWithSource(rootNode, layer.gridTiles, ctx, drawGridTile);

    return rootNode;
}

static QSGNode* drawGridTile(const graphics::GridTile& gridTile, QSGNode* oldNode, const RenderContext& ctx)
{
    QSGSimpleTextureNode* node;
    if (oldNode == nullptr)
    {
        node = new QSGSimpleTextureNode();
        node->setOwnsTexture(false);
    }
    else
    {
        // if not nullptr, it can only be a texture node
        node = static_cast<QSGSimpleTextureNode*>(oldNode);
    }

    QSGTexture* texture = ctx.surface->getTexture(gridTile.assetId, ctx.window);
    QSGTexture* currentTexture = node->texture();

    if (currentTexture == nullptr || currentTexture->textureId() != texture->textureId())
    {
        node->setTexture(texture);
    }

    // TODO: don't assume image is of the same size as the tile size
    const auto gridSize = ctx.surface->getGridSize();
    const QRect nodeRect(QPoint(gridTile.x * gridSize, gridTile.y * gridSize),
        QSize(gridTile.width * gridSize, gridTile.height * gridSize));
    if (node->rect() != nodeRect)
    {
        node->setRect(nodeRect);
    }

    return node;
}

} // namespace ui
} // namespace warmonger
