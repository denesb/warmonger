#ifndef UI_MAP_PREVIEW_H
#define UI_MAP_PREVIEW_H

#include <QHash>
#include <QPainterPath>
#include <QRect>
#include <QSize>
#include <QtQuick/QQuickPaintedItem>

#include "ui/MapUtil.h"

namespace warmonger {

namespace core {
    class Map;
    class MapNode;
    class World;
    class WorldSurface;
}

namespace ui {

class MapPreview :
    public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QObject *map READ readMap WRITE writeMap NOTIFY mapChanged)
public:
    MapPreview(QQuickItem *parent = nullptr);
    ~MapPreview();

    core::Map *getMap() const;
    void setMap(core::Map *map);
    QObject *readMap() const;
    void writeMap(QObject *map);

    void paint(QPainter *painter);

signals:
    void mapChanged();

protected:

private:
    void setupMap();
    void updateGeometry();
    void updateTransform();

    void drawNode(QPainter *painter, const core::MapNode *node);

    QList<core::MapNode *> nodes;
    core::World *world;
    core::WorldSurface *surface;
    QSize tileSize;

    core::Map *map;
    QHash<const core::MapNode *, NodeInfo *> nodesInfo;
    QRect boundingRect;
    QPainterPath hexagonPainterPath;

    qreal scale;
    QPointF translate;
};

} // namespace ui
} // namespace warmonger

#endif // UI_MAP_PREVIEW_H
