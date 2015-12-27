#include <QDir>
#include <QGuiApplication>
#include <QQmlContext>

#include "ui/ApplicationContext.h"
#include "ui/GameMap.h"
#include "ui/MiniMap.h"
#include "ui/MapPreview.h"
#include "ui/UnitRecruit.h"
#include "ui/qtquick2applicationviewer.h"
#include "log/LogStream.h"
#include "log/ConsoleHandler.h"
#include "log/Formatter.h"

static void readSettings();
static void initLogger();
static void initUi(
    QtQuick2ApplicationViewer &viewer,
    warmonger::ui::ApplicationContext &ctx
);

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QtQuick2ApplicationViewer viewer;
    warmonger::ui::ApplicationContext ctx;

    readSettings();
    initLogger();
    initUi(viewer, ctx);

    return app.exec();
}

void readSettings()
{
    QDir::setSearchPaths("Map", QStringList("worlds/iron_age/maps"));
    QDir::setSearchPaths("World", QStringList("worlds/iron_age"));
}

void initLogger()
{
    warmonger::log::Logger::init();

    const QString formatStr("%{level} {%{name}:%{file}:%{line}}: %{message}");
    std::shared_ptr<warmonger::log::Formatter> formatter(
        new warmonger::log::Formatter(formatStr)
    );

    std::shared_ptr<warmonger::log::ConsoleHandler> consoleHandler(
        new warmonger::log::ConsoleHandler()
    );
    consoleHandler->setLevel(warmonger::log::Debug);
    consoleHandler->setFormatter(formatter);

    warmonger::log::Logger *rootLogger = warmonger::log::Logger::get("root");
    rootLogger->addHandler(consoleHandler);
}

void initUi(
    QtQuick2ApplicationViewer &viewer,
    warmonger::ui::ApplicationContext &ctx
)
{
    qmlRegisterType<warmonger::ui::GameMap>("Warmonger", 1, 0, "GameMap");
    qmlRegisterType<warmonger::ui::MiniMap>("Warmonger", 1, 0, "MiniMap");
    qmlRegisterType<warmonger::ui::MapPreview>("Warmonger", 1, 0, "MapPreview");
    qmlRegisterType<warmonger::ui::UnitRecruit>("Warmonger", 1, 0, "UnitRecruit");

    viewer.rootContext()->setContextProperty("W", &ctx);

    viewer.setMainQmlFile(QStringLiteral("qml/Main.qml"));
    viewer.showExpanded();
}
