#ifndef UI_MINI_MAP_H
#define UI_MINI_MAP_H

#include <QHash>
#include <QPainterPath>
#include <QRect>
#include <QSize>
#include <QtQuick/QQuickPaintedItem>

#include "core/Map.h"
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

    Q_PROPERTY(warmonger::core::Map *map READ getMap WRITE setMap NOTIFY mapChanged)
    Q_PROPERTY(QPoint windowPos READ getWindowPos WRITE setWindowPos NOTIFY windowPosChanged)
    Q_PROPERTY(QSize windowSize READ getWindowSize WRITE setWindowSize NOTIFY windowSizeChanged)
public:
    MiniMap(QQuickItem *parent = nullptr);
    ~MiniMap();

    core::Map *getMap() const;
    void setMap(core::Map *map);

    QPoint getWindowPos() const;
    void setWindowPos(const QPoint &windowPos);
    void centerWindow(const QPoint &pos);

    QSize getWindowSize() const;
    void setWindowSize(const QSize& windowSize);

    void paint(QPainter *painter);

signals:
    void mapChanged();
    void windowPosChanged();
    void windowSizeChanged();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void setupMap();
    void updateGeometry();
    void updateWindowPosRect();
    void updateTransform();

    QPointF mapToMap(const QPointF &p);

    void drawNode(QPainter *painter, const core::MapNode *node);
    void drawContent(QPainter *painter, const core::MapNode *node);

    QList<core::MapNode *> nodes;
    core::World *world;
    core::WorldSurface *surface;
    QSize tileSize;

    core::Map *map;
    QHash<const core::MapNode *, QPoint> nodesPos;
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
