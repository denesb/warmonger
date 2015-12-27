#ifndef UI_MAP_PREVIEW_H
#define UI_MAP_PREVIEW_H

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

/**
 * Show a preview of a map.
 *
 * The map preview is very similar to the mini-map, showing the whole
 * map scaled down to fit into the available space.
 */
class MapPreview : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(warmonger::core::Map *map READ getMap WRITE setMap NOTIFY mapChanged)
public:
    MapPreview(QQuickItem *parent = nullptr);
    ~MapPreview();

    core::Map *getMap() const;
    void setMap(core::Map *map);

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
    QHash<const core::MapNode *, QPoint> nodesPos;
    QRect boundingRect;
    QPainterPath hexagonPainterPath;

    qreal scale;
    QPointF translate;
};

} // namespace ui
} // namespace warmonger

#endif // UI_MAP_PREVIEW_H
