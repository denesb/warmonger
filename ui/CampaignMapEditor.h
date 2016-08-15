#ifndef UI_CAMPAIGN_MAP_EDITOR_H
#define UI_CAMPAIGN_MAP_EDITOR_H

#include <map>

#include "core/CampaignMap.h"
#include "ui/BasicMap.h"
#include "ui/MapDrawer.h"
#include "ui/WorldSurface.h"

namespace warmonger {
namespace ui {

class CampaignMapEditor :
    public BasicMap,
    public MapDrawer
{
    Q_OBJECT
    Q_PROPERTY(warmonger::core::CampaignMap* campaignMap READ getCampaignMap WRITE setCampaignMap NOTIFY campaignMapChanged)
    Q_PROPERTY(warmonger::ui::WorldSurface* worldSurface READ getWorldSurface WRITE setWorldSurface NOTIFY worldSurfaceChanged)

public:
    CampaignMapEditor(QQuickItem *parent = nullptr);

    core::CampaignMap* getCampaignMap() const;
    void setCampaignMap(core::CampaignMap* campaignMap);

    WorldSurface* getWorldSurface() const;
    void setWorldSurface(WorldSurface* worldSurface);

    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* oldNodeData) override;

    QSGNode* drawMapNodeAndContents(const core::MapNode* mapNode, QSGNode* oldNode) override;

signals:
    void campaignMapChanged();
    void worldSurfaceChanged();

protected:
    void hoverMoveEvent(QHoverEvent* event) override;

private:
    void updateContent();
    void updateMapRect();

    core::CampaignMap *campaignMap;
    WorldSurface *worldSurface;
    std::map<const core::MapNode*, QPoint> mapNodesPos;
    core::MapNode* currentMapNode;
};

} // namespace ui
} // namespace warmonger

#endif // UI_CAMPAIGN_MAP_EDITOR_H
