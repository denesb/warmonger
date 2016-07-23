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
}

const QRect BasicMap::getMapRect() const
{
    return this->mapRect;
}

const QRect BasicMap::getWindowRect() const
{
    return QRect(this->windowPos, QSize(this->width(), this->height()));
}

void BasicMap::setWindowPos(const QPoint& pos)
{
    const QPoint newPos = this->adjustWindowPosition(pos);

    if (this->windowPos != newPos)
    {
        this->windowPos = newPos;
        emit windowRectChanged();
    }
}

void BasicMap::centerWindow(const QPoint& pos)
{
    this->setWindowPos(pos - QPoint(this->width(), this->height()) / 2);
}

void BasicMap::moveWindowBy(const QPoint& diff)
{
    this->setWindowPos(this->windowPos + diff);
}

void BasicMap::setMapRect(const QRect& mapRect)
{
    if (this->mapRect != mapRect)
    {
        this->mapRect = mapRect;
        this->setWindowPos(this->windowPos);
    }
}

QPoint BasicMap::mapToMap(const QPoint& p)
{
    return p + this->windowPos;
}

void BasicMap::updateWindow()
{
    this->setWindowPos(this->windowPos);
}

QPoint BasicMap::adjustWindowPosition(const QPoint& p)
{
    const int x = this->adjustAxis(p.x(), this->mapRect.x(), this->mapRect.width(), this->width());
    const int y = this->adjustAxis(p.y(), this->mapRect.y(), this->mapRect.height(), this->height());

    return QPoint(x, y);
}

int BasicMap::adjustAxis(const int n, const int minN, const int mapLength, const int windowLength)
{
    if (mapLength == windowLength)
    {
        return minN;
    }
    else if (mapLength < windowLength)
    {
        const int diff = windowLength - mapLength;
        return minN - diff / 2;
    }
    else
    {
        const int maxN = minN + mapLength - windowLength - 1;

        if (n < minN)
            return minN;
        else if (n > maxN)
            return maxN;
        else
            return n;
    }
}

} // namespace ui
} // namespace warmonger
