/**
 * Copyright (C) 2015-2018 Botond Dénes
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

#include "ui/MapWindow.h"
#include "utils/Logging.h"

namespace warmonger {
namespace ui {

static int adjustAxis(const int n, const int minN, const int mapLength, const int windowLength);

MapWindow::MapWindow()
{
}

MapWindow::MapWindow(const QRect& mapRect, const QSize& windowSize)
    : mapRect(mapRect)
    , windowRect(this->mapRect.topLeft(), windowSize)
{
    this->setWindowPos(this->windowRect.topLeft());
}

const QRect& MapWindow::getMapRect() const
{
    return this->mapRect;
}

void MapWindow::setMapRect(const QRect& mapRect)
{
    if (this->mapRect != mapRect)
    {
        this->mapRect = mapRect;

        emit mapRectChanged();

        this->setWindowPos(this->windowRect.topLeft());
    }
}

void MapWindow::setWindowRect(const QRect& windowRect)
{
    bool sizeChanged{false}, posChanged{false};

    if (this->windowRect.size() != windowRect.size())
    {
        this->windowRect.setSize(windowRect.size());
        sizeChanged = true;
    }

    const QPoint newPos = this->adjustWindowPosition(windowRect.topLeft());
    if (this->windowRect.topLeft() != newPos)
    {
        this->windowRect.moveTopLeft(newPos);
        posChanged = true;
    }

    if (sizeChanged || posChanged)
    {
        emit windowRectChanged();
    }
}

void MapWindow::setWindowPos(const QPoint& pos)
{
    const QPoint newPos = this->adjustWindowPosition(pos);
    if (newPos != this->windowRect.topLeft())
    {
        this->windowRect.moveTopLeft(newPos);
        emit windowRectChanged();
    }
}

void MapWindow::setWindowSize(const QSize& size)
{
    this->setWindowRect(QRect(this->windowRect.topLeft(), size));
}

const QRect& MapWindow::getWindowRect() const
{
    return this->windowRect;
}

void MapWindow::centerWindow(const QPoint& pos)
{
    this->setWindowPos(pos - QPoint(this->windowRect.width(), this->windowRect.height()) / 2);
}

void MapWindow::moveWindowBy(const QPoint& diff)
{
    this->setWindowPos(this->windowRect.topLeft() + diff);
}

QPoint MapWindow::windowPosToMapPos(const QPoint& pos)
{
    return pos + this->windowRect.topLeft();
}

QPoint MapWindow::adjustWindowPosition(const QPoint& p)
{
    const int x = adjustAxis(p.x(), this->mapRect.x(), this->mapRect.width(), this->windowRect.width());
    const int y = adjustAxis(p.y(), this->mapRect.y(), this->mapRect.height(), this->windowRect.height());

    return QPoint(x, y);
}

static int adjustAxis(const int n, const int minN, const int mapLength, const int windowLength)
{
    if (mapLength == windowLength)
    {
        return minN;
    }
    else if (mapLength < windowLength)
    {
        const int diff = windowLength - mapLength;
        return minN - diff / 2;
    }
    else
    {
        const int maxN = minN + mapLength - windowLength - 1;

        if (n < minN)
            return minN;
        else if (n > maxN)
            return maxN;
        else
            return n;
    }
}

} // namespace ui
} // namespace warmonger
