#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include <QtQml/QQmlEngine>

#include "ui/CampaignMiniMap.h"
#include "ui/CampaignMapEditor.h"
#include "ui/Context.h"
#include "ui/SearchPaths.h"
#include "utils/Constants.h"
#include "utils/Logging.h"
#include "utils/Settings.h"

using namespace warmonger;

static void initUi(QQuickView* view, ui::Context* ctx);

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    utils::initSettings();
    utils::initLogging();

    ui::setupSearchPaths();

    QQuickView view;

    ui::Context* ctx = new ui::Context(&view, &view);

    initUi(&view, ctx);

    QObject::connect(view.engine(), &QQmlEngine::quit, &view, &QQuickView::close);

    return app.exec();
}

static void initUi(QQuickView* view, ui::Context* ctx)
{
    Q_INIT_RESOURCE(ui);

    qmlRegisterType<ui::CampaignMiniMap>("Warmonger", 1, 0, "CampaignMiniMap");
    qmlRegisterType<ui::CampaignMapEditor>("Warmonger", 1, 0, "CampaignMapEditor");

    view->rootContext()->setContextProperty("W", ctx);
    view->setSource(QUrl("qrc:/qml/windows/MapEditorWindow.qml"));
    view->show();
}
