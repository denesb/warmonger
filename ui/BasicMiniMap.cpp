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
    QObject::connect(&this->mapWindow, &MapWindow::mapRectChanged, this, &BasicMiniMap::updateTransform);
    QObject::connect(&this->mapWindow, &MapWindow::mapRectChanged, this, &BasicMiniMap::mapRectChanged);

    this->setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
}

QRect BasicMiniMap::getWindowRect() const
{
    return this->mapWindow.getWindowRect();
}

QPoint BasicMiniMap::getWindowPos() const
{
    return this->mapWindow.getWindowRect().topLeft();
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

void BasicMiniMap::setWindowPos(const QPoint& pos)
{
    this->mapWindow.setWindowPos(pos);
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

void BasicMiniMap::mousePressEvent(QMouseEvent* event)
{
    this->centerWindow(this->centeredPosToPos(event->pos()));
}

void BasicMiniMap::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        this->centerWindow(this->centeredPosToPos(event->pos()));
    }
}

void BasicMiniMap::updateTransform()
{
    const QRectF frame(0.0, 0.0, this->width(), this->height());

    this->transform = centerIn(this->mapWindow.getMapRect(), frame);

    this->update();
}

QPoint BasicMiniMap::centeredPosToPos(const QPoint& pos) const
{
    const QVector4D firstRow = this->transform.row(0);
    const QVector4D secondRow = this->transform.row(1);

    const qreal scale = firstRow.x();
    const QPoint translate(firstRow.w(), secondRow.w());

    const qreal rscale = 1 / scale;

    return (pos - translate) * rscale;
}

} // namespace ui
} // namespace warmonger
