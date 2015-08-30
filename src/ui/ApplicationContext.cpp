#include <QColor>
#include <QDir>

#include "log/LogStream.h"
#include "core/Exception.h"
#include "core/WorldSurface.h"
#include "core/Map.h"
#include "core/World.h"
#include "ApplicationContext.h"

using namespace warmonger::ui;

ApplicationContext::ApplicationContext(QObject *parent) :
    QObject(parent),
    map(nullptr)
{
    //FIXME: will need to do this from js
    this->loadMap("prototype");
}

ApplicationContext::~ApplicationContext()
{
}

QVariant ApplicationContext::readMap() const
{
    return QVariant::fromValue<QObject *>(this->map);
}

void ApplicationContext::loadMap(const QString &mapName)
{
    this->map = new core::Map(this);
    this->map->load(this->map->specification(mapName));

    core::World *world = this->map->getWorld();
    world->setSurface("default");
    core::WorldSurface *surface = world->getSurface();

    QDir::setSearchPaths("images", QStringList(surface->getPath()));

    const QString path = surface->getPath();
    this->hexMask.load(path + QStringLiteral("/hexagon_mask.xpm"), "XPM");
}

bool ApplicationContext::hexContains(const QPoint &p) const
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
