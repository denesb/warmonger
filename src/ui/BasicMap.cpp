/**
 * Copyright (C) 2015-2018 Botond Dénes
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

#include "ui/BasicMap.h"

#include "utils/Logging.h"

namespace warmonger {
namespace ui {

BasicMap::BasicMap(QQuickItem* parent)
    : QQuickItem(parent)
{
    QObject::connect(this, &BasicMap::widthChanged, this, &BasicMap::updateWindow);
    QObject::connect(this, &BasicMap::heightChanged, this, &BasicMap::updateWindow);
    QObject::connect(this, &BasicMap::mapRectChanged, this, &BasicMap::update);
    QObject::connect(this, &BasicMap::windowRectChanged, this, &BasicMap::update);
    QObject::connect(&this->mapWindow, &MapWindow::mapRectChanged, this, &BasicMap::mapRectChanged);
    QObject::connect(&this->mapWindow, &MapWindow::windowRectChanged, this, &BasicMap::windowRectChanged);

    this->setAcceptedMouseButtons(Qt::LeftButton);
}

QRect BasicMap::getMapRect() const
{
    return this->mapWindow.getMapRect();
}

QRect BasicMap::getWindowRect() const
{
    return this->mapWindow.getWindowRect();
}

QPoint BasicMap::getWindowPos() const
{
    return this->mapWindow.getWindowRect().topLeft();
}

QPoint BasicMap::windowPosToMapPos(const QPoint& pos)
{
    return this->mapWindow.windowPosToMapPos(pos);
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
    this->lastPos = event->pos();
}

void BasicMap::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        this->moveWindowBy(this->lastPos - event->pos());

        this->lastPos = event->pos();
    }
}

void BasicMap::updateWindow()
{
    this->mapWindow.setWindowSize(QSize(this->width(), this->height()));
}

} // namespace ui
} // namespace warmonger
