
#include "ui/MapUtil.h"
#include "core/WorldSurface.h"

namespace warmonger {
namespace ui {

void positionNode(core::MapNode *node, std::map<const core::MapNode *, QPoint> &nodesPos, const QSize &tileSize);

QPoint neighbourPos(const QPoint &pos, core::Direction dir, const QSize &tileSize)
{
    QSize displacement(0, 0);

    switch(dir)
    {
    case core::Direction::West:
        displacement.setWidth(-tileSize.width());
        break;
    case core::Direction::NorthWest:
        displacement.setWidth(-tileSize.width()/2);
        displacement.setHeight(-tileSize.height() * 3/4);
        break;
    case core::Direction::NorthEast:
        displacement.setWidth(tileSize.width()/2);
        displacement.setHeight(-tileSize.height() * 3/4);
        break;
    case core::Direction::East:
        displacement.setWidth(tileSize.width());
        break;
    case core::Direction::SouthEast:
        displacement.setWidth(tileSize.width()/2);
        displacement.setHeight(tileSize.height() * 3/4);
        break;
    case core::Direction::SouthWest:
        displacement.setWidth(-tileSize.width()/2);
        displacement.setHeight(tileSize.height() * 3/4);
        break;
    }

    return QPoint(
        pos.x() + displacement.width(),
        pos.y() + displacement.height()
    );
}

std::map<const core::MapNode *, QPoint> positionNodes(core::MapNode * startNode, const QSize &tileSize)
{
    std::map<const core::MapNode *, QPoint> nodesPos;

    nodesPos.insert(std::make_pair(startNode, QPoint(0, 0)));

    positionNode(startNode, nodesPos, tileSize);

    return nodesPos;
}

void positionNode(core::MapNode *node, std::map<const core::MapNode *, QPoint> &nodesPos, const QSize &tileSize)
{
    QPoint pos = nodesPos[node];
    std::map<core::Direction, core::MapNode *> neighbours = node->getNeighbours();
    for (const auto &element :  neighbours)
    {
        core::Direction dir = element.first;
        core::MapNode *neighbour = element.second;

        if (neighbour == nullptr || nodesPos.find(neighbour) != nodesPos.end())
            continue;

        nodesPos.insert(std::make_pair(neighbour, neighbourPos(pos, dir, tileSize)));

        positionNode(neighbour, nodesPos, tileSize);
    }
}

QRect calculateBoundingRect(std::map<const core::MapNode *, QPoint> &nodesPos, const QSize &tileSize)
{
    if (nodesPos.size() == 0)
        return QRect(0, 0, 0, 0);

    QPoint bpos = nodesPos.begin()->second;
    QPoint topLeft = bpos;
    QPoint bottomRight = bpos;

    for (const auto &element : nodesPos)
    {
        QPoint pos = element.second;
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

std::pair<qreal, QPointF> centerIn(const QRectF &content, const QRectF &frame)
{
    const qreal contentSize = std::max(content.width(), content.height());
    const qreal frameSize = std::min(frame.width(), frame.height());

    const qreal scale = frameSize / contentSize;

    const QSizeF scaledContentSize = QSizeF(content.size()) * scale;

    const qreal dx = (frame.width() - scaledContentSize.width()) / 2.0;
    const qreal dy = (frame.height() - scaledContentSize.height()) / 2.0;

    QPointF translate = QPointF(-content.topLeft());
    translate += (QPointF(dx, dy) * (1 / scale));

    return std::pair<qreal, QPointF>(scale, translate);
}

} // namespace ui
} // namespace warmonger
