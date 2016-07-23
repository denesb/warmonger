#ifndef W_UI_CAMPAIGN_MINI_MAP_H
#define W_UI_CAMPAIGN_MINI_MAP_H

#include <map>

#include "core/CampaignMap.h"
#include "ui/BasicMiniMap.h"
#include "ui/MapDrawer.h"
#include "ui/WorldSurface.h"

namespace warmonger {

namespace core {
    class MapNode;
    class World;
}

namespace ui {

class CampaignMiniMap :
    public BasicMiniMap,
    public MapDrawer
{
    Q_OBJECT

    Q_PROPERTY(warmonger::core::CampaignMap* campaignMap READ getCampaignMap WRITE setCampaignMap NOTIFY campaignMapChanged)
    Q_PROPERTY(warmonger::ui::WorldSurface* worldSurface READ getWorldSurface WRITE setWorldSurface NOTIFY worldSurfaceChanged)
public:
    CampaignMiniMap(QQuickItem* parent = nullptr);

    core::CampaignMap* getCampaignMap() const;
    void setCampaignMap(core::CampaignMap* campaignMap);

    WorldSurface* getWorldSurface() const;
    void setWorldSurface(WorldSurface* worldSurface);

    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data) override;

    QSGNode* drawMapNodeAndContents(const core::MapNode* mapNode, QSGNode* oldNode) override;

signals:
    void campaignMapChanged();
    void worldSurfaceChanged();
    void windowRectChanged();

private:
    void updateContent();
    void updateMapRect();

    WorldSurface* worldSurface;
    core::CampaignMap* campaignMap;
    std::map<const core::MapNode*, QPoint> mapNodesPos;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_CAMPAIGN_MINI_MAP_H
