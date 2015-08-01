#include <QQmlContext>

#include "Warmonger.h"
#include "core/Exception.h"
#include "log/LogStream.h"
#include "log/ConsoleHandler.h"
#include "ui/Hexagon.h"

using namespace warmonger;

Warmonger::Warmonger(int argc, char *argv[]) :
    QGuiApplication(argc, argv),
    viewer(),
    mapLoader(this),
    map(nullptr)
{
    log::Logger::init();

    std::shared_ptr<log::ConsoleHandler> consoleHandler(new log::ConsoleHandler());
    consoleHandler->setLevel(log::Debug);

    log::Logger *rootLogger = log::Logger::get("root");
    rootLogger->addHandler(consoleHandler);

    try
    {
        this->setupModels();

        this->viewer.rootContext()->setContextProperty("warmonger", this);
        this->viewer.setMainQmlFile(QStringLiteral("qml/Main.qml"));
        this->viewer.showExpanded();

//        emit mapChanged();
    }
    catch (core::Exception &e)
    {
        wCritical("warmonger") << "Caught exception: " << e.getMessage();
        throw;
    }
}

Warmonger::~Warmonger()
{
}

QVariant Warmonger::readMap() const
{
    return QVariant::fromValue<QObject *>(this->map);
}

QVariant Warmonger::createHexagon(const QPoint &boundingRectCorner, int boundingRectSize, int side)
{
    ui::Hexagon *h = new ui::Hexagon(boundingRectCorner, boundingRectSize, side);
    return QVariant::fromValue<QObject *>(h);
}

void Warmonger::setupModels()
{
    QStringList mapSearchPath;
    mapSearchPath << "worlds/iron_age/maps";
    this->mapLoader.setSearchPath(mapSearchPath);

    this->map = this->mapLoader.load("prototype");
}
