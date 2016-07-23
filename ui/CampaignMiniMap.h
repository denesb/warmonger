#ifndef W_UI_CAMPAIGN_MINI_MAP_H
#define W_UI_CAMPAIGN_MINI_MAP_H

#include <map>
#include <vector>

#include <QMatrix4x4>
#include <QRect>
#include <QSize>
#include <QtQuick/QQuickItem>

#include "core/CampaignMap.h"
#include "ui/MapDrawer.h"
#include "ui/MapWindow.h"
#include "ui/WorldSurface.h"

namespace warmonger {

namespace core {
    class MapNode;
    class World;
}

namespace ui {

class CampaignMiniMap :
    public QQuickItem,
    public MapDrawer
{
    Q_OBJECT

    Q_PROPERTY(warmonger::core::CampaignMap* campaignMap READ getCampaignMap WRITE setCampaignMap NOTIFY campaignMapChanged)
    Q_PROPERTY(warmonger::ui::WorldSurface* worldSurface READ getWorldSurface WRITE setWorldSurface NOTIFY worldSurfaceChanged)
    Q_PROPERTY(QRect windowRect READ getWindowRect WRITE setWindowRect NOTIFY windowRectChanged)
public:
    CampaignMiniMap(QQuickItem* parent = nullptr);

    core::CampaignMap* getCampaignMap() const;
    void setCampaignMap(core::CampaignMap* campaignMap);

    WorldSurface* getWorldSurface() const;
    void setWorldSurface(WorldSurface* worldSurface);

    QRect getWindowRect() const;
    void setWindowRect(const QRect& windowRect);

    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data) override;

    QSGNode* drawMapNodeAndContents(const core::MapNode* mapNode, QSGNode* oldNode) override;

signals:
    void campaignMapChanged();
    void worldSurfaceChanged();
    void windowRectChanged();

protected:
    void updateContent();
    /*
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    */

private:
    void setupMap();
    void updateTransform();
    /*
    void updateWindowRectRect();
    */

    WorldSurface* worldSurface;
    core::CampaignMap* campaignMap;

    std::map<const core::MapNode*, QPoint> mapNodesPos;
    QMatrix4x4 transform;
    MapWindow mapWindow;
    QRect viewWindowRect;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_CAMPAIGN_MINI_MAP_H
