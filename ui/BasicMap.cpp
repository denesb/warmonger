#include "ui/BasicMap.h"

#include "utils/Logging.h"

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

    this->setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
}

QRect BasicMap::getMapRect() const
{
    return this->mapWindow.getMapRect();
}

QRect BasicMap::getWindowRect() const
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

void BasicMap::mousePressEvent(QMouseEvent* event)
{
    QPointF pos(this->mapWindow.windowPosToMapPos(event->pos()));
    this->centerWindow(pos.toPoint());
}

void BasicMap::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        QPointF pos(this->mapWindow.windowPosToMapPos(event->pos()));
        this->centerWindow(pos.toPoint());
    }
}

void BasicMap::updateWindow()
{
    this->mapWindow.setWindowSize(QSize(this->width(), this->height()));
}

} // namespace ui
} // namespace warmonger
