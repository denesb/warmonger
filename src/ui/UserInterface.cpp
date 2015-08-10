#include <QQmlContext>
#include <QColor>

#include "log/LogStream.h"
#include "core/Exception.h"
#include "core/Map.h"
#include "UserInterface.h"

using namespace warmonger::ui;

UserInterface::UserInterface(QObject *parent) :
    QObject(parent),
    viewer(),
    mapLoader(this),
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
    const QSize tileSize = world->getTileSize();

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
    QStringList mapSearchPath;
    mapSearchPath << "worlds/iron_age/maps";
    this->mapLoader.setSearchPath(mapSearchPath);

    this->map = this->mapLoader.load("prototype");

    const core::World *world = this->map->getWorld();
    const QString path = world->getPath();

    this->hexMask.load(path + QStringLiteral("/resources/hexagon_mask.xpm"), "XPM");
}
