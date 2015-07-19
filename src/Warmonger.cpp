#include <QQmlContext>

#include "Warmonger.h"
#include "core/Exception.h"
#include "log/LogStream.h"
#include "log/ConsoleHandler.h"

using namespace warmonger;

Warmonger::Warmonger(int argc, char *argv[]) :
    QGuiApplication(argc, argv),
    viewer(),
    mapLoader(this),
    map(nullptr),
    mapModel(nullptr)
{
    log::Logger::init();

    std::shared_ptr<log::ConsoleHandler> consoleHandler(new log::ConsoleHandler());
    consoleHandler->setLevel(log::Debug);

    log::Logger *rootLogger = log::Logger::get("root");
    rootLogger->addHandler(consoleHandler);

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
        wError("warmonger") << "Caught exception: " << e.getMessage();
        throw;
    }
}

Warmonger::~Warmonger()
{
}

void Warmonger::setupModels()
{
    QStringList mapSearchPath;
    mapSearchPath << "worlds/iron_age/maps";
    this->mapLoader.setSearchPath(mapSearchPath);

    this->map = this->mapLoader.load("prototype");
    //wDebug("warmonger") << this->map->toJson();
    //this->mapModel = new ui::MapModel(this->map, this);
}
