#include "ui/BasicMap.h"

namespace warmonger {
namespace ui {

BasicMap::BasicMap(QQuickItem* parent) :
    QQuickItem(parent)
{
    QObject::connect(this, &BasicMap::widthChanged, this, &BasicMap::updateWindow);
    QObject::connect(this, &BasicMap::heightChanged, this, &BasicMap::updateWindow);
}

const QRect BasicMap::getMapRect() const
{
    return this->mapRect;
}

const QRect BasicMap::getWindowRect() const
{
    return this->windowRect;
}

void BasicMap::setWindowPos(const QPoint &pos)
{
    const QPoint newPos = this->adjustWindowPosition(pos);

    if (this->windowRect.topLeft() != newPos)
    {
        this->windowRect.moveTopLeft(newPos);
        emit windowRectChanged();
    }
}

void BasicMap::centerWindow(const QPoint &pos)
{
    const QPoint ws(this->windowRect.width(), this->windowRect.height());

    this->windowRect.moveTopLeft(pos - ws / 2);
}

void BasicMap::moveWindowBy(const QPoint &diff)
{
    const QPoint newPos(this->windowRect.topLeft() + diff);

    this->windowRect.moveTopLeft(newPos);
}

void BasicMap::setMapRect(const QRect &mapRect)
{
    if (this->mapRect != mapRect)
    {
        this->mapRect = mapRect;
        this->windowRect = QRect(this->adjustWindowPosition(this->windowRect.topLeft()), this->windowRect.size());
    }
}

void BasicMap::setWindowSize(const QSize &size)
{
    if (this->windowRect.size() != size)
    {
        this->windowRect.setSize(size);
        emit windowRectChanged();
    }
}

QPoint BasicMap::mapToMap(const QPoint &p)
{
    return p + this->windowRect.topLeft();
}

void BasicMap::updateWindow()
{
    this->windowRect.setWidth(this->width());
    this->windowRect.setHeight(this->height());

    this->windowRect.moveTopLeft(this->adjustWindowPosition(this->windowRect.topLeft()));
    this->update();
}

QPoint BasicMap::adjustWindowPosition(const QPoint &p)
{
    const int x = this->adjustAxis(p.x(), this->mapRect.x(), this->mapRect.width(), this->windowRect.width());
    const int y = this->adjustAxis(p.y(), this->mapRect.y(), this->mapRect.height(), this->windowRect.height());

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
