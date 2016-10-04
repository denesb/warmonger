#ifndef W_UI_CAMPAIGN_MAP_DRAWER_H
#define W_UI_CAMPAIGN_MAP_DRAWER_H

class QSGNode;

#include "core/CampaignMap.h"

namespace warmonger {
namespace ui {

class CampaignMapDrawer
{
public:
    /**
     * Draw a map-node and all content on it
     * @param[in] core::MapNode* mapNode the map-node to draw
     * @param[in] core::Settlement* settlement the settlement on the map-node
     * or nullptr
     * @param[in] core::Army* army the army on the map-node or nullptr
     * @param[in] oldNode a QSGNode that can be reused while drawing, if this
     * is not nullptr try to make as few modifications as possible.
     * @returns QSGNode* the result scene-graph node
     */
    virtual QSGNode* drawContent(const core::CampaignMap::Content& content, QSGNode* oldNode) = 0;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_CAMPAIGN_MAP_DRAWER_H
