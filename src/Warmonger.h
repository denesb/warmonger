#ifndef WARMONGER_H
#define WARMONGER_H

#include <QGuiApplication>

#include "ui/qtquick2applicationviewer.h"
#include "core/World.h"
#include "core/WorldLoader.h"
#include "core/Map.h"
#include "core/MapLoader.h"

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
    core::WorldLoader *worldLoader;
    core::MapLoader *mapLoader;
    core::World *world;
    core::Map *map;
};

} // namespace warmonger

#endif // WAMNOGER_H
