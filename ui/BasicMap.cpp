#include "ui/BasicMap.h"

namespace warmonger {
namespace ui {

BasicMap::BasicMap(QQuickItem* parent) :
    QQuickItem(parent)
{
    QObject::connect(this, &BasicMap::widthChanged, this, &BasicMap::updateWindow);
    QObject::connect(this, &BasicMap::heightChanged, this, &BasicMap::updateWindow);
    QObject::connect(this, &BasicMap::mapRectChanged, this, &BasicMap::update);
    QObject::connect(this, &BasicMap::windowRectChanged, this, &BasicMap::update);
    QObject::connect(&this->mapWindow, &MapWindow::mapRectChanged, this, &BasicMap::mapRectChanged);
    QObject::connect(&this->mapWindow, &MapWindow::windowRectChanged, this, &BasicMap::windowRectChanged);
}

const QRect BasicMap::getMapRect() const
{
    return this->mapWindow.getMapRect();
}

const QRect BasicMap::getWindowRect() const
{
    return this->mapWindow.getWindowRect();
}

void BasicMap::setWindowPos(const QPoint& pos)
{
    this->mapWindow.setWindowPos(pos);
}

void BasicMap::centerWindow(const QPoint& pos)
{
    this->mapWindow.centerWindow(pos);
}

void BasicMap::moveWindowBy(const QPoint& diff)
{
    this->mapWindow.moveWindowBy(diff);
}

void BasicMap::setMapRect(const QRect& mapRect)
{
    this->mapWindow.setMapRect(mapRect);
}

void BasicMap::updateWindow()
{
    this->mapWindow.setWindowSize(QSize(this->width(), this->height()));
}

} // namespace ui
} // namespace warmonger
