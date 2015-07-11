#include <QGuiApplication>
#include <QQmlContext>
#include <QDebug>

#include "Warmonger.h"
#include "qtquick2applicationviewer.h"

using namespace warmonger;

Warmonger::Warmonger(QObject *parent) :
    QObject(parent),
    worldLoader(nullptr),
    mapLoader(nullptr),
    world(nullptr),
    map(nullptr)
{
}

Warmonger::~Warmonger()
{
}

int Warmonger::exec(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    this->setupModels();
    this->setupViews();

    return app.exec();
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

void Warmonger::setupViews()
{
    QtQuick2ApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("warmonger", this);
    viewer.setMainQmlFile(QStringLiteral("qml/Main.qml"));
    viewer.showExpanded();
}
