/** \file
 * CampaignMapPreview class.
 *
 * \copyright (C) 2015-2016 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef W_UI_CAMPAIGN_MAP_PREVIEW_H
#define W_UI_CAMPAIGN_MAP_PREVIEW_H

#include "core/CampaignMap.h"
#include "ui/BasicMap.h"
#include "ui/CampaignMapDrawer.h"
#include "ui/WorldSurface.h"

namespace warmonger {
namespace ui {

class CampaignMapWatcher;

/**
 * Presents a non-interactive preview of the campaign-map.
 *
 * For the map to be actually drawn the CampaignMapPreview needs a campaign-map
 * and a matching, actvivated  world-surface (that belongs to the same world)
 * set. When these conditions are not met the QQuickItem::ItemHasContents flag
 * is unset and nothing is going to be drawn on the screen.
 *
 * \see CampaignMapPreview::setCampaignMap
 * \see CampaignMapPreview::setWorldSurface
 * \see core::CampaignMap
 * \see WorldSurface
 */
class CampaignMapPreview : public BasicMap, public CampaignMapDrawer
{
    Q_OBJECT
    Q_PROPERTY(
        warmonger::core::CampaignMap* campaignMap READ getCampaignMap WRITE setCampaignMap NOTIFY campaignMapChanged)
    Q_PROPERTY(WorldSurface* worldSurface READ getWorldSurface WRITE setWorldSurface NOTIFY worldSurfaceChanged)

public:
    /**
     * Constructs an empty campaign-map preview.
     */
    CampaignMapPreview(QQuickItem* parent = nullptr);

    /**
     * Get the shown campaign-map
     *
     * The CampaignMapPreview does not own the campaign-map!
     *
     * \return the campaign-map
     */
    core::CampaignMap* getCampaignMap() const
    {
        return this->campaignMap;
    }

    /**
     * Set the campaign map to show.
     *
     * If all conditions are given for drawing the map, this will trigger a
     * redraw. If this was the missing piece it will trigger the first drawing.
     * The CampaignMapPreview does not assume ownership of the campaign-map!
     * Will emit the signal CampaignMapPreview::campaignMapChanged() if the newly
     * set value is different than the current one.
     *
     * \param campaignMap the campaign-map
     */
    void setCampaignMap(core::CampaignMap* campaignMap);

    /**
     * Get the world-surface used for drawing the campign-map.
     *
     * The CampaignMapPreview does not own the world-surface!
     *
     * \return the world-surface
     */
    WorldSurface* getWorldSurface() const
    {
        return this->worldSurface;
    }

    /**
     * Set the world-surface used for drawing the map.
     *
     * If all conditions are given for drawing the map, this will trigger a
     * redraw. If this was the missing piece it will trigger the first drawing.
     * The CampaignMapPreview does not assume ownership of the world-surface!
     * Will emit the signal CampaignMapPreview::worldSurfaceChanged() if the
     * newly set value is different than the current one.
     *
     * \param worldSurface the world-surface
     */
    void setWorldSurface(WorldSurface* worldSurface);

    /**
     * Update the scene-graph.
     *
     * \see QQuickItem::updatePaintNode()
     */
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* oldNodeData) override;

    /**
     * Draw the content.
     *
     * \param content - the content to be drawn
     * \param oldNode - the node drawn by the previous call if any
     *
     * \return the drawn node
     */
    QSGNode* drawContent(const core::CampaignMap::Content& content, QSGNode* oldNode) override;

signals:
    /**
     * Emitted when the map-node changes.
     */
    void campaignMapChanged();

    /**
     * Emitted when the world-surface changes.
     */
    void worldSurfaceChanged();

private:
    void updateContent();
    void updateMapRect();
    void onMapNodesChanged();

    core::CampaignMap* campaignMap;
    WorldSurface* worldSurface;
    std::map<core::MapNode*, QPoint> mapNodesPos;

    CampaignMapWatcher* watcher;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_CAMPAIGN_MAP_PREVIEW_H
