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
    this->windowPos = windowPos;
}

QRect MapWindow::getWindowRect() const
{
    return QRect(this->windowPos, this->windowSize);
}

void MapWindow::centerWindow(const QPoint &pos)
{
    QPoint ws(this->windowSize.width(), this->windowSize.height());
    this->setWindowPos(pos - ws / 2);
}

void MapWindow::moveWindowBy(const QPoint &diff)
{
    const QPoint newPos(this->windowPos + diff);
    this->setWindowPos(newPos);
}

QPoint MapWindow::mapToMap(const QPoint &p)
{
    return p + this->windowPos;
}

}
}
