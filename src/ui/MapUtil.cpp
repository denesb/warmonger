#include <QSize>

#include "core/MapNode.h"
#include "ui/MapUtil.h"

namespace warmonger {
namespace ui {

void update(
    QHash<const core::MapNode *, QPoint> &hash1,
    const QHash<const core::MapNode *, QPoint> &hash2
)
{
    QHash<const core::MapNode *, QPoint>::ConstIterator it;
    for (it = hash2.constBegin(); it != hash2.constEnd(); it++)
    {
        hash1.insert(it.key(), it.value());
    }
}

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

QHash<const core::MapNode *, QPoint> positionNodes(
    const core::MapNode *node,
    const QPoint &pos,
    const QSize &tileSize,
    QSet<const core::MapNode *> &visitedNodes
)
{
    QHash<const core::MapNode *, QPoint> positions;
    positions[node] = pos;
    visitedNodes << node;

    QHash<core::MapNode::Direction, core::MapNode *> neighbours =
        node->getNeighbours();
    QHash<core::MapNode::Direction, core::MapNode *>::ConstIterator it;
    for (it = neighbours.constBegin(); it != neighbours.constEnd(); it++)
    {
        core::MapNode *neighbour = it.value();
        core::MapNode::Direction dir = it.key();

        if (neighbour == nullptr || visitedNodes.contains(neighbour))
            continue;

        QPoint nPos = neighbourPos(pos, dir, tileSize);
        QHash<const core::MapNode *, QPoint> neighbourPositions = positionNodes(
            neighbour,
            nPos,
            tileSize,
            visitedNodes
        );
        update(positions, neighbourPositions);
    }

    return positions;
}

QRect calculateBoundingRect(
    const QList<core::MapNode *> &nodes,
    const QHash<const core::MapNode *, QPoint> &nodePos,
    const QSize &tileSize
)
{
    if (nodes.size() == 0)
        return QRect(0, 0, 0, 0);

    QPoint pos = nodePos[nodes[0]];

    QPoint topLeft = QPoint(pos);
    QPoint bottomRight = QPoint(pos);

    for (const core::MapNode *node : nodes)
    {
        pos = nodePos[node];

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

    return path;
}

} // namespace ui
} // namespace warmonger
