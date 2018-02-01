/** \file
 * Map-related utility functions.
 */

#ifndef UI_MAP_UTIL_H
#define UI_MAP_UTIL_H

#include <unordered_map>

#include <QMatrix4x4>
#include <QPoint>
#include <QRect>

#include "core/Hexagon.h"
#include "core/Map.h"

class QSGNode;
class QQuickWindow;

namespace warmonger {

namespace core {

class MapNode;
}

namespace ui {

class MapDrawer;
class WorldSurface;

/**
 * The position of the neighbour map-node in the specified direction.
 *
 * \param pos the position of the map-node
 * \param dir the direction of the neighbour
 * \param tileSize the size of the map-nodes
 *
 * \return the position of the neighbour map-node
 */
QPoint neighbourPos(const QPoint& pos, core::Direction dir, const QSize& tileSize);

/**
 * Find the neighbours by position.
 *
 * Return the neighbours that a map-node would have if it would be at position
 * `pos'.
 * \param pos the position
 * \param tileSize the tile-size
 *
 * \return the neighbour at each direction, or nullptr if there is none
 */
core::MapNodeNeighbours neighboursByPos(
    const QPoint& pos, const WorldSurface* worldSurface, const std::unordered_map<core::MapNode*, QPoint>& mapNodesPos);

/**
 * Calculate the position of each map-node.
 *
 * First, the starting map-node is assigned the position of (0,0), then the
 * graph is traversed and each map-node is assigned a position based on it's
 * displacement from it's neighbour with an already known position.
 * The tile-size of the map-nodes is used to calculate the displacement of
 * neighbouring map-nodes relative to each other.
 *
 * \param startNode the starting node
 * \param tileSize the size of the map-nodes
 *
 * \return mapping of map-nodes to their position.
 */
std::unordered_map<core::MapNode*, QPoint> positionMapNodes(core::MapNode* startNode, const QSize& tileSize);

/**
 * Calculate the bounding rectangle of the map-graph.
 *
 * The bounding rectangle is that minimal rectangle which contains all
 * map-nodes.
 *
 * \param nodesPos the map-nodes
 * and their position
 * \param tileSize the tile size of the map-nodes
 *
 * \return the bounding rectangle
 */
QRect calculateBoundingRect(const std::unordered_map<core::MapNode*, QPoint>& nodesPos, const QSize& tileSize);

/**
 * Find node at position pos.
 *
 * \param pos the position
 * \param mapNodesPos the position of map-nodes
 * \param worldSurface the active worldSurface
 *
 * \returns the map-node or nullptr if no map-node was found at the position
 */
core::MapNode* mapNodeAtPos(
    const QPoint& pos, const std::unordered_map<core::MapNode*, QPoint>& mapNodesPos, const WorldSurface* worldSurface);

/**
 * Project point p into the reactangle r.
 *
 * \param p the point
 * \param r the rectangle
 *
 * \returns the projected point
 */
QPoint project(const QPoint& p, const QRect& r);

/**
 * Center content in frame.
 *
 * Calculate the translation matrix necessary to centralize
 * content in frame, where content is the bounding rectangle of some
 * content to be draw and frame is the bounding rect of the drawing
 * area. If content is larger than frame than it will be shrinked to fit
 * (maintaining the aspect ratio) and then centralized in the frame.
 *
 * \param content the bounding rectangle of the content
 * \param frame the available space
 *
 * \return the translation matrix that if used will center content in frame
 */
QMatrix4x4 centerIn(const QRectF& content, const QRectF& frame);

/**
 * Compute the translation matrix necessary to move point to refPoint.
 *
 * \param point the point to move
 * \param refPoint the reference point
 *
 * \return the translation matrix
 */
QMatrix4x4 moveTo(const QPoint& point, const QPoint& refPoint);

/**
 * Draw a rectangle
 *
 * \param rect the rectangle to be drawn
 * \param oldNode the result of the previous draw call if any or nullptr
 *
 * \return the resulting scene-graph node
 */
QSGNode* drawRect(const QRect& rect, QSGNode* oldNode);

} // namespace ui
} // namespace warmonger

#endif // UI_MAP_UTIL_H
