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
    Q_PROPERTY(QPoint windowPos READ getWindowPos WRITE setWindowPos NOTIFY windowRectChanged)

public:
    BasicMap(QQuickItem* parent = nullptr);

    QRect getWindowRect() const;
    QPoint getWindowPos() const;
    QRect getMapRect() const;

public slots:
    void setWindowPos(const QPoint& pos);
    void centerWindow(const QPoint& pos);
    void moveWindowBy(const QPoint& diff);

protected:
    void setMapRect(const QRect& mapRect);

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

signals:
    void mapRectChanged();
    void windowRectChanged();

private:
    void updateWindow();

    QPoint lastPos;
    MapWindow mapWindow;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_BASIC_MAP_H
