#ifndef UI_MAP_UTIL_H
#define UI_MAP_UTIL_H

#include <map>

#include <QMatrix4x4>
#include <QPainterPath>
#include <QPoint>
#include <QRect>

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
 * @param[in] pos the position of the map-node
 * @param[in] dir the direction of the neighbour
 * @param[in] tileSize the size of the map-nodes
 */
QPoint neighbourPos(const QPoint& pos, utils::Direction dir, const QSize& tileSize);

std::map<const core::MapNode*, QPoint> positionMapNodes(const core::MapNode* startNode, const QSize& tileSize);

QRect calculateBoundingRect(const std::map<const core::MapNode*, QPoint>& nodesPos, const QSize& tileSize);

/**
 * The visible map-nodes in the window
 *
 * @param[in] mapNodePos map with the map-nodes and their positions
 * @param[in] tileSize the size of the map-nodes
 * @param[in] window the window
 *
 * @returns std::vector<const core::MapNode*> the list of visible map-nodes
 */
std::vector<const core::MapNode*> visibleMapNodes(
        const std::map<const core::MapNode*, QPoint>& mapNodesPos,
        const QSize& tileSize,
        const QRect& window);

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
 * Draw map-nodes
 *
 * @param[in] mapNodes list of map-nodes to be drawn
 * @param[in] rootNode the root-node of the subtree to be built
 * @param[in] mapNodeDrawer the object which will do the actual drawing
 */
void drawMapNodes(const std::vector<const core::MapNode*>& mapNodes, QSGNode* rootNode, MapDrawer& mapNodeDrawer);

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
        const core::MapNode* mapNode,
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
