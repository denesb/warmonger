#ifndef WARMONGER_H
#define WARMONGER_H

#include <QObject>

#include "core/World.h"
#include "core/WorldLoader.h"
#include "core/Map.h"
#include "core/MapLoader.h"

namespace warmonger {

class Warmonger :
    public QObject
{
    Q_OBJECT

public:
    Warmonger(QObject *parent = nullptr);
    ~Warmonger();

    int exec(int argc, char *argv[]);

private:
    void setupModels();
    void setupViews();

    core::WorldLoader *worldLoader;
    core::MapLoader *mapLoader;
    core::World *world;
    core::Map *map;
};

}; // namespace warmonger

#endif // WAMNOGER_H
