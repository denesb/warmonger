#ifndef W_UI_BASIC_MINI_MAP_H
#define W_UI_BASIC_MINI_MAP_H

#include <QtQuick/QQuickItem>
#include <QPoint>
#include <QRect>
#include <QSize>

#include "ui/MapWindow.h"

namespace warmonger {
namespace ui {

class BasicMiniMap
    : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QRect windowRect READ getWindowRect NOTIFY windowRectChanged)

public:
    BasicMiniMap(QQuickItem* parent = nullptr);

    const QRect getWindowRect() const;

public slots:
    void setWindowRect(const QRect& rect);
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

#endif // W_UI_BASIC_MINI_MAP_H
