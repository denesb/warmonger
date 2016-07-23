#ifndef W_UI_BASIC_MINI_MAP_H
#define W_UI_BASIC_MINI_MAP_H

#include <QtQuick/QQuickItem>
#include <QMatrix4x4>
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
    Q_PROPERTY(QRect windowRect READ getWindowRect WRITE setWindowRect NOTIFY windowRectChanged)

public:
    BasicMiniMap(QQuickItem* parent = nullptr);

    QRect getWindowRect() const;
    QRect getMapRect() const;

    const QMatrix4x4& getTransformMatrix() const;

public slots:
    void setWindowRect(const QRect& rect);
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
    void updateTransform();

    QMatrix4x4 transform;
    MapWindow mapWindow;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_BASIC_MINI_MAP_H
