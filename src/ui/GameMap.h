#ifndef UI_GAME_MAP_H
#define UI_GAME_MAP_H

#include <QHash>
#include <QPainterPath>
#include <QRect>
#include <QtQuick/QQuickPaintedItem>

#include "ui/MapUtil.h"

namespace warmonger {

namespace core {
    class Game;
    class MapNode;
    class World;
    class WorldSurface;
}

namespace ui {

class GameMap :
    public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QObject *game READ readGame WRITE writeGame NOTIFY gameChanged)
    Q_PROPERTY(QObject *currentMapNode READ readCurrentMapNode NOTIFY currentMapNodeChanged)
    Q_PROPERTY(QPoint currentPos READ getCurrentPos NOTIFY currentPosChanged)
    Q_PROPERTY(QObject *currentSettlement READ readCurrentSettlement NOTIFY currentSettlementChanged)
    Q_PROPERTY(QObject *currentUnit READ readCurrentUnit NOTIFY currentUnitChanged)
public:
    GameMap(QQuickItem *parent = nullptr);
    ~GameMap();

    core::Game *getGame() const;
    void setGame(core::Game *game);
    QObject *readGame() const;
    void writeGame(QObject *game);

    core::MapNode * getCurrentMapNode() const;
    QObject * readCurrentMapNode() const;

    core::Settlement * getCurrentSettlement() const;
    QObject * readCurrentSettlement() const;

    core::Unit * getCurrentUnit() const;
    QObject * readCurrentUnit() const;

    QPoint getCurrentPos() const;

    void paint(QPainter *painter);

signals:
    void gameChanged();
    void currentMapNodeInfoChanged();
    void currentMapNodeChanged();
    void currentPosChanged();
    void currentSettlementChanged();
    void currentUnitChanged();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void hoverMoveEvent(QHoverEvent *event);

private:
    void setupMap();
    void updateGeometry();
    bool rectContainsNode(const QRect &rect, const core::MapNode *node);

    void drawNode(QPainter *painter, const core::MapNode *node);
    void drawGrid(QPainter *painter, const core::MapNode *node);
    void drawFocusMark(QPainter *painter, const core::MapNode *node);

    QList<core::MapNode *> nodes;
    core::World *world;
    core::WorldSurface *surface;
    QSize tileSize;

    core::Game *game;
    QHash<const core::MapNode *, NodeInfo *> nodesInfo;
    QRect boundingRect;
    QPainterPath hexagonPainterPath;
    core::MapNode *focusedNode;
    NodeInfo *currentNodeInfo;
};

} // namespace ui
} // namespace warmonger

#endif // UI_GAME_MAP_H
