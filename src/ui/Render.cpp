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

#include "core/Entity.h"
#include "core/MapNode.h"
#include "ui/WorldSurface.h"

namespace warmonger {
namespace ui {

struct MapNodeContents
{
    core::MapNode* mapNode;
    QPoint pos;
    std::vector<core::Entity*> entities;

    MapNodeContents(core::MapNode* mapNode, QPoint pos, core::Entity* entity)
        : mapNode(mapNode)
        , pos(pos)
        , entities(1, entity)
    {
    }
};

static bool isVisible(const QPoint& position, const RenderContext& ctx);
static std::vector<MapNodeContents> depthSorted(std::vector<MapNodeContents> mapNodeContents);
static std::vector<MapNodeContents> visibleRootEntities(
    const std::vector<core::Entity*>& entities, const RenderContext& ctx);
static QSGNode* drawMapNodeContent(const MapNodeContents& mapNodeContents, QSGNode* oldNode, const RenderContext& ctx);
static std::vector<core::Entity*> drawableEntities(const std::vector<core::Entity*>& entities);
static QSGNode* drawEntity(core::Entity* entity, QSGNode* oldNode, const RenderContext& ctx);

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
    const std::vector<MapNodeContents> mapNodeContents = depthSorted(visibleRootEntities(entities, ctx));

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

static std::vector<MapNodeContents> visibleRootEntities(
    const std::vector<core::Entity*>& entities, const RenderContext& ctx)
{
    std::vector<MapNodeContents> mapNodeContents;
    std::unordered_map<core::MapNode*, std::size_t> mapNodeContentsIndex;

    for (auto* entity : entities)
    {
        if (entity->hasParentEntity())
            continue;

        auto* positionComponent = entity->getPositionComponent();
        auto* graphicsComponent = entity->getGraphicsComponent();

        if (!positionComponent || !graphicsComponent)
            continue;

        auto* mapNode = positionComponent->getMapNode();
        auto it = mapNodeContentsIndex.find(mapNode);
        if (it == mapNodeContentsIndex.end())
        {
            auto pos = ctx.mapNodesPos.at(mapNode);

            if (isVisible(pos, ctx))
            {
                mapNodeContents.emplace_back(mapNode, pos, entity);
                mapNodeContentsIndex.emplace(mapNode, mapNodeContents.size() - 1);
            }
        }
        else
        {
            // If the mapNode is present, it's  visible.
            mapNodeContents[it->second].entities.emplace_back(entity);
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

    std::for_each(mapNodeContents.begin(), mapNodeContents.end(), [](MapNodeContents& m) {
        std::sort(m.entities.begin(), m.entities.end(), [](core::Entity* a, core::Entity* b) {
            auto& gca = *a->getGraphicsComponent();
            auto& gcb = *b->getGraphicsComponent();
            return gca.getZ() < gcb.getZ() || gca.getY() < gcb.getY() || gca.getX() < gcb.getX();
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

    syncChildNodesWithSource(node, mapNodeContents.entities, ctx, drawEntity);

    return node;
}

static std::vector<core::Entity*> drawableEntities(const std::vector<core::Entity*>& entities)
{
    std::vector<core::Entity*> filtered;
    std::copy_if(entities.cbegin(), entities.cend(), std::back_inserter(filtered), [](core::Entity* const e) {
        return bool(e->getGraphicsComponent());
    });
    return filtered;
}

static QSGNode* drawEntity(core::Entity* const entity, QSGNode* oldNode, const RenderContext& ctx)
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

    auto& graphicsComponent = *entity->getGraphicsComponent();
    QSGTexture* texture = ctx.surface->getTexture(graphicsComponent.getPath(), ctx.window);
    QSGTexture* currentTexture = node->texture();

    if (currentTexture == nullptr || currentTexture->textureId() != texture->textureId())
    {
        node->setTexture(texture);
    }

    // TODO: don't assume image is of the same size as the tile size
    const QRect nodeRect(QPoint(graphicsComponent.getX(), graphicsComponent.getY()), ctx.surface->getTileSize());
    if (node->rect() != nodeRect)
    {
        node->setRect(nodeRect);
    }

    syncChildNodesWithSource(node, drawableEntities(entity->getChildEntities()), ctx, drawEntity);

    return node;
}

} // namespace ui
} // namespace warmonger
