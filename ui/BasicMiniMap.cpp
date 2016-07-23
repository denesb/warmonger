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
}

const QRect BasicMiniMap::getWindowRect() const
{
    return this->windowRect;
}

void BasicMiniMap::setWindowRect(const QRect& rect)
{
    bool sizeChanged{false}, posChanged{false};

    const QSize newSize = QSize(
            std::min(static_cast<int>(this->width()), rect.width()),
            std::min(static_cast<int>(this->height()), rect.height()));
    if (newSize != this->windowRect.size())
    {
        this->windowRect.setSize(newSize);
        sizeChanged = true;
    }

    const QPoint newPos = this->adjustWindowPosition(this->windowRect.topLeft());
    if  (newPos != this->windowRect.topLeft())
    {
        this->windowRect.moveTopLeft(newPos);
        posChanged = true;
    }

    if (sizeChanged || posChanged)
    {
        emit windowRectChanged();
    }
}

void BasicMiniMap::centerWindow(const QPoint& pos)
{
    this->setWindowPos(pos - QPoint(this->width(), this->height()) / 2);
}

void BasicMiniMap::moveWindowBy(const QPoint& diff)
{
    this->setWindowPos(this->windowRect.topLeft() + diff);
}

void BasicMiniMap::setMapRect(const QRect& mapRect)
{
    if (this->mapRect != mapRect)
    {
        this->mapRect = mapRect;
        this->setWindowPos(this->windowRect.topLeft());
    }
}

QPoint BasicMiniMap::mapToMap(const QPoint& p)
{
    return p;
}

void BasicMiniMap::updateWindow()
{
    this->setWindowPos(this->windowRect.topLeft());
}

void BasicMiniMap::setWindowPos(const QPoint& pos)
{
    const QPoint newPos = this->adjustWindowPosition(pos);

    if (this->windowRect.topLeft() != newPos)
    {
        this->windowRect.moveTopLeft(newPos);
        emit windowRectChanged();
    }
}

QPoint BasicMiniMap::adjustWindowPosition(const QPoint& p)
{
    const int x = this->adjustAxis(p.x(), this->windowRect.width(), static_cast<int>(this->width()));
    const int y = this->adjustAxis(p.y(), this->windowRect.height(), static_cast<int>(this->height()));

    return QPoint(x, y);
}

int BasicMiniMap::adjustAxis(const int n, const int windowLength, const int frameLength)
{
    if (windowLength >= frameLength)
    {
        return 0;
    }
    else
    {
        const int maxN = frameLength - windowLength - 1;

        if (n < 0)
            return 0;
        else if (n > maxN)
            return maxN;
        else
            return n;
    }
}

} // namespace ui
} // namespace warmonger
