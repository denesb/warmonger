#include <QSize>

#include "ui/MapUtil.h"

namespace warmonger {
namespace ui {

QPoint neighbourPos(
    const QPoint &pos,
    core::MapNode::Direction dir,
    const QSize &tileSize
)
{
    QSize displacement(0, 0);

    switch(dir)
    {
    case core::MapNode::West:
        displacement.setWidth(-tileSize.width());
        break;
    case core::MapNode::NorthWest:
        displacement.setWidth(-tileSize.width()/2);
        displacement.setHeight(-tileSize.height() * 3/4);
        break;
    case core::MapNode::NorthEast:
        displacement.setWidth(tileSize.width()/2);
        displacement.setHeight(-tileSize.height() * 3/4);
        break;
    case core::MapNode::East:
        displacement.setWidth(tileSize.width());
        break;
    case core::MapNode::SouthEast:
        displacement.setWidth(tileSize.width()/2);
        displacement.setHeight(tileSize.height() * 3/4);
        break;
    case core::MapNode::SouthWest:
        displacement.setWidth(-tileSize.width()/2);
        displacement.setHeight(tileSize.height() * 3/4);
        break;
    }

    return QPoint(
        pos.x() + displacement.width(),
        pos.y() + displacement.height()
    );
}

void positionNodes(
    core::MapNode *node,
    QHash<const core::MapNode *, NodeInfo *> &nodesInfo,
    const QSize &tileSize
)
{
    QPoint pos = nodesInfo[node]->pos;
    QHash<core::MapNode::Direction, core::MapNode *> neighbours =
        node->getNeighbours();
    for (auto it = neighbours.constBegin(); it != neighbours.constEnd(); it++)
    {
        core::MapNode *neighbour = it.value();
        core::MapNode::Direction dir = it.key();

        if (neighbour == nullptr || nodesInfo.contains(neighbour))
            continue;

        NodeInfo *nodeInfo = new NodeInfo(neighbour);
        nodeInfo->pos = neighbourPos(pos, dir, tileSize);
        nodesInfo.insert(neighbour, nodeInfo);

        positionNodes(neighbour, nodesInfo, tileSize);
    }
}

QRect calculateBoundingRect(
    QHash<const core::MapNode *, NodeInfo *> &nodesInfo,
    const QSize &tileSize
)
{
    if (nodesInfo.size() == 0)
        return QRect(0, 0, 0, 0);

    NodeInfo *nodeInfo = *(nodesInfo.begin());

    QPoint topLeft = QPoint(nodeInfo->pos);
    QPoint bottomRight = QPoint(nodeInfo->pos);

    for (auto it = nodesInfo.constBegin(); it != nodesInfo.constEnd(); it++)
    {
        const QPoint pos = (*it)->pos;

        topLeft.setX(std::min(pos.x(), topLeft.x()));
        bottomRight.setX(std::max(pos.x(), bottomRight.x()));

        topLeft.setY(std::min(pos.y(), topLeft.y()));
        bottomRight.setY(std::max(pos.y(), bottomRight.y()));
    }

    // x,y is the top-left corner of the node so we need to add the tile
    // size
    bottomRight += QPoint(tileSize.width(), tileSize.height());

    // leave a half-tile padding
    QPoint padding(tileSize.width()/2, tileSize.height()/2);
    topLeft -= padding;
    bottomRight += padding;

    return QRect(topLeft, bottomRight);
}

QPainterPath hexagonPath(const QSize &tileSize)
{
    const int w = tileSize.width();
    const int h = tileSize.height();

    const QPoint p0(0, h/4);
    const QPoint p1(w/2, 0);
    const QPoint p2(w, h/4);
    const QPoint p3(w, 3 * h/4);
    const QPoint p4(w/2, h - 1);
    const QPoint p5(0, 3 * h/4);

    QPainterPath path;

    path.moveTo(p0);
    path.lineTo(p1);
    path.lineTo(p2);
    path.lineTo(p3);
    path.lineTo(p4);
    path.lineTo(p5);
    path.lineTo(p0);

    return path;
}

QPoint project(const QPoint &p, const QRect &r)
{
    QPoint pp(p);
    if (!r.contains(pp))
    {
        pp.setX(std::min(std::max(r.left(), pp.x()), r.right()));
        pp.setY(std::min(std::max(r.top(), pp.y()), r.bottom()));
    }

    return pp;
}

} // namespace ui
} // namespace warmonger
