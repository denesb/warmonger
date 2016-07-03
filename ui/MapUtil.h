#ifndef UI_MAP_UTIL_H
#define UI_MAP_UTIL_H

#include <map>

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

QPoint neighbourPos(const QPoint &pos, core::Direction dir, const QSize &tileSize);

std::map<const core::MapNode *, QPoint> positionNodes(core::MapNode * startNode, const QSize &tileSize);

QRect calculateBoundingRect(std::map<const core::MapNode *, QPoint> &nodesPos, const QSize &tileSize);

QPainterPath hexagonPath(const QSize &tileSize);

QPoint project(const QPoint &p, const QRect &r);

/**
 * Center content in frame.
 *
 * Calculate the translation and scaling necessary to centralize
 * content in frame, where content is the bounding rectangle of some
 * content to be draw and frame is the bounding rect of the drawing
 * area.
 * The scale and translate values are ment to be used to scale and
 * translate a graphic scene (first scale and then translate!).
 */
std::pair<qreal, QPointF> centerIn(const QRectF &content, const QRectF &frame);

} // namespace ui
} // namespace warmonger

#endif // UI_MAP_UTIL_H
