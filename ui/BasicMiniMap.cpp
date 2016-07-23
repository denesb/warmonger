#include "ui/BasicMiniMap.h"
#include "ui/MapUtil.h"
#include "utils/Logging.h"

namespace warmonger {
namespace ui {

BasicMiniMap::BasicMiniMap(QQuickItem* parent) :
    QQuickItem(parent)
{
    QObject::connect(this, &BasicMiniMap::widthChanged, this, &BasicMiniMap::updateTransform);
    QObject::connect(this, &BasicMiniMap::heightChanged, this, &BasicMiniMap::updateTransform);
    QObject::connect(&this->mapWindow, &MapWindow::windowRectChanged, this, &BasicMiniMap::update);
    QObject::connect(&this->mapWindow, &MapWindow::windowRectChanged, this, &BasicMiniMap::windowRectChanged);
    QObject::connect(&this->mapWindow, &MapWindow::mapRectChanged, this, &BasicMiniMap::update);
    QObject::connect(&this->mapWindow, &MapWindow::mapRectChanged, this, &BasicMiniMap::mapRectChanged);
}

QRect BasicMiniMap::getWindowRect() const
{
    return this->mapWindow.getWindowRect();
}

QRect BasicMiniMap::getMapRect() const
{
    return this->mapWindow.getWindowRect();
}

const QMatrix4x4& BasicMiniMap::getTransformMatrix() const
{
    return this->transform;
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

void BasicMiniMap::mousePressEvent(QMouseEvent*)
{
    //QPointF pos(this->campaignMapToMap(event->pos()));
    //this->centerWindow(pos.toPoint());
}

void BasicMiniMap::mouseMoveEvent(QMouseEvent*)
{
    /*
    if (event->buttons() & Qt::LeftButton)
    {
        const qreal rscale = 1 / this->scale;
        return p * rscale - this->translate;
        QPointF pos(this->windowMapToMap(event->pos()));
        this->centerWindow(pos.toPoint());
    }
    */
}

void BasicMiniMap::updateTransform()
{
    const QRectF frame(0.0, 0.0, this->width(), this->height());

    this->transform = centerIn(this->mapWindow.getMapRect(), frame);

    this->update();
}

} // namespace ui
} // namespace warmonger
