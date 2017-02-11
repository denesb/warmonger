/** \file
 * CampaignMiniMap class.
 *
 * \copyright (C) 2015-2017 Botond Dénes
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

#ifndef W_UI_CAMPAIGN_MINI_MAP_H
#define W_UI_CAMPAIGN_MINI_MAP_H

#include <map>

#include "core/CampaignMap.h"
#include "ui/BasicMiniMap.h"
#include "ui/CampaignMapDrawer.h"
#include "ui/WorldSurface.h"

namespace warmonger {

namespace core {
class MapNode;
class World;
}

namespace ui {

class CampaignMapWatcher;

class CampaignMiniMap : public BasicMiniMap, public CampaignMapDrawer
{
    Q_OBJECT

    Q_PROPERTY(
        warmonger::core::CampaignMap* campaignMap READ getCampaignMap WRITE setCampaignMap NOTIFY campaignMapChanged)
    Q_PROPERTY(
        warmonger::ui::WorldSurface* worldSurface READ getWorldSurface WRITE setWorldSurface NOTIFY worldSurfaceChanged)
public:
    CampaignMiniMap(QQuickItem* parent = nullptr);

    core::CampaignMap* getCampaignMap() const;
    void setCampaignMap(core::CampaignMap* campaignMap);

    WorldSurface* getWorldSurface() const;
    void setWorldSurface(WorldSurface* worldSurface);

    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data) override;

    QSGNode* drawContent(const core::CampaignMap::Content& content, QSGNode* oldNode) override;

signals:
    void campaignMapChanged();
    void worldSurfaceChanged();
    void windowRectChanged();

private:
    void updateContent();
    void updateMapRect();
    void onMapNodesChanged();

    WorldSurface* worldSurface;
    core::CampaignMap* campaignMap;
    std::map<core::MapNode*, QPoint> mapNodesPos;

    CampaignMapWatcher* watcher;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_CAMPAIGN_MINI_MAP_H
