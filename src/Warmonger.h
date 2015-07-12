#ifndef WARMONGER_H
#define WARMONGER_H

#include <QGuiApplication>

#include "ui/qtquick2applicationviewer.h"
#include "ui/MapModel.h"
#include "core/World.h"
#include "core/Map.h"
#include "core/Loader.hpp"

namespace warmonger {

class Warmonger :
    public QGuiApplication
{
    Q_OBJECT

public:
    Warmonger(int argc, char *argv[]);
    ~Warmonger();

private:
    void setupModels();

    QtQuick2ApplicationViewer viewer;
    core::Loader<core::World> worldLoader;
    core::Loader<core::Map> mapLoader;
    core::World *world;
    core::Map *map;
    ui::MapModel *mapModel;
};

} // namespace warmonger

#endif // WAMNOGER_H
