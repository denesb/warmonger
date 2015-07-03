#include <QGuiApplication>
#include <QQmlContext>
#include <QDebug>

#include "Warmonger.h"
#include "qtquick2applicationviewer.h"

using namespace warmonger;

Warmonger::Warmonger(QObject *parent) :
    QObject(parent),
    worldLoader(nullptr),
    worldList(nullptr)
{
}

Warmonger::~Warmonger()
{
}

int Warmonger::exec(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    this->setupModels();
    this->setupViews();

    return app.exec();
}

void Warmonger::setupModels()
{
    this->worldLoader = new core::WorldLoader(this);
    this->worldList = new ui::model::WorldList(this);

    QObject::connect(this->worldLoader,
            &core::WorldLoader::worldListChanged,
            this->worldList,
            &ui::model::WorldList::setData);

    //TODO: this does not belong here, will come from settings
    QStringList worldSearchPath;
    worldSearchPath.append("worlds");
    this->worldLoader->setSearchPath(worldSearchPath);
}

void Warmonger::setupViews()
{
    QtQuick2ApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("warmonger", this);
    viewer.setMainQmlFile(QStringLiteral("qml/main.qml"));
    viewer.showExpanded();
}
