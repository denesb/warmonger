#include <QStringList>
#include <QSettings>

#include "core/QVariantUtil.h"
#include "io/File.h"
#include "io/JsonUnserializer.h"
#include "ui/ApplicationContext.h"
#include "ui/WorldSurface.h"
#include "Constants.h"
#include "Utils.h"

using namespace warmonger;
using namespace warmonger::ui;

static const QString loggerName{"ui.ApplicationContext"};

ApplicationContext::ApplicationContext(QObject *parent) :
    QObject(parent),
    world(nullptr),
    worldSurface(nullptr),
    campaignMap(nullptr),
    game(nullptr)
{
}

core::World * ApplicationContext::getWorld() const
{
    return this->world;
}

ui::WorldSurface * ApplicationContext::getWorldSurface() const
{
    return this->worldSurface;
}

core::CampaignMap * ApplicationContext::getCampaignMap() const
{
    return this->campaignMap;
}

core::Game * ApplicationContext::getGame() const
{
    return this->game;
}

QVariantList ApplicationContext::readWorlds() const
{
    return core::toQVariantList(this->worlds);
}

QVariantList ApplicationContext::readWorldSurfaces() const
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

QVariantList ApplicationContext::readCampaignMaps() const
{
    return core::toQVariantList(this->campaignMaps);
}

void ApplicationContext::newMap(warmonger::core::World *world)
{
    this->campaignMap = new core::CampaignMap(this);
    this->campaignMap->setWorld(world);
}

void ApplicationContext::setWorld(core::World *world)
{
    if (this->world != world)
    {
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

void ApplicationContext::setWorldSurface(ui::WorldSurface *worldSurface)
{
    if (this->worldSurface != worldSurface)
    {
        this->worldSurface = worldSurface;
        emit worldSurfaceChanged();

        if (this->worldSurface != nullptr)
        {
            QSettings settings;
            settings.setValue(this->world->objectName() + "/surface", this->worldSurface->objectName());
        }
    }
}

void ApplicationContext::setDefaultWorldSurface()
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

void ApplicationContext::setMapCampaign(core::CampaignMap *campaignMap)
{
    if (this->campaignMap != campaignMap)
    {
        this->campaignMap = campaignMap;
        this->setWorld(this->campaignMap->getWorld());

        emit campaignMapChanged();
    }
}

void ApplicationContext::loadWorlds()
{
    io::JsonUnserializer worldUnserializer;

    for (QString worldPath : QDir::searchPaths("Worlds"))
    {
        core::World *world{nullptr};

        try
        {
            world = io::readWorld(worldPath, worldUnserializer);
        }
        catch(const Exception &error)
        {
            wError(loggerName) << "Error loading world " << worldPath << ", " << error.getMessage();
            continue;
        }

        world->setParent(this);
        this->worlds.push_back(world);

        io::Context worldContext;
        worldContext.add(world);

        QDir mapsDir(worldPath + "/" + paths::maps);
        if (!mapsDir.exists())
        {
            wInfo(loggerName) << "World " << world->objectName() << " does not have a maps directory";
            continue;
        }

        io::JsonUnserializer mapUnserializer(worldContext);

        this->loadMapsFromDir(mapsDir, mapUnserializer);

        QDir surfacesDir(worldPath + "/" + paths::surfaces);
        if (!surfacesDir.exists())
        {
            wInfo(loggerName) << "World " << world->objectName() << " does not have a surfaces directory";
            continue;
        }

        this->loadSurfacesFromDir(surfacesDir, world);
    }

    emit worldsChanged();
    emit worldSurfacesChanged();
    emit campaignMapsChanged();
}

void ApplicationContext::loadMapsFromDir(const QDir &mapsDir, io::Unserializer &mapUnserializer)
{
    QStringList nameFilters;
    nameFilters.append("*." + fileExtensions::mapDefinition);

    const QFlags<QDir::Filter> filters = QDir::Files | QDir::Readable;

    const QStringList mapEntries = mapsDir.entryList(nameFilters, filters);
    for (const QString &mapFile : mapEntries)
    {
        const QString mapPath = mapsDir.absoluteFilePath(mapFile);

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
    }
}

void ApplicationContext::loadSurfacesFromDir(const QDir &surfacesDir, core::World *world)
{
    QStringList nameFilters;
    nameFilters.append("*." + fileExtensions::surfacePackage);

    const QFlags<QDir::Filter> filters = QDir::Files | QDir::Readable;

    const QStringList surfaceEntries = surfacesDir.entryList(nameFilters, filters);
    for (const QString &surfaceFile : surfaceEntries)
    {
        const QString surfacePath = surfacesDir.absoluteFilePath(surfaceFile);

        ui::WorldSurface *surface{nullptr};
        try
        {
            surface = new WorldSurface(surfacePath, this);
        }
        catch (const Exception &error)
        {
            wError(loggerName) << "Error loading surface " << surfacePath << ", " << error.getMessage();
            continue;
        }

        this->worldSurfaces[world].push_back(surface);
    }
}
