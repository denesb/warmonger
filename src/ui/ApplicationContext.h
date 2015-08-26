#ifndef UI_APPLICATION_CONTEXT_H
#define UI_APPLICATION_CONTEXT_H

#include <QImage>

#include "core/Map.h"

namespace warmonger {
namespace ui {

class ApplicationContext :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant map READ readMap NOTIFY mapChanged)

public:
    ApplicationContext(QObject *parent);
    ~ApplicationContext();

    QVariant readMap() const;

    Q_INVOKABLE void loadMap(const QString &mapName);
    Q_INVOKABLE bool hexContains(const QPoint &p) const;

signals:
    void mapChanged() const;

private:
    core::Map *map;
    QImage hexMask;
};

} // namespace ui
} // namespace warmonger

#endif // UI_APPLICATION_CONTEXT_H
