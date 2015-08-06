#ifndef UI_USER_INTERFACE_H
#define UI_USER_INTERFACE_H

#include "ui/qtquick2applicationviewer.h"
#include "core/World.h"
#include "core/Map.h"
#include "core/Loader.hpp"

namespace warmonger {
namespace ui {

class UserInterface :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant map READ readMap NOTIFY mapChanged)

public:
    UserInterface(QObject *parent);
    ~UserInterface();

    QVariant readMap() const;

    Q_INVOKABLE bool hexContains(const QPoint &p) const;
    Q_INVOKABLE QVariant mapEditor(QObject *map);

signals:
    void mapChanged();

private:
    void setupModels();

    QtQuick2ApplicationViewer viewer;
    core::Loader<core::Map> mapLoader;
    core::Map *map;
    QImage hexMask;
};

} // namespace ui
} // namespace warmonger

#endif // WAMNOGER_H
