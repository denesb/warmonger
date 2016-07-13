#include "ui/MapWindow.h"

namespace warmonger {
namespace ui {

MapWindow::MapWindow(const QRect &mapRect, const QSize &windowSize) :
    mapRect(mapRect),
    windowRect(this->mapRect.topLeft(), windowSize)
{
    this->windowRect.moveTopLeft(this->adjustWindowPosition(this->windowRect.topLeft()));
}

QSize MapWindow::getMapSize() const
{
    return this->mapRect.size();
}

QRect MapWindow::getMapRect() const
{
    return this->mapRect;
}

void MapWindow::setMapRect(const QRect &mapRect)
{
    this->mapRect = mapRect;
}

QSize MapWindow::getWindowSize() const
{
    return this->windowRect.size();
}

void MapWindow::setWindowSize(const QSize &windowSize)
{
    if (this->windowRect.size() != windowSize)
    {
        this->windowRect.setSize(windowSize);
        emit windowSizeChanged();
    }
}

QPoint MapWindow::getWindowPos() const
{
    return this->windowRect.topLeft();
}

void MapWindow::setWindowPos(const QPoint &windowPos)
{
    const QPoint newPos = this->adjustWindowPosition(windowPos);
    if (this->windowRect.topLeft() != newPos)
    {
        this->windowRect.moveTopLeft(newPos);
        emit windowPosChanged();
    }
}

QRect MapWindow::getWindowRect() const
{
    return this->windowRect;
}

void MapWindow::centerWindow(const QPoint &pos)
{
    QPoint ws(this->windowRect.width(), this->windowRect.height());

    this->setWindowPos(this->adjustWindowPosition(pos - ws / 2));
}

void MapWindow::moveWindowBy(const QPoint &diff)
{
    const QPoint newPos(this->getWindowPos() + diff);
    this->setWindowPos(this->adjustWindowPosition(newPos));
}

QPoint MapWindow::mapToMap(const QPoint &p)
{
    return p + this->getWindowPos();
}

QPoint MapWindow::adjustWindowPosition(const QPoint &p)
{
    const int x = this->adjustAxis(p.x(), this->mapRect.x(), this->mapRect.width(), this->windowRect.width());
    const int y = this->adjustAxis(p.y(), this->mapRect.y(), this->mapRect.height(), this->windowRect.height());

    return QPoint(x, y);
}

int MapWindow::adjustAxis(const int n, const int minN, const int mapLength, const int windowLength)
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
