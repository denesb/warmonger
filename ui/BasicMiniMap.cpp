/**
 * Copyright (C) 2015-2016 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "ui/BasicMiniMap.h"
#include "ui/MapUtil.h"
#include "utils/Logging.h"

namespace warmonger {
namespace ui {

BasicMiniMap::BasicMiniMap(QQuickItem* parent)
    : QQuickItem(parent)
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
