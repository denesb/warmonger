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

#include <algorithm>
#include <iterator>

#include <QGuiApplication>
#include <QPalette>
#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>
#include <QSGSimpleTextureNode>

#include "core/MapNode.h"
#include "ui/MapUtil.h"
#include "ui/WorldSurface.h"
#include "utils/Logging.h"
#include "utils/Settings.h"

namespace warmonger {
namespace ui {

static void positionMapNode(core::MapNode* node, std::unordered_map<core::MapNode*, QPoint>& nodesPos, int tileSize);

QPoint neighbourPos(const QPoint& pos, core::Direction dir, int tileSize)
{
    QSize displacement(0, 0);

    switch (dir)
    {
        case core::Direction::West:
            displacement.setWidth(-tileSize);
            break;
        case core::Direction::NorthWest:
            displacement.setWidth(-tileSize / 2);
            displacement.setHeight(-tileSize * 3 / 4);
            break;
        case core::Direction::NorthEast:
            displacement.setWidth(tileSize / 2);
            displacement.setHeight(-tileSize * 3 / 4);
            break;
        case core::Direction::East:
            displacement.setWidth(tileSize);
            break;
        case core::Direction::SouthEast:
            displacement.setWidth(tileSize / 2);
            displacement.setHeight(tileSize * 3 / 4);
            break;
        case core::Direction::SouthWest:
            displacement.setWidth(-tileSize / 2);
            displacement.setHeight(tileSize * 3 / 4);
            break;
    }

    return QPoint(pos.x() + displacement.width(), pos.y() + displacement.height());
}

core::MapNodeNeighbours neighboursByPos(
    const QPoint& pos, const WorldSurface* worldSurface, const std::unordered_map<core::MapNode*, QPoint>& mapNodesPos)
{
    core::MapNodeNeighbours neighbours;

    const auto tileSize = worldSurface->getTileSize();
    for (core::Direction direction : core::directions)
    {
        QPoint nPos = neighbourPos(pos, direction, tileSize);

        neighbours[direction] = mapNodeAtPos(nPos, mapNodesPos, worldSurface);
    }

    return neighbours;
}

std::unordered_map<core::MapNode*, QPoint> positionMapNodes(core::MapNode* startNode, int tileSize)
{
    std::unordered_map<core::MapNode*, QPoint> nodesPos;

    nodesPos.insert(std::make_pair(startNode, QPoint(0, 0)));

    positionMapNode(startNode, nodesPos, tileSize);

    return nodesPos;
}

QRect calculateBoundingRect(const std::unordered_map<core::MapNode*, QPoint>& nodesPos, int tileSize)
{
    if (nodesPos.size() == 0)
        return QRect(0, 0, 0, 0);

    QPoint bpos = nodesPos.begin()->second;
    QPoint topLeft = bpos;
    QPoint bottomRight = bpos;

    for (const auto& element : nodesPos)
    {
        QPoint pos = element.second;
        topLeft.setX(std::min(pos.x(), topLeft.x()));
        bottomRight.setX(std::max(pos.x(), bottomRight.x()));

        topLeft.setY(std::min(pos.y(), topLeft.y()));
        bottomRight.setY(std::max(pos.y(), bottomRight.y()));
    }

    // x,y is the top-left corner of the node so we need to add the tile
    // size
    bottomRight += QPoint(tileSize, tileSize);

    // leave a half-tile padding
    QPoint padding(tileSize / 2, tileSize / 2);
    topLeft -= padding;
    bottomRight += padding;

    return QRect(topLeft, bottomRight);
}

core::MapNode* mapNodeAtPos(
    const QPoint& pos, const std::unordered_map<core::MapNode*, QPoint>& nodesPos, const WorldSurface* worldSurface)
{
    auto tileSize = worldSurface->getTileSize();
    for (const std::pair<core::MapNode*, QPoint>& nodePosItem : nodesPos)
    {
        const QPoint& nodePos = nodePosItem.second;
        const QPoint hexPos = pos - nodePos;
        if (QRect(nodePos, QSize(tileSize, tileSize)).contains(pos) && worldSurface->hexContains(hexPos))
            return nodePosItem.first;
    }

    return nullptr;
}

QPoint project(const QPoint& p, const QRect& r)
{
    QPoint pp(p);
    if (!r.contains(pp))
    {
        pp.setX(std::min(std::max(r.left(), pp.x()), r.right()));
        pp.setY(std::min(std::max(r.top(), pp.y()), r.bottom()));
    }

    return pp;
}

QMatrix4x4 centerIn(const QRectF& content, const QRectF& frame)
{
    const qreal contentSize = std::max(content.width(), content.height());
    const qreal frameSize = std::min(frame.width(), frame.height());

    qreal scale{1};
    if (frameSize < contentSize)
    {
        scale = frameSize / contentSize;
    }

    const QSizeF scaledContentSize = QSizeF(content.size()) * scale;

    const qreal dx = (frame.width() - scaledContentSize.width()) / 2.0;
    const qreal dy = (frame.height() - scaledContentSize.height()) / 2.0;

    QPointF translate = QPointF(-content.topLeft());
    translate += (QPointF(dx, dy) * (1 / scale));

    QMatrix4x4 matrix;
    matrix.scale(scale);
    matrix.translate(translate.x(), translate.y());

    return matrix;
}

QMatrix4x4 moveTo(const QPoint& point, const QPoint& refPoint)
{
    QMatrix4x4 matrix;

    matrix.translate(refPoint.x() - point.x(), refPoint.y() - point.y());

    return matrix;
}

QSGNode* drawRect(const QRect& rect, QSGNode* oldNode)
{
    QSGGeometryNode* node;
    if (oldNode == nullptr)
    {
        node = new QSGGeometryNode;

        QSGGeometry* geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 8);
        geometry->setDrawingMode(GL_LINES);
        geometry->setLineWidth(1);

        QSGFlatColorMaterial* material = new QSGFlatColorMaterial;
        material->setColor(QGuiApplication::palette().color(QPalette::Normal, QPalette::WindowText));

        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }
    else
    {
        node = static_cast<QSGGeometryNode*>(oldNode);
    }

