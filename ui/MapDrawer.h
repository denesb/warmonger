#ifndef W_UI_MAP_DRAWER_H
#define W_UI_MAP_DRAWER_H

#include <QSGNode>

#include "core/MapNode.h"

namespace warmonger {
namespace ui {

class MapDrawer
{
public:

    /**
     * Draw a map-node and all content on it
     * @param[in] mapNode the map-node to draw
     * @param[in] oldNode a QSGNode that can be reused while drawing, if this
     * is not nullptr try to make as few modifications as possible.
     * @returns QSGNode* the result scene-graph node
     */
    virtual QSGNode* drawMapNodeAndContents(const core::MapNode* mapNode, QSGNode* oldNode) = 0;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_MAP_DRAWER_H
