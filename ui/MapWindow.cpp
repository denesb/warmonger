#include "ui/MapWindow.h"

namespace warmonger {
namespace ui {

MapWindow::MapWindow(const QRect &mapRect, const QSize &windowSize) :
    mapRect(mapRect),
    windowRect(QPoint(0, 0), windowSize)
{
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
    QPoint newPos(p);

    const int minX = this->mapRect.x();
    const int minY = this->mapRect.y();
    const int maxX = this->mapRect.bottomRight().x() - this->windowRect.width();
    const int maxY = this->mapRect.bottomRight().y() - this->windowRect.height();

    if (newPos.x() < minX)
        newPos.setX(minX);

    if (newPos.y() < minY)
        newPos.setY(minY);

    if (newPos.x() > maxX)
        newPos.setX(maxX);

    if (newPos.y() > maxY)
        newPos.setY(maxY);

    return newPos;
}

} // namespace ui
} // namespace warmonger
