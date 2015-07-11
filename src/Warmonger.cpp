#include <QQmlContext>
#include <QDebug>

#include "Warmonger.h"

using namespace warmonger;

Warmonger::Warmonger(int argc, char *argv[]) :
    QGuiApplication(argc, argv),
    viewer(),
    worldLoader(nullptr),
    mapLoader(nullptr),
    world(nullptr),
    map(nullptr)
{
    this->viewer.setMainQmlFile(QStringLiteral("qml/Main.qml"));
    this->viewer.showExpanded();
}

Warmonger::~Warmonger()
{
}

void Warmonger::setupModels()
{
    this->worldLoader = new core::WorldLoader(this);

    QStringList worldSearchPath;
    worldSearchPath << "worlds";
    this->worldLoader->setSearchPath(worldSearchPath);

    this->mapLoader = new core::MapLoader(this);

    QStringList mapSearchPath;
    mapSearchPath << "worlds/default/maps";
    this->mapLoader->setSearchPath(mapSearchPath);

    this->world = this->worldLoader->getWorldList()[0];
    this->map = this->mapLoader->getMapList()[0];
}
