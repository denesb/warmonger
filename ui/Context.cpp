#include <QStringList>

#include "io/File.h"
#include "io/JsonUnserializer.h"
#include "ui/Context.h"
#include "ui/WorldSurface.h"
#include "utils/Constants.h"
#include "utils/MapGenerator.h"
#include "utils/QVariantUtils.h"
#include "utils/Utils.h"
#include "utils/Settings.h"

namespace warmonger {
namespace ui {

Context::Context(QQuickWindow *window, QObject *parent) :
    QObject(parent),
    window(window),
    world(nullptr),
    worldSurface(nullptr),
    campaignMap(nullptr),
    game(nullptr)
{
    loadWorlds();

    this->colorPalette["foregroundColor0"] = utils::settingsValue(utils::SettingsKey::foregroundColor0);
    this->colorPalette["foregroundColor1"] = utils::settingsValue(utils::SettingsKey::foregroundColor1);
    this->colorPalette["backgroundColor0"] = utils::settingsValue(utils::SettingsKey::backgroundColor0);
    this->colorPalette["backgroundColor1"] = utils::settingsValue(utils::SettingsKey::backgroundColor1);
    this->colorPalette["focusColor0"] = utils::settingsValue(utils::SettingsKey::focusColor0);
    this->colorPalette["focusColor1"] = utils::settingsValue(utils::SettingsKey::focusColor1);
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
    return utils::toQVariantList(this->worlds);
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
        return utils::toQVariantList(it->second);
    }
}

QVariantList Context::readCampaignMaps() const
{
    return utils::toQVariantList(this->campaignMaps);
}

QVariantMap Context::getColorPalette() const
{
    return this->colorPalette;
}

void Context::newCampaignMap(warmonger::core::World *world)
{
    core::CampaignMap *map = new core::CampaignMap(this);
    map->setObjectName("newMap");
    map->setWorld(world);

    const std::vector<core::TerrainType *> terrainTypes = world->getTerrainTypes();
    if (!terrainTypes.empty())
    {
        const std::vector<core::MapNode *> nodes = utils::generateMapNodes(10);
        utils::generateMapNodeNames(nodes);
        utils::generateMapNodeTerrainTypes(nodes, terrainTypes);

        map->setMapNodes(nodes);
    }

    this->setCampaignMap(map);
}

void Context::setWorld(core::World *world)
{
    if (this->world != world)
    {
        wInfo << "world: `" << this->world << "' -> `" << world << "'";

        this->world = world;

        emit worldChanged();
        emit worldSurfacesChanged();

        const QVariant surfaceVal = settingsValue(this->world, utils::WorldSettingsKey::preferredSurface);
        const std::vector<WorldSurface *>& surfaces = this->worldSurfaces[this->world];

        if (surfaceVal.isNull())
        {
            this->setDefaultWorldSurface();
        }
        else
        {
            const QString surfaceName = surfaceVal.toString();
            const auto it = std::find_if(surfaces.cbegin(), surfaces.cend(), utils::QObjectFinder(surfaceName));

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
        wInfo << "worldSurface: `" << this->worldSurface << "' -> `" << worldSurface << "'";

        emit aboutToChangeWorldSurface();

        this->worldSurface = worldSurface;

        if (this->worldSurface != nullptr)
        {
            utils::setSettingsValue(
                this->world,
                utils::WorldSettingsKey::preferredSurface,
                this->worldSurface->objectName());
        }

        this->worldSurface->activate();

        emit worldSurfaceChanged();
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
        wInfo << "campaignMap: `" << this->campaignMap << "' -> `" << campaignMap << "'";

        this->campaignMap = campaignMap;
        this->setWorld(this->campaignMap->getWorld());

        emit campaignMapChanged();
    }
}

void Context::loadWorlds()
{
    io::JsonUnserializer worldUnserializer;

    for (QString worldPath : QDir::searchPaths(utils::searchPaths::world))
    {
        QFileInfo fileInfo(worldPath);
        const QString worldDefinitionPath = worldPath + "/" + fileInfo.baseName() + "."
            + utils::fileExtensions::worldDefinition;
        core::World *world{nullptr};

        try
        {
            world = io::readWorld(worldDefinitionPath, worldUnserializer);
        }
        catch(const utils::Exception &error)
        {
            wError << "Error loading world " << worldDefinitionPath << ", " << error.getMessage();
            continue;
        }

        wInfo << "Loaded world " << worldDefinitionPath;

        world->setParent(this);
        this->worlds.push_back(world);

        QDir mapsDir(worldPath + "/" + utils::paths::maps);
        if (!mapsDir.exists())
        {
            wInfo << "World " << worldPath << " does not have a maps directory";
            continue;
        }

        this->loadMapsFromDir(mapsDir, world);

        QDir surfacesDir(worldPath + "/" + utils::paths::surfaces);
        if (!surfacesDir.exists())
        {
            wInfo << "World " << worldPath << " does not have a surfaces directory";
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
    nameFilters.append("*." + utils::fileExtensions::mapDefinition);

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
        catch (const utils::Exception &error)
        {
            wError << "Error loading map " << mapPath << ", " << error.getMessage();
            continue;
        }

        map->setParent(this);

        this->campaignMaps.push_back(map);
        ++n;
    }

    wInfo << "Loaded " << n << " maps for world `" << world->objectName() << "'";
}

void Context::loadSurfacesFromDir(const QDir &surfacesDir, core::World *world)
{
    QStringList nameFilters;
    nameFilters.append("*." + utils::fileExtensions::surfacePackage);

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
        catch (const utils::Exception &error)
        {
            wError << "Error loading surface " << surfacePath << ", " << error.getMessage();
            continue;
        }

        this->worldSurfaces[world].push_back(surface);
        ++n;
    }

    wInfo << "Loaded " << n << " surfaces for world `" << world->objectName() << "'";
}

} // namespace ui
} // namespace warmonger
