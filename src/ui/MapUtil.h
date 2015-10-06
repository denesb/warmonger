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
}

namespace ui {

struct NodeInfo
{
    NodeInfo(core::MapNode *node) :
        pos(),
        node(node),
        settlement(nullptr),
        unit(nullptr)
    {}

    QPoint pos;
    core::MapNode *node;
    core::Settlement *settlement;
    core::Unit *unit;
};

QPoint neighbourPos(
    const QPoint &pos,
    core::MapNode::Direction dir,
    const QSize &tileSize
);

void positionNodes(
    core::MapNode *node,
    QHash<const core::MapNode *, NodeInfo *> &nodesInfo,
    const QSize &tileSize
);

QRect calculateBoundingRect(
    QHash<const core::MapNode *, NodeInfo *> &nodesInfo,
    const QSize &tileSize
);

QPainterPath hexagonPath(const QSize &tileSize);

} // namespace ui
} // namespace warmonger

#endif // UI_MAP_UTIL_H
