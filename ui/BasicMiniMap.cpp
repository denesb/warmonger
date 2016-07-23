#include "ui/BasicMiniMap.h"

namespace warmonger {
namespace ui {

BasicMiniMap::BasicMiniMap(QQuickItem* parent) :
    QQuickItem(parent)
{
    QObject::connect(this, &BasicMiniMap::widthChanged, this, &BasicMiniMap::updateWindow);
    QObject::connect(this, &BasicMiniMap::heightChanged, this, &BasicMiniMap::updateWindow);
    QObject::connect(this, &BasicMiniMap::mapRectChanged, this, &BasicMiniMap::update);
    QObject::connect(this, &BasicMiniMap::windowRectChanged, this, &BasicMiniMap::update);
    QObject::connect(&this->mapWindow, &MapWindow::mapRectChanged, this, &BasicMiniMap::mapRectChanged);
    QObject::connect(&this->mapWindow, &MapWindow::windowRectChanged, this, &BasicMiniMap::windowRectChanged);
}

const QRect BasicMiniMap::getWindowRect() const
{
    return this->mapWindow.getWindowRect();
}

void BasicMiniMap::setWindowRect(const QRect& rect)
{
    this->mapWindow.setWindowRect(rect);
}

void BasicMiniMap::centerWindow(const QPoint& pos)
{
    this->mapWindow.centerWindow(pos);
}

void BasicMiniMap::moveWindowBy(const QPoint& diff)
{
    this->mapWindow.moveWindowBy(diff);
}

void BasicMiniMap::setMapRect(const QRect& mapRect)
{
    this->mapWindow.setMapRect(mapRect);
}

void BasicMiniMap::updateWindow()
{
    this->mapWindow.setWindowSize(QSize(this->width(), this->height()));
}

} // namespace ui
} // namespace warmonger
