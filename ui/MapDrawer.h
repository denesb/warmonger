#ifndef UI_MAP_DRAWER_H
#define UI_MAP_DRAWER_H

#include <QHash>
#include <QPainter>
#include <QPainterPath>
#include <QPoint>

#include "ui/MapUtil.h"

namespace warmonger {

namespace core {
    class Map;
    class MapNode;
    class Unit;
    class World;
    class WorldSurface;
}

namespace ui {

struct DrawingInfo
{
    struct Overlay
    {
        QSet<core::MapNode *> nodes;
        QColor color;
    };

    DrawingInfo() :
        focusedNode(nullptr),
        overlays(),
        movingUnits()
    {}

    core::MapNode *focusedNode;
    QList<Overlay> overlays;
    QHash<const core::Unit *, QPoint> movingUnits;
};

class MapDrawer
{
public:
    MapDrawer(core::Map *map, const QHash<const core::MapNode *, QPoint> &nodesPos);
    ~MapDrawer();

    void drawMap(QPainter *painter, const QList<core::MapNode *> &nodes, const DrawingInfo &drawingInfo) const;

private:
    void drawNodes(QPainter *painter, const QList<core::MapNode *> &nodes) const;
    void drawNode(QPainter *painter, const core::MapNode *node) const;

    void drawNodesGrid(QPainter *painter, const QList<core::MapNode *> &nodes) const;
    void drawNodeGrid(QPainter *painter, const core::MapNode *node) const;

    void drawFocusMark(QPainter *painter, const core::MapNode *node) const;

    void drawNodesSettlements(QPainter *painter, const QList<core::MapNode *> &nodes) const;
    void drawSettlement(QPainter *painter, const core::Settlement *settlement) const;

    void drawOverlays(
        QPainter *painter,
        const QList<core::MapNode *> &nodes,
        const QList<DrawingInfo::Overlay> &overlays
    ) const;
    void drawOverlay(
        QPainter *painter,
        const QList<core::MapNode *> &nodes,
        const DrawingInfo::Overlay &overlay
    ) const;
    void drawNodeOverlay(
        QPainter *painter,
        QColor color,
        const core::MapNode *node
    ) const;

    void drawUnits(
        QPainter *painter,
        const QList<core::MapNode *> &nodes,
        QHash<const core::Unit *, QPoint> movingUnits
    ) const;
    void drawUnit(
        QPainter *painter,
        const QPoint &pos,
        const core::Unit *unit
    ) const;

    core::Map *map;
    core::World *world;
    core::WorldSurface *surface;
    QSize tileSize;
    QHash<const core::MapNode *, QPoint> nodesPos;
    QPainterPath hexagonPainterPath;
};

} // namespace ui
} // namespace warmonger

#endif // UI_MAP_DRAWER_H
