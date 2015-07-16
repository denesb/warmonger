#include <QQmlContext>
#include <QDebug>

#include "Warmonger.h"
#include "core/Exception.h"

using namespace warmonger;

Warmonger::Warmonger(int argc, char *argv[]) :
    QGuiApplication(argc, argv),
    viewer(),
    worldLoader(this),
    mapLoader(this),
    world(nullptr),
    map(nullptr),
    mapModel(nullptr)
{
    qSetMessagePattern("%{type} - [%{category}] %{file}:%{line} %{message}");
    try
    {
        this->setupModels();

        this->viewer.setMainQmlFile(QStringLiteral("qml/Main.qml"));
        this->viewer.rootContext()->setContextProperty("warmonger", this);
        //this->viewer.rootContext()->setContextProperty("mapModel", this->mapModel);
        this->viewer.showExpanded();
    }
    catch (core::Exception &e)
    {
        throw;
    }
}

Warmonger::~Warmonger()
{
}

void Warmonger::setupModels()
{
    QStringList worldSearchPath;
    worldSearchPath << "worlds";
    this->worldLoader.setSearchPath(worldSearchPath);

    QStringList mapSearchPath;
    mapSearchPath << "worlds/default/maps";
    this->mapLoader.setSearchPath(mapSearchPath);

    this->world = this->worldLoader.get("default");

    qDebug() << this->world->objectName();
    qDebug() << this->world->toJson();

    //this->map = this->mapLoader.get("prototype");
    //this->map->setParent(this);
    //qDebug() << this->map->toJson();
    
    QList<core::Map *> mlist = this->mapLoader.getList();
    for (core::Map *m : mlist)
    {
        qDebug() << m->objectName();
        qDebug() << m->toJson();
    }

    //this->mapModel = new ui::MapModel(this->map, this);
}
