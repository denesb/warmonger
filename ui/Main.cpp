#include <QDir>
#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>

#include "ui/ApplicationContext.h"
//#include "ui/GameMap.h"
#include "ui/MiniMap.h"
//#include "ui/MapPreview.h"
#include "log/LogStream.h"
#include "log/ConsoleHandler.h"
#include "log/Formatter.h"

using namespace warmonger;

void readSettings();
void initLogger();
void initUi(std::unique_ptr<QQuickView> &view, std::unique_ptr<ui::ApplicationContext> &ctx);

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    readSettings();
    initLogger();

    std::unique_ptr<QQuickView> view{new QQuickView};
    std::unique_ptr<ui::ApplicationContext> ctx{new ui::ApplicationContext()};

    initUi(view, ctx);

    return app.exec();
}

void readSettings()
{
    QDir::setSearchPaths("Map", QStringList("worlds/iron_age/maps"));
    QDir::setSearchPaths("World", QStringList("worlds/iron_age"));
}

void initLogger()
{
    log::Logger::init();

    const QString formatStr("%{level} {%{name}} %{message}");
    std::shared_ptr<log::Formatter> formatter(
        new log::Formatter(formatStr)
    );

    std::shared_ptr<log::ConsoleHandler> consoleHandler(
        new log::ConsoleHandler()
    );
    consoleHandler->setLevel(log::Debug);
    consoleHandler->setFormatter(formatter);

    log::Logger *rootLogger = log::Logger::get("root");
    rootLogger->addHandler(consoleHandler);
}

void initUi(std::unique_ptr<QQuickView> &view, std::unique_ptr<ui::ApplicationContext> &ctx)
{

    //qmlRegisterType<ui::GameMap>("Warmonger", 1, 0, "GameMap");
    qmlRegisterType<ui::MiniMap>("Warmonger", 1, 0, "MiniMap");
    //qmlRegisterType<ui::MapPreview>("Warmonger", 1, 0, "MapPreview");

    view->rootContext()->setContextProperty("W", ctx.get());
    view->setSource(QUrl("qrc:/qml/Main.qml"));
    view->show();
    //viewer.showExpanded();
}
