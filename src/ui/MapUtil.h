#ifndef UI_MAP_UTIL_H
#define UI_MAP_UTIL_H

#include <QHash>
#include <QPainterPath>
#include <QPoint>
#include <QRect>
#include <QSet>

namespace warmonger {

namespace core {
    class MapNode;
}

namespace ui {

void update(
    QHash<const core::MapNode *, QPoint> &hash1,
    const QHash<const core::MapNode *, QPoint> &hash2
);

QPoint neighbourPos(
    const QPoint &pos,
    core::MapNode::Direction dir,
    const QSize &tileSize
);

QHash<const core::MapNode *, QPoint> positionNodes(
    const core::MapNode *node,
    const QPoint &pos,
    const QSize &tileSize,
    QSet<const core::MapNode *> &visitedNodes
);

QRect calculateBoundingRect(
    const QList<core::MapNode *> &nodes,
    const QHash<const core::MapNode *, QPoint> &nodePos,
    const QSize &tileSize
);

QPainterPath hexagonPath(const QSize &tileSize);

} // namespace ui
} // namespace warmonger

#endif // UI_MAP_UTIL_H
