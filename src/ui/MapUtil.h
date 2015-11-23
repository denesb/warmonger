#ifndef UI_MAP_UTIL_H
#define UI_MAP_UTIL_H

#include <QHash>
#include <QPainterPath>
#include <QPoint>
#include <QRect>
#include <QSet>

#include "core/MapNode.h"

namespace warmonger {

namespace core {
    class Settlement;
    class Unit;
    class WorldSurface;
}

namespace ui {

QPoint neighbourPos(
    const QPoint &pos,
    core::MapNode::Direction dir,
    const QSize &tileSize
);

QHash<const core::MapNode *, QPoint> positionNodes(
    const QList<core::MapNode *> &nodes,
    const QSize &tileSize
);

QRect calculateBoundingRect(
    QHash<const core::MapNode *, QPoint> &nodesPos,
    const QSize &tileSize
);

QPainterPath hexagonPath(const QSize &tileSize);

QPoint project(const QPoint &p, const QRect &r);

} // namespace ui
} // namespace warmonger

#endif // UI_MAP_UTIL_H
