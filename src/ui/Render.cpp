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

#include "ui/Render.h"

#include <algorithm>
#include <iterator>

#include <QSGNode>
#include <QSGSimpleTextureNode>
#include <QSGTransformNode>
#include <QString>

#include "core/Component.h"
#include "core/Entity.h"
#include "core/MapNode.h"
#include "ui/WorldSurface.h"

namespace warmonger {
namespace ui {

// component-types of interest
namespace ComponentType {

const QString Position{"position"};
const QString Graphics{"graphics"};
}

struct MapNodeContents
{
    core::MapNode* mapNode;
    QPoint pos;
    std::vector<core::ComponentWrapper> graphicsComponents;

    MapNodeContents(core::MapNode* mapNode, QPoint pos, core::ComponentWrapper graphicsComponent)
        : mapNode(mapNode)
        , pos(pos)
        , graphicsComponents(1, std::move(graphicsComponent))
    {
    }
};

static bool isVisible(const QPoint& position, const RenderContext& ctx);
static std::vector<MapNodeContents> depthSorted(
    std::unordered_map<core::WObject*, MapNodeContents> unsortedMapNodeContents);
static std::unordered_map<core::WObject*, MapNodeContents> visibleMapNodeContents(
    const std::vector<core::Entity*>& entities, const RenderContext& ctx);
static QSGNode* drawMapNodeContent(const MapNodeContents& mapNodeContents, QSGNode* oldNode, const RenderContext& ctx);
static QSGNode* drawGraphicsComponent(
    const core::ComponentWrapper& graphicsComponent, QSGNode* oldNode, const RenderContext& ctx);

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

QSGNode* renderEntities(const std::vector<core::Entity*>& entities, QSGNode* oldNode, const RenderContext& ctx)
{
    const std::vector<MapNodeContents> mapNodeContents = depthSorted(visibleMapNodeContents(entities, ctx));

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
    const QRect nodeRect(position, ctx.surface->getTileSize());
    return ctx.renderWindow.intersects(nodeRect);
}

static std::unordered_map<core::WObject*, MapNodeContents> visibleMapNodeContents(
    const std::vector<core::Entity*>& entities, const RenderContext& ctx)
{
    std::unordered_map<core::WObject*, MapNodeContents> mapNodeContents;

    for (core::EntityWrapper entity : entities)
    {
        core::ComponentWrapper positionComponent = entity[ComponentType::Position];
        core::ComponentWrapper graphicsComponent = entity[ComponentType::Graphics];

        if (!positionComponent || !graphicsComponent)
            continue;

        core::WObject* key = positionComponent["mapNode"];
        auto it = mapNodeContents.find(key);
        QPoint pos;
        if (it == mapNodeContents.end())
        {
            auto mapNode = qobject_cast<core::MapNode*>(key);
            pos = ctx.mapNodesPos.at(mapNode);

            if (isVisible(pos, ctx))
            {
                mapNodeContents.emplace(mapNode, MapNodeContents{mapNode, pos, graphicsComponent});
            }
        }
        else
        {
            // If the mapNode is present, it's  visible.
            it->second.graphicsComponents.emplace_back(std::move(graphicsComponent));
        }
    }

    return mapNodeContents;
}

static std::vector<MapNodeContents> depthSorted(
    std::unordered_map<core::WObject*, MapNodeContents> unsortedMapNodeContents)
{
    std::vector<MapNodeContents> mapNodeContents;

    mapNodeContents.reserve(unsortedMapNodeContents.size());
    std::transform(unsortedMapNodeContents.begin(),
        unsortedMapNodeContents.end(),
        std::back_inserter(mapNodeContents),
        [](auto& value) { return std::move(value.second); });

    std::sort(mapNodeContents.begin(), mapNodeContents.end(), [](const MapNodeContents& a, const MapNodeContents& b) {
        return a.pos.y() < b.pos.y() || a.pos.x() < b.pos.x();
    });

    std::for_each(mapNodeContents.begin(), mapNodeContents.end(), [](MapNodeContents& m) {
        std::sort(m.graphicsComponents.begin(),
            m.graphicsComponents.end(),
            [](core::ComponentWrapper& a, core::ComponentWrapper& b) {
                return a["z"] < b["z"] || a["y"] < b["y"] || a["x"] < b["x"];
            });
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

    syncChildNodesWithSource(node, mapNodeContents.graphicsComponents, ctx, drawGraphicsComponent);

    return node;
}

static QSGNode* drawGraphicsComponent(
    const core::ComponentWrapper& graphicsComponent, QSGNode* oldNode, const RenderContext& ctx)
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

    QSGTexture* texture = ctx.surface->getTexture(graphicsComponent["path"], ctx.window);
    QSGTexture* currentTexture = node->texture();

    if (currentTexture == nullptr || currentTexture->textureId() != texture->textureId())
    {
        node->setTexture(texture);
    }

    // TODO: don't assume image is of the same size as the tile size
    const QRect nodeRect(QPoint(graphicsComponent["x"], graphicsComponent["y"]), ctx.surface->getTileSize());
    if (node->rect() != nodeRect)
    {
        node->setRect(nodeRect);
    }

    return node;
}

} // namespace ui
} // namespace warmonger
