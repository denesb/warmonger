#ifndef UI_CAMPAIGN_MAP_EDITOR_H
#define UI_CAMPAIGN_MAP_EDITOR_H

#include <map>

#include <QtQuick/QQuickItem>
#include <QPoint>

#include "core/CampaignMap.h"
#include "ui/MapWindow.h"
#include "ui/WorldSurface.h"

namespace warmonger {
namespace ui {

class CampaignMapEditor :
    public QQuickItem
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

    QRect getWindowRect() const;

    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* oldNodeData) override;

signals:
    void campaignMapChanged();
    void worldSurfaceChanged();
    void windowRectChanged();

private:
    void updateContent();
    void updateGeometry();

    QSGNode* drawNode(QSGNode* old, const core::MapNode* mapNode) const;

    core::CampaignMap *campaignMap;
    WorldSurface *worldSurface;

    MapWindow mapWindow;
    std::map<const core::MapNode*, QPoint> mapNodesPos;
};

} // namespace ui
} // namespace warmonger

#endif // UI_CAMPAIGN_MAP_EDITOR_H
