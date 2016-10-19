#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include <QtQml/QQmlEngine>

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

    QQuickView view;

    ui::Context* ctx = new ui::Context(&view, &view);

    ui::initUI();

    Q_INIT_RESOURCE(ui);

    QObject::connect(view.engine(), &QQmlEngine::quit, &view, &QQuickView::close);

    view.rootContext()->setContextProperty("W", ctx);
    view.setSource(QUrl("qrc:/qml/windows/MapEditorWindow.qml"));
    view.show();

    return app.exec();
}
