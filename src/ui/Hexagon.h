#ifndef UI_HEXAGON_H
#define UI_HEXAGON_H

#include <QObject>
#include <QPolygon>

namespace warmonger {
namespace ui {

class Hexagon :
    public QObject
{
    Q_OBJECT

public:
    Hexagon(const QPoint &boundingRectCorner, int boundingRectSize, int side);
    ~Hexagon();

    Q_INVOKABLE bool contains(const QPoint &point);

private:
    QRect boundingRect;
    QPolygon hexagonPolygon;
};

} // namespace ui
} // namespace warmonger

#endif // WAMNOGER_H
