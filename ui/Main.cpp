#include <QDir>
#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include <QSettings>
#include <QtQml/QQmlEngine>

#include "ui/ApplicationContext.h"
//#include "ui/GameMap.h"
//#include "ui/MiniMap.h"
//#include "ui/MapPreview.h"
#include "ui/CampaignMapEditor.h"
#include "log/LogStream.h"
#include "log/ConsoleHandler.h"
#include "log/Formatter.h"
#include "Constants.h"

namespace warmonger {

const QString ApplicationName{"Warmonger"};
const QString OrganizationName{"Warmonger"};
const QString OrganizationDomain{"warmonger.org"};

} // namespace warmonger

using namespace warmonger;

namespace {

const QString loggerName{"ui.Main"};

void setSearchPaths();
void addSubdirToSearchPath(const QString&worldPath, const QString &subdirName, QStringList &searchPath);
void initLogger();
void initUi(std::unique_ptr<QQuickView> &view, std::unique_ptr<ui::ApplicationContext> &ctx);

}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName(OrganizationName);
    QCoreApplication::setOrganizationDomain(OrganizationDomain);
    QCoreApplication::setApplicationName(ApplicationName);

    initLogger();
    setSearchPaths();

    std::unique_ptr<QQuickView> view{new QQuickView};
    std::unique_ptr<ui::ApplicationContext> ctx{new ui::ApplicationContext()};

    initUi(view, ctx);

    QObject::connect(view->engine(), &QQmlEngine::quit, view.get(), &QQuickView::close);

    return app.exec();
}

namespace {

void setSearchPaths()
{
    const QSettings settings;
    const QVariant worldsDirVal = settings.value("worldsDir");

    if (worldsDirVal.isNull())
    {
        wError(loggerName) << "worldsDir not found in settings, search paths will not be set";
        return;
    }

    const QString worldsDirPath = worldsDirVal.toString();

    wDebug(loggerName) << "World dir is " << worldsDirPath;

    QStringList worldSearchPath, mapSearchPath, surfaceSearchPath;

    const QDir worldsDir(worldsDirPath);
    const QFileInfoList entries = worldsDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs);
    for (const auto &entry : entries)
    {
        const QString &worldPath = entry.canonicalFilePath();

        worldSearchPath.append(worldPath);

        addSubdirToSearchPath(worldPath, paths::surfaces, surfaceSearchPath);
        addSubdirToSearchPath(worldPath, paths::maps, mapSearchPath);

        wInfo(loggerName) << "Added world " << worldPath << " to world search path";
    }

    QDir::setSearchPaths(searchPaths::world, worldSearchPath);
    QDir::setSearchPaths(searchPaths::worldSurface, surfaceSearchPath);
    QDir::setSearchPaths(searchPaths::campaignMap, mapSearchPath);
}

void addSubdirToSearchPath(const QString&worldPath, const QString &subdirName, QStringList &searchPath)
{
    const QString &subdirPath = worldPath + "/" + subdirName;
    const QFileInfo &subdirDirInfo(subdirPath);

    if (subdirDirInfo.exists() && subdirDirInfo.isDir())
        searchPath.append(subdirDirInfo.canonicalFilePath());

    wInfo(loggerName) << "Added " << subdirPath << " to " << subdirName << " search path";
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
    //qmlRegisterType<ui::MiniMap>("Warmonger", 1, 0, "MiniMap");
    //qmlRegisterType<ui::MapPreview>("Warmonger", 1, 0, "MapPreview");
    qmlRegisterType<ui::CampaignMapEditor>("Warmonger", 1, 0, "CampaignMapEditor");

    view->rootContext()->setContextProperty("W", ctx.get());
    view->setSource(QUrl("qrc:/qml/Main.qml"));
    view->show();
}

}
