#ifndef UI_MAP_WINDOW_H
#define UI_MAP_WINDOW_H

#include <QPoint>
#include <QSize>
#include <QRect>

namespace warmonger {
namespace ui {

/*
 * Windowed view of a map.
 *
 * Most maps are to big to fit into the view, so only a part of them is
 * rendered. MapWindow keeps track of the window's size, position and
 * provides related functionality like coordinate system mapping
 * from view to map.
 */
class MapWindow
{
public:
    MapWindow(const QSize &mapSize, const QSize &windowSize);

    QSize getMapSize() const;
    void setMapSize(const QSize &mapSize);
    void setMapWidth(int width);
    void setMapHeight(int height);

    QSize getWindowSize() const;
    void setWindowSize(const QSize &windowSize);
    void setWindowWidth(int width);
    void setWindowHeight(int height);

    QPoint getWindowPos() const;
    void setWindowPos(const QPoint &windowPos);

    QRect getWindowRect() const;

    void centerWindow(const QPoint &pos);
    void moveWindowBy(const QPoint &diff);

    QPoint mapToMap(const QPoint &p);

private:
    QSize mapSize;
    QSize windowSize;
    QPoint windowPos;
};

} // namespace ui
} // namespace warmonger

#endif // UI_MAP_WINDOW_H
