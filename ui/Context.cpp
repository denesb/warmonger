#include <QStringList>
#include <QSettings>

#include "core/QVariantUtil.h"
#include "io/File.h"
#include "io/JsonUnserializer.h"
#include "ui/Context.h"
#include "ui/WorldSurface.h"
#include "Constants.h"
#include "Utils.h"

using namespace warmonger;
using namespace warmonger::ui;

static const QString loggerName{"ui.Context"};

Context::Context(QQuickWindow *window, QObject *parent) :
    QObject(parent),
    window(window),
    world(nullptr),
    worldSurface(nullptr),
    campaignMap(nullptr),
    game(nullptr)
{
    loadWorlds();
}

core::World * Context::getWorld() const
{
    return this->world;
}

ui::WorldSurface * Context::getWorldSurface() const
{
    return this->worldSurface;
}

core::CampaignMap * Context::getCampaignMap() const
{
    return this->campaignMap;
}

core::Game * Context::getGame() const
{
    return this->game;
}

QVariantList Context::readWorlds() const
{
    return core::toQVariantList(this->worlds);
}

QVariantList Context::readWorldSurfaces() const
{
    const auto it = this->worldSurfaces.find(this->world);
    if (it == this->worldSurfaces.end())
    {
        return QVariantList();
    }
    else
    {
        return core::toQVariantList(it->second);
    }
}

QVariantList Context::readCampaignMaps() const
{
    return core::toQVariantList(this->campaignMaps);
}

void Context::newMap(warmonger::core::World *world)
{
    core::CampaignMap *map = new core::CampaignMap(this);
    map->setObjectName("newMap");
    map->setWorld(world);

    this->setWorld(world);
    this->setCampaignMap(map);
}

void Context::setWorld(core::World *world)
{
    if (this->world != world)
    {
        wInfo(loggerName) << "world: `" << this->world << "' -> `" << world << "'";

        this->world = world;

        emit worldChanged();
        emit worldSurfacesChanged();

        QSettings settings;
        const QString key(this->world->objectName() + "/surface");
        const QVariant surfaceVal = settings.value(key);
        const std::vector<WorldSurface *>& surfaces = this->worldSurfaces[this->world];

        if (surfaceVal.isNull())
        {
            this->setDefaultWorldSurface();
        }
        else
        {
            const QString surfaceName = surfaceVal.toString();
            const auto it = std::find_if(surfaces.cbegin(), surfaces.cend(), QObjectFinder(surfaceName));

            if (it == surfaces.cend())
            {
                this->setDefaultWorldSurface();
            }
            else
            {
                this->setWorldSurface(*it);
            }
        }
    }
}

void Context::setWorldSurface(ui::WorldSurface *worldSurface)
{
    if (this->worldSurface != worldSurface)
    {
        wInfo(loggerName) << "worldSurface: `" << this->worldSurface << "' -> `" << worldSurface << "'";

        this->worldSurface = worldSurface;
        emit worldSurfaceChanged();

        if (this->worldSurface != nullptr)
        {
            QSettings settings;
            settings.setValue(this->world->objectName() + "/surface", this->worldSurface->objectName());
        }
    }
}

void Context::setDefaultWorldSurface()
{
    const std::vector<WorldSurface *>& surfaces = this->worldSurfaces[this->world];
    if (surfaces.empty())
    {
        this->setWorldSurface(nullptr);
    }
    else
    {
        this->setWorldSurface(surfaces.front());
    }
}

void Context::setCampaignMap(core::CampaignMap *campaignMap)
{
    if (this->campaignMap != campaignMap)
    {
        wInfo(loggerName) << "campaignMap: `" << this->campaignMap << "' -> `" << campaignMap << "'";

        this->campaignMap = campaignMap;
        this->setWorld(this->campaignMap->getWorld());

        emit campaignMapChanged();
    }
}

void Context::loadWorlds()
{
    io::JsonUnserializer worldUnserializer;

    for (QString worldPath : QDir::searchPaths(searchPaths::world))
    {
        QFileInfo fileInfo(worldPath);
        const QString worldDefinitionPath = worldPath + "/" + fileInfo.baseName() + "."
            + fileExtensions::worldDefinition;
        core::World *world{nullptr};

        try
        {
            world = io::readWorld(worldDefinitionPath, worldUnserializer);
        }
        catch(const Exception &error)
        {
            wError(loggerName) << "Error loading world " << worldDefinitionPath << ", " << error.getMessage();
            continue;
        }

        wInfo(loggerName) << "Loaded world " << worldDefinitionPath;

        world->setParent(this);
        this->worlds.push_back(world);

        QDir mapsDir(worldPath + "/" + paths::maps);
        if (!mapsDir.exists())
        {
            wInfo(loggerName) << "World " << worldPath << " does not have a maps directory";
            continue;
        }

        this->loadMapsFromDir(mapsDir, world);

        QDir surfacesDir(worldPath + "/" + paths::surfaces);
        if (!surfacesDir.exists())
        {
            wInfo(loggerName) << "World " << worldPath << " does not have a surfaces directory";
            continue;
        }

        this->loadSurfacesFromDir(surfacesDir, world);
    }

    emit worldsChanged();
    emit worldSurfacesChanged();
    emit campaignMapsChanged();
}

void Context::loadMapsFromDir(const QDir &mapsDir, core::World *world)
{
    QStringList nameFilters;
    nameFilters.append("*." + fileExtensions::mapDefinition);

    const QFlags<QDir::Filter> filters = QDir::Files | QDir::Readable;

    std::size_t n{0};
    const QStringList mapEntries = mapsDir.entryList(nameFilters, filters);
    for (const QString &mapFile : mapEntries)
    {
        const QString mapPath = mapsDir.absoluteFilePath(mapFile);

        io::Context worldContext;
        worldContext.add(world);
        io::JsonUnserializer mapUnserializer(worldContext);

        core::CampaignMap *map{nullptr};
        try
        {
            map = io::readCampaignMap(mapPath, mapUnserializer);
        }
        catch (const Exception &error)
        {
            wError(loggerName) << "Error loading map " << mapPath << ", " << error.getMessage();
            continue;
        }

        map->setParent(this);

        this->campaignMaps.push_back(map);
        ++n;
    }

    wInfo(loggerName) << "Loaded " << n << " maps for world `" << world->objectName() << "'";
}

void Context::loadSurfacesFromDir(const QDir &surfacesDir, core::World *world)
{
    QStringList nameFilters;
    nameFilters.append("*." + fileExtensions::surfacePackage);

    const QFlags<QDir::Filter> filters = QDir::Files | QDir::Readable;
    std::size_t n{0};

    const QStringList surfaceEntries = surfacesDir.entryList(nameFilters, filters);
    for (const QString &surfaceFile : surfaceEntries)
    {
        const QString surfacePath = surfacesDir.absoluteFilePath(surfaceFile);

        ui::WorldSurface *surface{nullptr};
        try
        {
            surface = new WorldSurface(surfacePath, world, window, this);
        }
        catch (const Exception &error)
        {
            wError(loggerName) << "Error loading surface " << surfacePath << ", " << error.getMessage();
            continue;
        }

        this->worldSurfaces[world].push_back(surface);
        ++n;
    }

    wInfo(loggerName) << "Loaded " << n << " surfaces for world `" << world->objectName() << "'";
}
