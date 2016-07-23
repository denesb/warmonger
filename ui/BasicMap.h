#ifndef W_UI_BASIC_MAP_H
#define W_UI_BASIC_MAP_H

#include <QtQuick/QQuickItem>
#include <QPoint>
#include <QRect>
#include <QSize>

#include "ui/MapWindow.h"

namespace warmonger {
namespace ui {

class BasicMap
    : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QRect windowRect READ getWindowRect NOTIFY windowRectChanged)

public:
    BasicMap(QQuickItem* parent = nullptr);

    const QRect getWindowRect() const;
    const QRect getMapRect() const;

public slots:
    void setWindowPos(const QPoint& pos);
    void centerWindow(const QPoint& pos);
    void moveWindowBy(const QPoint& diff);

protected:
    void setMapRect(const QRect& mapRect);

signals:
    void mapRectChanged();
    void windowRectChanged();

private:
    void updateWindow();

    MapWindow mapWindow;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_BASIC_MAP_H
