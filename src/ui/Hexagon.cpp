#include <QSize>

#include "ui/Hexagon.h"

using namespace warmonger::ui;

Hexagon::Hexagon(const QPoint &boundingRectCorner, int boundingRectSize, int side) :
    boundingRect(boundingRectCorner, QSize(boundingRectSize, boundingRectSize)),
    hexagonPolygon(7)
{
    int hd = (boundingRectSize - side) / 2;
    int vd = boundingRectSize / 2;

    QPoint nw(boundingRectCorner + QPoint(hd, 0));
    QPoint ne(boundingRectCorner + QPoint(hd + side, 0));
    QPoint e(boundingRectCorner + QPoint(boundingRectSize, vd));
    QPoint se(boundingRectCorner + QPoint(hd + side, boundingRectSize));
    QPoint sw(boundingRectCorner + QPoint(hd, boundingRectSize));
    QPoint w(boundingRectCorner + QPoint(0, vd));

    this->hexagonPolygon << nw << ne << e << se << sw << w << nw;
}

Hexagon::~Hexagon()
{
}

bool Hexagon::contains(const QPoint &point)
{
    return this->hexagonPolygon.containsPoint(point, Qt::OddEvenFill);
}

