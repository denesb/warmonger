#include <QColor>
#include <QDir>
#include <QStringList>

#include "core/WorldSurface.h"
#include "core/Util.h"
#include "ApplicationContext.h"

using namespace warmonger;
using namespace warmonger::ui;

static const QString category{"ui"};

ApplicationContext::ApplicationContext(QObject *parent) :
    QObject(parent),
    maps(),
    map(nullptr),
    game(nullptr),
    world(nullptr)
{
    core::GameEntity::setOwner(this);
}

ApplicationContext::~ApplicationContext()
{
}

QVariantList ApplicationContext::readMaps() const
{
    return core::toQVariantList<core::Map>(this->maps);
}

core::Map * ApplicationContext::getMap() const
{
    return this->map;
}

core::Game * ApplicationContext::getGame() const
{
    return this->game;
}

core::World * ApplicationContext::getWorld() const
{
    return this->world;
}

bool ApplicationContext::intersects(QRect rect1, QRect rect2) const
{
    return rect1.intersects(rect2);
}

void ApplicationContext::loadMaps()
{
    QStringList nameFilters;
    nameFilters << "*.wmd";

    QFlags<QDir::Filter> filters = QDir::Files | QDir::Readable;

    for (QString path : QDir::searchPaths("Map"))
    {
        QDir mapsDir(path);

        for (QString mapFile : mapsDir.entryList(nameFilters, filters))
        {
            core::Map *map = new core::Map();
            map->loadAs(mapsDir.absoluteFilePath(mapFile));
            map->getWorld()->setSurface("default");
            this->maps << map;
        }
    }

    emit mapsChanged();
}

void ApplicationContext::closeMaps()
{
    for (core::Map *map : this->maps)
    {
        delete map;
    }
    this->maps.clear();

    emit mapsChanged();
}

void ApplicationContext::newMap()
{
    if (this->map != nullptr)
    {
        delete this->map;
    }
    this->closeGame();

    this->map = new core::Map();

    emit this->mapChanged();
}

void ApplicationContext::loadMap(QString objectName)
{
    if (this->map != nullptr)
    {
        delete this->map;
    }
    this->closeGame();

    core::GameEntity *entity = core::GameEntity::get(
        objectName,
        &core::Map::staticMetaObject
    );
    this->map = qobject_cast<core::Map *>(entity);

    this->setWorld(this->map->getWorld());

    emit mapChanged();
}

void ApplicationContext::loadMapFromPath(QString path)
{
    if (this->map != nullptr)
    {
        delete this->map;
    }
    this->closeGame();

    this->map = new core::Map();
    this->map->loadAs(path);

    this->setWorld(this->map->getWorld());

    emit mapChanged();
}

void ApplicationContext::loadMapFromUrl(QUrl url)
{
    this->loadMapFromPath(url.toLocalFile());
}

void ApplicationContext::closeMap()
{
    if (this->map == nullptr) return;

    delete this->map;
    this->map = nullptr;

    emit mapChanged();
}

void ApplicationContext::newGame(warmonger::core::Map *map)
{
    if (this->game != nullptr)
    {
        delete this->game;
    }
    this->closeMap();

    this->game = new core::Game();
    this->game->fromMapJson(map->toJson());

    this->setWorld(this->game->getWorld());

    emit gameChanged();
}

void ApplicationContext::loadGame(QString objectName)
{
    if (this->game != nullptr)
    {
        delete this->game;
    }
    this->closeMap();

    core::GameEntity *entity = core::GameEntity::get(
        objectName,
        &core::Game::staticMetaObject
    );
    this->game = qobject_cast<core::Game *>(entity);

    this->setWorld(this->game->getWorld());

    emit gameChanged();
}

void ApplicationContext::loadGameFromPath(QString path)
{
    if (this->game != nullptr)
    {
        delete this->game;
    }
    this->closeMap();

    this->game = new core::Game();
    this->game->loadAs(path);

    this->setWorld(this->game->getWorld());

    emit gameChanged();
}

void ApplicationContext::loadGameFromUrl(QUrl url)
{
    this->loadGameFromPath(url.toLocalFile());
}

void ApplicationContext::closeGame()
{
    if (this->game == nullptr) return;

    delete this->game;
    this->game = nullptr;

    emit gameChanged();
}

void ApplicationContext::setWorld(core::World *world)
{
    if (this->world == world) return;

    this->world = world;

    QObject::connect(
        this->world,
        &core::World::surfaceChanged,
        this,
        &ApplicationContext::onWorldSurfaceChanged
    );

    if (this->world->getSurface() == nullptr)
    {
        this->world->setSurface("default");
    }

    this->onWorldSurfaceChanged();
    emit worldChanged();
}

void ApplicationContext::onWorldSurfaceChanged()
{
    QStringList searchPath;

    if (this->world != nullptr)
    {
        searchPath << this->world->getSurface()->getPath();
    }

    QDir::setSearchPaths("surface", searchPath);
}
