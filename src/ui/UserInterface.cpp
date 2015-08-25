#include <QQmlContext>
#include <QColor>
#include <QDir>

#include "log/LogStream.h"
#include "core/Exception.h"
#include "core/WorldSurface.h"
#include "core/Map.h"
#include "UserInterface.h"

using namespace warmonger::ui;

UserInterface::UserInterface(QObject *parent) :
    QObject(parent),
    viewer(),
    map(nullptr)
{
    try
    {
        this->setupModels();

        this->viewer.rootContext()->setContextProperty("ui", this);
        this->viewer.setMainQmlFile(QStringLiteral("qml/Main.qml"));
        this->viewer.showExpanded();
    }
    catch (core::Exception &e)
    {
        wCritical("ui") << "Caught exception: " << e.getMessage();
        throw;
    }
}

UserInterface::~UserInterface()
{
}

QVariant UserInterface::readMap() const
{
    return QVariant::fromValue<QObject *>(this->map);
}

Q_INVOKABLE bool UserInterface::hexContains(const QPoint &p) const
{
    const core::World *world = this->map->getWorld();

    int x = p.x();
    int y = p.y();
    const QSize tileSize = world->getSurface()->getTileSize();

    if (x < 0 || x >= tileSize.width() || y < 0 || y >= tileSize.height())
        return false;

    QRgb pixel = this->hexMask.pixel(x, y);
    if (pixel != 0xffffffff)
        return false;

    return true;
}

// FIXME: will need to get rid of this
void UserInterface::setupModels()
{
    //FIXME: these search paths will need to come from some sort of settings
    QDir::setSearchPaths("maps", QStringList("worlds/iron_age/maps/prototype"));
    QDir::setSearchPaths("worlds", QStringList("worlds/iron_age"));

    this->map = new core::Map(this);
    this->map->load(this->map->specification("prototype"));

    const core::World *world = this->map->getWorld();
    const core::WorldSurface *surface = world->getSurface();

    const QString path = surface->getPath();
    this->hexMask.load(path + QStringLiteral("hexagon_mask.xpm"), "XPM");
}
