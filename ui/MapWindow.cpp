#include "ui/MapWindow.h"

namespace warmonger {
namespace ui {

MapWindow::MapWindow(const QSize &mapSize, const QSize &windowSize) :
    mapSize(mapSize),
    windowSize(windowSize),
    windowPos(0, 0)
{
}

QSize MapWindow::getMapSize() const
{
    return this->mapSize;
}

void MapWindow::setMapSize(const QSize &mapSize)
{
    this->mapSize = mapSize;
}

void MapWindow::setMapWidth(int width)
{
    this->mapSize.setWidth(width);
}

void MapWindow::setMapHeight(int height)
{
    this->mapSize.setHeight(height);
}

QSize MapWindow::getWindowSize() const
{
    return this->windowSize;
}

void MapWindow::setWindowSize(const QSize &windowSize)
{
    this->windowSize = windowSize;
}

void MapWindow::setWindowWidth(int width)
{
    this->windowSize.setWidth(width);
}

void MapWindow::setWindowHeight(int height)
{
    this->windowSize.setHeight(height);
}

QPoint MapWindow::getWindowPos() const
{
    return this->windowPos;
}

void MapWindow::setWindowPos(const QPoint &windowPos)
{
    this->windowPos = this->adjustWindowPosition(windowPos);
}

QRect MapWindow::getWindowRect() const
{
    return QRect(this->windowPos, this->windowSize);
}

void MapWindow::centerWindow(const QPoint &pos)
{
    QPoint ws(this->windowSize.width(), this->windowSize.height());

    this->setWindowPos(this->adjustWindowPosition(pos - ws / 2));
}

void MapWindow::moveWindowBy(const QPoint &diff)
{
    const QPoint newPos(this->windowPos + diff);
    this->setWindowPos(this->adjustWindowPosition(newPos));
}

QPoint MapWindow::mapToMap(const QPoint &p)
{
    return p + this->windowPos;
}

QPoint MapWindow::adjustWindowPosition(const QPoint &p)
{
    QPoint newPos(p);

    if (newPos.x() < 0)
        newPos.setX(0);

    if (newPos.y() < 0)
        newPos.setY(0);

    const int maxX = this->mapSize.width() - this->windowSize.width() - 1;
    const int maxY = this->mapSize.height() - this->windowSize.height() - 1;

    if (newPos.x() > maxX)
        newPos.setX(maxY);

    if (newPos.y() > maxY )
        newPos.setY(maxY);

    return newPos;
}

} // namespace ui
} // namespace warmonger
