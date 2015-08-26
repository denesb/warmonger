#include <QDir>
#include <QQmlContext>

#include "Warmonger.h"
#include "log/LogStream.h"
#include "log/ConsoleHandler.h"
#include "log/Formatter.h"

using namespace warmonger;

Warmonger::Warmonger(int argc, char *argv[]) :
    QGuiApplication(argc, argv),
    viewer(),
    ctx(nullptr)
{
    this->readSettings();
    this->initLogger();

    try
    {
        this->initUi();
    }
    catch (core::Exception &e)
    {
        wCritical("ui") << "Caught exception: " << e.getMessage();
        throw;
    }
}

Warmonger::~Warmonger()
{
}

void Warmonger::readSettings()
{
    //FIXME: these search paths will need to come from some sort of settings
    QDir::setSearchPaths("maps", QStringList("worlds/iron_age/maps/prototype"));
    QDir::setSearchPaths("worlds", QStringList("worlds/iron_age"));
}

void Warmonger::initLogger()
{
    log::Logger::init();

    const QString formatStr("%{level} {%{name}}: %{message}");
    std::shared_ptr<log::Formatter> formatter(new log::Formatter(formatStr));

    std::shared_ptr<log::ConsoleHandler> consoleHandler(new log::ConsoleHandler());
    consoleHandler->setLevel(log::Debug);
    consoleHandler->setFormatter(formatter);

    log::Logger *rootLogger = log::Logger::get("root");
    rootLogger->addHandler(consoleHandler);
}

void Warmonger::initUi()
{
    this->ctx = new ui::ApplicationContext(this);

    this->viewer.rootContext()->setContextProperty("W", this->ctx);

    this->viewer.setMainQmlFile(QStringLiteral("qml/Main.qml"));
    this->viewer.showExpanded();
}