    QSGGeometry* geometry = node->geometry();

    // FIXME: check if these really have to be updated
    geometry->vertexDataAsPoint2D()[0].set(rect.topLeft().x(), rect.topLeft().y());
    geometry->vertexDataAsPoint2D()[1].set(rect.topRight().x(), rect.topRight().y());
    geometry->vertexDataAsPoint2D()[2].set(rect.topRight().x(), rect.topRight().y());
    geometry->vertexDataAsPoint2D()[3].set(rect.bottomRight().x(), rect.bottomRight().y());
    geometry->vertexDataAsPoint2D()[4].set(rect.bottomRight().x(), rect.bottomRight().y());
    geometry->vertexDataAsPoint2D()[5].set(rect.bottomLeft().x(), rect.bottomLeft().y());
    geometry->vertexDataAsPoint2D()[6].set(rect.bottomLeft().x(), rect.bottomLeft().y());
    geometry->vertexDataAsPoint2D()[7].set(rect.topLeft().x(), rect.topLeft().y());

    node->markDirty(QSGNode::DirtyGeometry);

    return node;
}

static void positionMapNode(core::MapNode* node, std::unordered_map<core::MapNode*, QPoint>& nodesPos, int tileSize)
{
    QPoint pos = nodesPos[node];
    const core::MapNodeNeighbours& neighbours = node->getNeighbours();
    for (const auto& element : neighbours)
    {
        core::Direction dir = element.first;
        core::MapNode* neighbour = element.second;

        if (neighbour == nullptr || nodesPos.find(neighbour) != nodesPos.end())
            continue;

        nodesPos.insert(std::make_pair(neighbour, neighbourPos(pos, dir, tileSize)));

        positionMapNode(neighbour, nodesPos, tileSize);
    }
}

} // namespace ui
} // namespace warmonger
