#ifndef UI_MINI_MAP_H
#define UI_MINI_MAP_H

#include <map>
#include <vector>

#include <QRect>
#include <QSize>
#include <QtQuick/QQuickItem>

#include "core/CampaignMap.h"
#include "ui/MapUtil.h"
#include "ui/WorldSurface.h"

namespace warmonger {

namespace core {
    class MapNode;
    class World;
}

namespace ui {

class MiniMap :
    public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(warmonger::core::CampaignMap *campaignMap READ getCampaignMap WRITE setCampaignMap NOTIFY campaignMapChanged)
    Q_PROPERTY(warmonger::ui::WorldSurface *worldSurface READ getWorldSurface WRITE setWorldSurface NOTIFY worldSurfaceChanged)
    Q_PROPERTY(QPoint windowPos READ getWindowPos WRITE setWindowPos NOTIFY windowPosChanged)
    Q_PROPERTY(QSize windowSize READ getWindowSize WRITE setWindowSize NOTIFY windowSizeChanged)
public:
    MiniMap(QQuickItem *parent = nullptr);

    core::CampaignMap * getCampaignMap() const;
    void setCampaignMap(core::CampaignMap *campaignMap);

    WorldSurface * getWorldSurface() const;
    void setWorldSurface(WorldSurface *worldSurface);

    QPoint getWindowPos() const;
    void setWindowPos(const QPoint &windowPos);
    void centerWindow(const QPoint &pos);

    QSize getWindowSize() const;
    void setWindowSize(const QSize& windowSize);

    QSGNode * updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;

signals:
    void campaignMapChanged();
    void worldSurfaceChanged();
    void windowPosChanged();
    void windowSizeChanged();

protected:
    void updateContent();
    /*
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    */

private:
    void setupMap();
    /*
    void updateGeometry();
    void updateWindowPosRect();
    void updateTransform();

    QPointF mapToMap(const QPointF &p);

    void drawNode(QPainter *painter, const core::MapNode *node);
    void drawContent(QPainter *painter, const core::MapNode *node);
    */

    std::vector<core::MapNode *> nodes;
    core::World *world;
    WorldSurface *worldSurface;
    QSize tileSize;

    core::CampaignMap *campaignMap;
    std::map<const core::MapNode *, QPoint> nodesPos;
    //QRect boundingRect;
    QPainterPath hexagonPainterPath;
    QRect windowPosRect;
    QPoint windowPos;
    QSize windowSize;

    qreal scale;
    QPointF translate;
};

} // namespace ui
} // namespace warmonger

#endif // UI_MINI_MAP_H
