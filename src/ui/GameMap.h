#ifndef UI_GAME_MAP_H
#define UI_GAME_MAP_H

#include <QHash>
#include <QPainterPath>
#include <QRect>
#include <QtQuick/QQuickPaintedItem>

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
public:
    GameMap(QQuickItem *parent = nullptr);
    ~GameMap();

    core::Game *getGame() const;
    void setGame(core::Game *game);
    QObject *readGame() const;
    void writeGame(QObject *game);

    void paint(QPainter *painter);

signals:
    void gameChanged();

protected:
    void mousePressEvent(QMouseEvent *event);

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
    QHash<const core::MapNode *, QPoint> nodePos;
    QRect boundingRect;
    QPainterPath hexagonPainterPath;
    core::MapNode *focusedNode;
};

} // namespace ui
} // namespace warmonger

#endif // UI_GAME_MAP_H
