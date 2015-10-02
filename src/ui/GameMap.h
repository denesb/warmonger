#ifndef UI_GAME_MAP_H
#define UI_GAME_MAP_H

#include <QHash>
#include <QRect>
#include <QtQuick/QQuickPaintedItem>

namespace warmonger {

namespace core {
    class Game;
    class MapNode;
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

private:
    void setupMap();
    void updateGeometry();

    core::Game *game;
    QHash<const core::MapNode *, QPoint> nodePos;
    QRect boundingRect;
};

} // namespace ui
} // namespace warmonger

#endif // UI_GAME_MAP_H
