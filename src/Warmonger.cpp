#include <QDir>
#include <QQmlContext>

#include "Warmonger.h"
#include "ui/GameMap.h"
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
    this->initUi();
}

Warmonger::~Warmonger()
{
}

void Warmonger::readSettings()
{
    QDir::setSearchPaths("Map", QStringList("worlds/iron_age/maps"));
    QDir::setSearchPaths("World", QStringList("worlds/iron_age"));
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

    qmlRegisterType<ui::GameMap>("Warmonger", 1, 0, "GameMap");

    this->viewer.rootContext()->setContextProperty("W", this->ctx);

    this->viewer.setMainQmlFile(QStringLiteral("qml/Main.qml"));
    this->viewer.showExpanded();
}
