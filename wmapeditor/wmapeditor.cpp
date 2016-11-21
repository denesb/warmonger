#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>

#include "ui/SearchPaths.h"
#include "ui/UI.h"
#include "utils/Constants.h"
#include "utils/Logging.h"
#include "utils/Settings.h"
#include "wmapeditor/Context.h"

using namespace warmonger;

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    utils::initSettings();
    utils::initLogging();

    ui::setupSearchPaths();
    ui::initUI();

    QQmlApplicationEngine engine;
    wmapeditor::Context* ctx = new wmapeditor::Context(&engine);

    engine.rootContext()->setContextProperty("W", ctx);

    engine.load(QUrl("qrc:/MapEditor.qml"));

    return app.exec();
}
