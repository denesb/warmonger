#ifndef W_UI_BASIC_MINI_MAP_H
#define W_UI_BASIC_MINI_MAP_H

#include <QMatrix4x4>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QtQuick/QQuickItem>

#include "ui/MapWindow.h"

namespace warmonger {
namespace ui {

class BasicMiniMap : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QRect windowRect READ getWindowRect WRITE setWindowRect NOTIFY windowRectChanged)
    Q_PROPERTY(QPoint windowPos READ getWindowPos WRITE setWindowPos NOTIFY windowRectChanged)

public:
    BasicMiniMap(QQuickItem* parent = nullptr);

    QRect getWindowRect() const;
    QPoint getWindowPos() const;
    QRect getMapRect() const;

    const QMatrix4x4& getTransformMatrix() const;

public slots:
    void setWindowRect(const QRect& rect);
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
    void updateTransform();
    QPoint centeredPosToPos(const QPoint& pos) const;

    QMatrix4x4 transform;
    MapWindow mapWindow;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_BASIC_MINI_MAP_H
