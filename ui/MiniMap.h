#ifndef UI_MINI_MAP_H
#define UI_MINI_MAP_H

#include <map>
#include <vector>

#include <QPainterPath>
#include <QRect>
#include <QSize>
#include <QtQuick/QQuickPaintedItem>

#include "core/CampaignMap.h"
#include "ui/MapUtil.h"

namespace warmonger {

namespace core {
    class MapNode;
    class World;
    class WorldSurface;
}

namespace ui {

class MiniMap :
    public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(warmonger::core::CampaignMap *map READ getMap WRITE setMap NOTIFY mapChanged)
    Q_PROPERTY(QPoint windowPos READ getWindowPos WRITE setWindowPos NOTIFY windowPosChanged)
    Q_PROPERTY(QSize windowSize READ getWindowSize WRITE setWindowSize NOTIFY windowSizeChanged)
public:
    MiniMap(QQuickItem *parent = nullptr);
    ~MiniMap();

    core::CampaignMap *getMap() const;
    void setMap(core::CampaignMap *map);

    QPoint getWindowPos() const;
    void setWindowPos(const QPoint &windowPos);
    void centerWindow(const QPoint &pos);

    QSize getWindowSize() const;
    void setWindowSize(const QSize& windowSize);

    void paint(QPainter *painter) override;

signals:
    void mapChanged();
    void windowPosChanged();
    void windowSizeChanged();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void setupMap();
    void updateGeometry();
    void updateWindowPosRect();
    void updateTransform();

    QPointF mapToMap(const QPointF &p);

    void drawNode(QPainter *painter, const core::MapNode *node);
    void drawContent(QPainter *painter, const core::MapNode *node);

    std::vector<core::MapNode *> nodes;
    core::World *world;
    core::WorldSurface *surface;
    QSize tileSize;

    core::CampaignMap *map;
    std::map<const core::MapNode *, QPoint> nodesPos;
    QRect boundingRect;
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
