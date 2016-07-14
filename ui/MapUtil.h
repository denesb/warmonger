#ifndef UI_MAP_UTIL_H
#define UI_MAP_UTIL_H

#include <map>

#include <QMatrix4x4>
#include <QPainterPath>
#include <QPoint>
#include <QRect>

#include "core/MapNode.h"

namespace warmonger {

namespace core {

class Settlement;
class Unit;

}

namespace ui {

QPoint neighbourPos(const QPoint &pos, utils::Direction dir, const QSize &tileSize);

std::map<const core::MapNode *, QPoint> positionNodes(core::MapNode * startNode, const QSize &tileSize);

QRect calculateBoundingRect(std::map<const core::MapNode *, QPoint> &nodesPos, const QSize &tileSize);

QPainterPath hexagonPath(const QSize &tileSize);

QPoint project(const QPoint &p, const QRect &r);

/**
 * Center content in frame.
 *
 * Calculate the translation matrix necessary to centralize
 * content in frame, where content is the bounding rectangle of some
 * content to be draw and frame is the bounding rect of the drawing
 * area. If content is larger than frame than it will be shrinked to fit
 * (maintaining the aspect ratio) and then centralized in the frame.
 */
QMatrix4x4 centerIn(const QRectF &content, const QRectF &frame);

} // namespace ui
} // namespace warmonger

#endif // UI_MAP_UTIL_H
