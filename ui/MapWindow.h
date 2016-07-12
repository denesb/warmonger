#ifndef UI_MAP_WINDOW_H
#define UI_MAP_WINDOW_H

#include <QObject>
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
class MapWindow : public QObject
{
    Q_OBJECT

public:
    MapWindow(const QRect &mapRect, const QSize &windowSize);

    QSize getMapSize() const;
    QRect getMapRect() const;
    void setMapRect(const QRect &mapRect);

    QSize getWindowSize() const;
    void setWindowSize(const QSize &windowSize);

    QPoint getWindowPos() const;
    void setWindowPos(const QPoint &windowPos);

    QRect getWindowRect() const;

    void centerWindow(const QPoint &pos);
    void moveWindowBy(const QPoint &diff);

    QPoint mapToMap(const QPoint &p);

signals:
    void windowPosChanged();
    void windowSizeChanged();

private:
    QPoint adjustWindowPosition(const QPoint &p);

    QRect mapRect;
    QRect windowRect;
};

} // namespace ui
} // namespace warmonger

#endif // UI_MAP_WINDOW_H
