#ifndef UI_MAP_UTIL_H
#define UI_MAP_UTIL_H

#include <map>

#include <QMatrix4x4>
#include <QPoint>
#include <QRect>

#include "core/MapNodeNeighbours.h"
#include "utils/Hexagon.h"

class QSGNode;

namespace warmonger {

namespace core {

class MapNode;
class Settlement;
class Unit;

}

namespace ui {

class MapDrawer;
class WorldSurface;

/**
 * The position of the neighbour map-node in the specified direction
 *
 * @param[in] const QPoint& pos the position of the map-node
 * @param[in] utils::Direction dir the direction of the neighbour
 * @param[in] const QSize& tileSize the size of the map-nodes
 */
QPoint neighbourPos(const QPoint& pos, utils::Direction dir, const QSize& tileSize);

/**
 * Find the neighbours by position
 *
 * Return the neighbours that a map-node would have if it would be at position
 * `pos'.
 * @param[in] const QPoint& pos the position
 * @param[in] const QSize& tileSize the tile-size
 *
 * @returns std::map<utils::Direction, core::MapNode*> the neighbour at each
 * direction, or nullptr if there is none
 */
core::MapNodeNeighbours neighboursByPos(
        const QPoint& pos,
        const WorldSurface* worldSurface,
        const std::map<core::MapNode*, QPoint>& mapNodesPos);

/**
 * Calculate the position of each map-node
 *
 * First, the starting map-node is assigned the position of (0,0), then the
 * graph is traversed and each map-node is assigned a position based on it's
 * displacement from it's neighbour with an already known position.
 * The tile-size of the map-nodes is used to calculate the displacement of
 * neighbouring map-nodes relative to each other.
 *
 * @param[in] core::MapNode* startNode the starting node
 * @param[in] const QSize& tileSize the size of the map-nodes
 *
 * @returns std::map<core::MapNode*, QPoint> mapping of map-nodes to their
 * position.
 */
std::map<core::MapNode*, QPoint> positionMapNodes(core::MapNode* startNode, const QSize& tileSize);

/**
 * Calculate the bounding rectangle of the map-graph
 *
 * The bounding rectangle is that minimal rectangle which contains all
 * map-nodes.
 *
 * @param[in] const std::map<core::MapNode*, QPoint>& nodesPos the map-nodes
 * and their position
 * @param[in] const QSize& tileSize the tile size of the map-nodes
 */
QRect calculateBoundingRect(const std::map<core::MapNode*, QPoint>& nodesPos, const QSize& tileSize);

/**
 * The visible map-nodes in the window
 *
 * @param[in] mapNodePos map with the map-nodes and their positions
 * @param[in] tileSize the size of the map-nodes
 * @param[in] window the window
 *
 * @returns std::vector<core::MapNode*> the list of visible map-nodes
 */
std::vector<core::MapNode*> visibleMapNodes(
        const std::map<core::MapNode*, QPoint>& mapNodesPos,
        const QSize& tileSize,
        const QRect& window);

/**
 * Find node at position pos.
 *
 * @param[in] const QPoint& pos the position
 * @param[in] const std::map<core::MapNode*, QPoint>& mapNodesPos the
 * position of map-nodes
 * @param[in] const WorldSurface* worldSurface the active worldSurface
 *
 * @returns core::MapNode* the map-node or nullptr if no map-node was found at
 * the position.
 */
core::MapNode* mapNodeAtPos(
        const QPoint& pos,
        const std::map<core::MapNode*, QPoint>& mapNodesPos,
        const WorldSurface* worldSurface);

QPoint project(const QPoint& p, const QRect& r);

/**
 * Center content in frame.
 *
 * Calculate the translation matrix necessary to centralize
 * content in frame, where content is the bounding rectangle of some
 * content to be draw and frame is the bounding rect of the drawing
 * area. If content is larger than frame than it will be shrinked to fit
 * (maintaining the aspect ratio) and then centralized in the frame.
 */
QMatrix4x4 centerIn(const QRectF& content, const QRectF& frame);

/**
 * Compute the translation matrix necessary to move point p1 to point p2.
 *
 * @param[in] QPoint point the point to move
 * @param[in] QPoint refPoint the reference point
 *
 * @returns QMatrix4x4 the translation matrix
 */
QMatrix4x4 moveTo(const QPoint& point, const QPoint& refPoint);

/**
 * Draw map-nodes
 *
 * @param[in] mapNodes list of map-nodes to be drawn
 * @param[in] rootNode the root-node of the subtree to be built
 * @param[in] mapNodeDrawer the object which will do the actual drawing
 */
void drawMapNodes(const std::vector<core::MapNode*>& mapNodes, QSGNode* rootNode, MapDrawer& mapNodeDrawer);

/**
 * Draw a map-node
 *
 * @param[in] mapNode the map-node to be drawn
 * @param[in] worldSurface the actual world surface (contains the textures)
 * @param[in] pos the position of the node
 * @param[in] oldNode the result of the previous draw call if any or nullptr
 *
 * @returns QSGNode* the resulting scene-graph node
 */
QSGNode* drawMapNode(
        core::MapNode* mapNode,
        const ui::WorldSurface* worldSurface,
        const QPoint& pos,
        QSGNode* oldNode);

/**
 * Draw a rectangle
 *
 * @param[in] rect the rectangle to be drawn
 * @param[in] oldNode the result of the previous draw call if any or nullptr
 *
 * @returns QSGNode* the resulting scene-graph node
 */
QSGNode* drawRect(const QRect& rect, QSGNode* oldNode);

} // namespace ui
} // namespace warmonger

#endif // UI_MAP_UTIL_H
