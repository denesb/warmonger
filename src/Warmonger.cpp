#include <QtGui/QGuiApplication>
#include <QQmlContext>
#include <QDebug>

#include "qtquick2applicationviewer.h"
#include "core/GameEngine.h"
#include "ui/model/WorldList.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    core::GameEngine *engine = new core::GameEngine();

    QStringList worldSearchPath;
    worldSearchPath.append("worlds");

    engine->setWorldSearchPath(worldSearchPath);

    ui::model::WorldList *worldList = new ui::model::WorldList();

    worldList->addWorlds(engine->getWorldMetaList());

    QtQuick2ApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("engine", engine);
    viewer.rootContext()->setContextProperty("worldListModel", worldList);
    viewer.setMainQmlFile(QStringLiteral("qml/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
