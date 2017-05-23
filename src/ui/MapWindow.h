/** \file
 * MapWindow class.
 *
 * \copyright (C) 2015-2017 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef W_UI_MAP_WINDOW_H
#define W_UI_MAP_WINDOW_H

#include <QObject>
#include <QPoint>
#include <QRect>
#include <QSize>

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
    MapWindow();
    MapWindow(const QRect& mapRect, const QSize& windowSize);

    const QRect& getMapRect() const;
    void setMapRect(const QRect& mapRect);

    const QRect& getWindowRect() const;
    void setWindowRect(const QRect& windowRect);
    void setWindowPos(const QPoint& windowPos);
    void setWindowSize(const QSize& windowSize);

    void centerWindow(const QPoint& pos);
    void moveWindowBy(const QPoint& diff);

    QPoint windowPosToMapPos(const QPoint& pos);

signals:
    void mapRectChanged();
    void windowRectChanged();

private:
    QPoint adjustWindowPosition(const QPoint& p);

    QRect mapRect;
    QRect windowRect;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_MAP_WINDOW_H
