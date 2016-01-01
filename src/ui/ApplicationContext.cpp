#include <QColor>
#include <QDir>
#include <QStringList>

#include "core/WorldSurface.h"
#include "core/EntityManager.h"
#include "core/Util.h"
#include "ui/ApplicationContext.h"

using namespace warmonger;
using namespace warmonger::ui;

static const QString loggerName{"ui.ApplicationContext"};

ApplicationContext::ApplicationContext(QObject *parent) :
    QObject(parent),
    maps(),
    map(nullptr),
    game(nullptr),
    world(nullptr)
{
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

void ApplicationContext::loadMaps()
{
    QStringList nameFilters;
    nameFilters << "*.wmd";

    QFlags<QDir::Filter> filters = QDir::Files | QDir::Readable;

    core::EntityManager *em = core::EntityManager::getInstance();

    for (QString path : QDir::searchPaths("Map"))
    {
        QDir mapsDir(path);

        for (QString mapFile : mapsDir.entryList(nameFilters, filters))
        {
            this->maps << em->loadEntityAs<core::Map>(
                mapsDir.absoluteFilePath(mapFile)
            );
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

    this->map = new core::Map(core::EntityManager::getInstance());

    emit this->mapChanged();
}

void ApplicationContext::loadMap(QString objectName)
{
    if (this->map != nullptr)
    {
        delete this->map;
    }
    this->closeGame();

    core::EntityManager *em = core::EntityManager::getInstance();
    this->map = em->loadEntity<core::Map>(objectName);

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

    core::EntityManager *em = core::EntityManager::getInstance();

    this->map = em->loadEntityAs<core::Map>(path);

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

    this->game = new core::Game(core::EntityManager::getInstance());
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

    core::EntityManager *em = core::EntityManager::getInstance();
    this->game = em->loadEntity<core::Game>(objectName);

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

    core::EntityManager *em = core::EntityManager::getInstance();

    this->game = em->loadEntityAs<core::Game>(path);

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

    if (this->world->getSurface() == nullptr)
    {
        this->world->setSurface("default");
    }

    emit worldChanged();
}
