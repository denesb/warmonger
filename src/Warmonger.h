#ifndef WARMONGER_H
#define WARMONGER_H

#include <QGuiApplication>

#include "ui/qtquick2applicationviewer.h"
#include "core/World.h"
#include "core/Map.h"
#include "core/Loader.hpp"

namespace warmonger {

class Warmonger :
    public QGuiApplication
{
    Q_OBJECT
    Q_PROPERTY(QVariant map READ readMap NOTIFY mapChanged)

public:
    Warmonger(int argc, char *argv[]);
    ~Warmonger();

    QVariant readMap() const;

signals:
    void mapChanged() const;

private:
    void setupModels();

    QtQuick2ApplicationViewer viewer;
    core::Loader<core::Map> mapLoader;
    core::Map *map;
};

} // namespace warmonger

#endif // WAMNOGER_H
