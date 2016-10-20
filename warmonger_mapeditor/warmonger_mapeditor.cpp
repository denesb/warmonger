#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>

#include "ui/Context.h"
#include "ui/SearchPaths.h"
#include "ui/UI.h"
#include "utils/Constants.h"
#include "utils/Logging.h"
#include "utils/Settings.h"

using namespace warmonger;

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    utils::initSettings();
    utils::initLogging();

    ui::setupSearchPaths();
    ui::initUI();

    Q_INIT_RESOURCE(ui);

    QQmlApplicationEngine engine;
    ui::Context* ctx = new ui::Context(&engine);

    engine.rootContext()->setContextProperty("W", ctx);

    engine.load(QUrl("qrc:/qml/windows/MapEditorWindow.qml"));

    return app.exec();
}
