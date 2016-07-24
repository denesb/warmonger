#include <QDir>
#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include <QtQml/QQmlEngine>

#include "ui/Context.h"
#include "ui/CampaignMapEditor.h"
#include "ui/CampaignMiniMap.h"
#include "utils/Constants.h"
#include "utils/Logging.h"
#include "utils/Settings.h"

using namespace warmonger;

namespace {

void setSearchPaths();
void addSubdirToSearchPath(const QString&worldPath, const QString &subdirName, QStringList &searchPath);
void initUi(QQuickView *view, ui::Context *ctx);

}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    utils::initSettings();
    utils::initLogging();

    setSearchPaths();

    QQuickView view;

    ui::Context *ctx = new ui::Context(&view, &view);

    initUi(&view, ctx);

    QObject::connect(view.engine(), &QQmlEngine::quit, &view, &QQuickView::close);

    return app.exec();
}

namespace {

void setSearchPaths()
{
    const QVariant worldsDirVal = utils::settingsValue(utils::SettingsKey::worldsDir);

    if (worldsDirVal.isNull())
    {
        wError << "worldsDir not found in settings, search paths will not be set";
        return;
    }

    const QString worldsDirPath = worldsDirVal.toString();

    wDebug << "World dir is " << worldsDirPath;

    QStringList worldSearchPath, mapSearchPath, surfaceSearchPath;

    const QDir worldsDir(worldsDirPath);
    const QFileInfoList entries = worldsDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs);
    for (const auto &entry : entries)
    {
        const QString &worldPath = entry.canonicalFilePath();

        worldSearchPath.append(worldPath);

        addSubdirToSearchPath(worldPath, utils::paths::surfaces, surfaceSearchPath);
        addSubdirToSearchPath(worldPath, utils::paths::maps, mapSearchPath);

        wInfo << "Added world " << worldPath << " to world search path";
    }

    QDir::setSearchPaths(utils::searchPaths::world, worldSearchPath);
    QDir::setSearchPaths(utils::searchPaths::worldSurface, surfaceSearchPath);
    QDir::setSearchPaths(utils::searchPaths::campaignMap, mapSearchPath);
}

void addSubdirToSearchPath(const QString&worldPath, const QString &subdirName, QStringList &searchPath)
{
    const QString &subdirPath = worldPath + "/" + subdirName;
    const QFileInfo &subdirDirInfo(subdirPath);

    if (subdirDirInfo.exists() && subdirDirInfo.isDir())
        searchPath.append(subdirDirInfo.canonicalFilePath());

    wInfo << "Added " << subdirPath << " to " << subdirName << " search path";
}

void initUi(QQuickView *view, ui::Context *ctx)
{
    qmlRegisterType<ui::CampaignMiniMap>("Warmonger", 1, 0, "CampaignMiniMap");
    qmlRegisterType<ui::CampaignMapEditor>("Warmonger", 1, 0, "CampaignMapEditor");

    view->rootContext()->setContextProperty("W", ctx);
    view->setSource(QUrl("qrc:/qml/windows/MapEditorWindow.qml"));
    view->show();
}

}
