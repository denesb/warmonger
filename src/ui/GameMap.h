#ifndef UI_GAME_MAP_H
#define UI_GAME_MAP_H

#include <QHash>
#include <QLineF>
#include <QPainterPath>
#include <QRect>
#include <QtQuick/QQuickPaintedItem>

#include "ui/MapUtil.h"

namespace warmonger {

namespace core {
    class Game;
    class MapNode;
    class Unit;
    class World;
    class WorldSurface;
}

namespace ui {

struct MovingUnit;

class GameMap :
    public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QObject *game READ readGame WRITE writeGame NOTIFY gameChanged)
    Q_PROPERTY(QObject *focusedMapNode READ readFocusedMapNode NOTIFY focusedMapNodeChanged)
    Q_PROPERTY(QObject *focusedSettlement READ readFocusedSettlement NOTIFY focusedSettlementChanged)
    Q_PROPERTY(QObject *focusedUnit READ readFocusedUnit NOTIFY focusedUnitChanged)
    Q_PROPERTY(QObject *currentMapNode READ readCurrentMapNode NOTIFY currentMapNodeChanged)
    Q_PROPERTY(QObject *currentSettlement READ readCurrentSettlement NOTIFY currentSettlementChanged)
    Q_PROPERTY(QObject *currentUnit READ readCurrentUnit NOTIFY currentUnitChanged)
    Q_PROPERTY(QPoint windowPos READ getWindowPos WRITE setWindowPos NOTIFY windowPosChanged)
    Q_PROPERTY(QSize windowSize READ getWindowSize NOTIFY windowSizeChanged)
public:
    GameMap(QQuickItem *parent = nullptr);
    ~GameMap();

    core::Game *getGame() const;
    void setGame(core::Game *game);
    QObject *readGame() const;
    void writeGame(QObject *game);

    core::MapNode * getFocusedMapNode() const;
    QObject * readFocusedMapNode() const;

    core::Settlement * getFocusedSettlement() const;
    QObject * readFocusedSettlement() const;

    core::Unit * getFocusedUnit() const;
    QObject * readFocusedUnit() const;

    core::MapNode * getCurrentMapNode() const;
    QObject * readCurrentMapNode() const;

    core::Settlement * getCurrentSettlement() const;
    QObject * readCurrentSettlement() const;

    core::Unit * getCurrentUnit() const;
    QObject * readCurrentUnit() const;

    QPoint getWindowPos() const;
    void setWindowPos(const QPoint &windowPos);
    void centerWindow(const QPoint &pos);
    void moveWindowBy(const QPoint &diff);

    QSize getWindowSize() const;

    void paint(QPainter *painter);

signals:
    void gameChanged();
    void focusedMapNodeChanged();
    void focusedSettlementChanged();
    void focusedUnitChanged();
    void currentMapNodeChanged();
    void currentSettlementChanged();
    void currentUnitChanged();
    void windowPosChanged();
    void windowSizeChanged();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void hoverMoveEvent(QHoverEvent *event);

private:
    void setupMap();
    void updateGeometry();
    void updateWindowPosRect();
    bool rectContainsNode(const QRect &rect, const core::MapNode *node);
    QPoint mapToMap(const QPoint &p);

    void onWidthChanged();
    void onHeightChanged();
    void onFocusedNodeChanged();
    void onCurrentNodeChanged();

    void drawNode(QPainter *painter, const core::MapNode *node);
    void drawGrid(QPainter *painter, const core::MapNode *node);
    void drawFocusMark(QPainter *painter, const core::MapNode *node);
    void drawContent(QPainter *painter, const core::MapNode *node);
    void drawMovingUnit(QPainter *painter, const MovingUnit *movingUnit);
    void drawOverlay(QPainter *painter, core::MapNode *node);

    void updateFocus(const QPoint &p);
    void moveUnit(const QPoint &p);
    void advanceUnits();
    qreal stepUnitTorwards(MovingUnit *u, core::MapNode *n);

    core::Game *game;
    core::World *world;
    core::WorldSurface *surface;
    QSize tileSize;

    QList<core::MapNode *> nodes;
    QHash<const core::MapNode *, QPoint> nodesPos;
    QSet<core::MapNode *> reachableNodes;
    QSet<core::MapNode *> pathNodes;

    core::MapNode *focusedNode;
    core::MapNode *currentNode;

    QTimer *unitMoveTimer;
    QList<MovingUnit *> movingUnits;

    QRect boundingRect;
    QPainterPath hexagonPainterPath;
    QRect windowPosRect;
    QPoint windowPos;
    QSize windowSize;
    QPoint lastPos;
};

} // namespace ui
} // namespace warmonger

#endif // UI_GAME_MAP_H
