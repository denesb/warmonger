#include "ui/MapWindow.h"

namespace warmonger {
namespace ui {

MapWindow::MapWindow()
{
}

MapWindow::MapWindow(const QRect &mapRect, const QSize &windowSize) :
    mapRect(mapRect),
    windowRect(this->mapRect.topLeft(), windowSize)
{
    this->windowRect.moveTopLeft(this->adjustWindowPosition(this->windowRect.topLeft()));
}

const QRect& MapWindow::getMapRect() const
{
    return this->mapRect;
}

void MapWindow::setMapRect(const QRect &mapRect)
{
    if (this->mapRect != mapRect)
    {
        this->mapRect = mapRect;
        this->windowRect = QRect(this->adjustWindowPosition(this->windowRect.topLeft()), this->windowRect.size());
    }
}

void MapWindow::setWindowRect(const QRect &windowRect)
{
    const QPoint newPos = this->adjustWindowPosition(windowRect.topLeft());
    const QRect newRect(newPos, windowRect.size());

    if (this->windowRect != newRect)
    {
        this->windowRect = newRect;
        emit windowRectChanged();
    }
}

const QRect& MapWindow::getWindowRect() const
{
    return this->windowRect;
}

void MapWindow::centerWindow(const QPoint &pos)
{
    const QPoint ws(this->windowRect.width(), this->windowRect.height());

    this->setWindowRect(QRect(pos - ws / 2, this->windowRect.size()));
}

void MapWindow::moveWindowBy(const QPoint &diff)
{
    const QPoint newPos(this->windowRect.topLeft() + diff);

    this->setWindowRect(QRect(newPos, this->windowRect.size()));
}

QPoint MapWindow::mapToMap(const QPoint &p)
{
    return p + this->windowRect.topLeft();
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
