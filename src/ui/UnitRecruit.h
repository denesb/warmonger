#ifndef UI_UNIT_RECRUIT_H
#define UI_UNIT_RECRUIT_H

#include <QHash>
#include <QLineF>
#include <QRect>
#include <QtQuick/QQuickPaintedItem>

#include "core/Game.h"
#include "core/MapNode.h"
#include "core/Settlement.h"
#include "core/Unit.h"
#include "ui/MapDrawer.h"

namespace warmonger {

namespace core {
    class World;
    class WorldSurface;
}

namespace ui {

class MapDrawer;

class UnitRecruit :
    public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(warmonger::core::Game *game READ getGame WRITE setGame NOTIFY gameChanged)
    Q_PROPERTY(warmonger::core::Settlement *settlement READ getSettlement WRITE setSettlement NOTIFY settlementChanged)

public:
    UnitRecruit(QQuickItem *parent = nullptr);
    ~UnitRecruit();

    core::Game * getGame() const;
    void setGame(core::Game *game);

    core::Settlement * getSettlement() const;
    void setSettlement(core::Settlement *settlement);

    void paint(QPainter *painter);

signals:
    void gameChanged();
    void settlementChanged();

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    void setupMap();
    void updateGeometry();
    void updateTransform();
    bool rectContainsNode(const QRect &rect, const core::MapNode *node);
    QPointF mapToMap(const QPointF &p) const;

    void onFocusedNodeChanged();
    void onCurrentNodeChanged();

    void updateFocus(const QPoint &p);
    void moveUnit(const QPoint &p);
    void advanceUnits();

    core::Game *game;
    core::World *world;
    core::WorldSurface *surface;
    QSize tileSize;

    core::Settlement *settlement;

    MapDrawer *mapDrawer;

    QList<core::MapNode *> nodes;
    QHash<const core::MapNode *, QPoint> nodesPos;

    core::MapNode *focusedNode;
    core::MapNode *currentNode;

    QRect boundingRect;
    QPoint lastPos;

    qreal scale;
    QPointF translate;
};

} // namespace ui
} // namespace warmonger

#endif // UI_UNIT_RECRUIT_H
