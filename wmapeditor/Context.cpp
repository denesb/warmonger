/**
 * Copyright (C) 2015-2017 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "wmapeditor/Context.h"

#include <random>

#include <QGuiApplication>
#include <QStringList>

#include "core/MapGenerator.h"
#include "io/File.h"
#include "io/JsonSerializer.h"
#include "io/JsonUnserializer.h"
#include "ui/WorldSurface.h"
#include "utils/Constants.h"
#include "utils/Exception.h"
#include "utils/QVariantUtils.h"
#include "utils/Settings.h"
#include "utils/Utils.h"

namespace warmonger {
namespace wmapeditor {

const QString campaignMapObjectName{"newCampaignMap"};
const QString campaignMapDisplayName{"New campaign map"};
const QString unknownErrorMessage{"Unknown error"};

static QString nextCampaignMapName(const std::vector<core::CampaignMap*>& campaignMaps);
static QString defaultPath(const core::World* world, const core::CampaignMap* campaignMap);

struct WorldInjector
{
    WorldInjector(const std::vector<core::World*>& worlds)
        : worlds(worlds)
    {
    }

    void operator()(const QString& className, const QString& objectName, io::Context& ctx);

    const std::vector<core::World*>& worlds;
};

Context::Context(QObject* parent)
    : QObject(parent)
    , world(nullptr)
    , worldSurface(nullptr)
    , campaignMap(nullptr)
    , disabledPalette(new ui::Palette(QGuiApplication::palette(), QPalette::Disabled, this))
    , activePalette(new ui::Palette(QGuiApplication::palette(), QPalette::Active, this))
    , inactivePalette(new ui::Palette(QGuiApplication::palette(), QPalette::Inactive, this))
    , normalPalette(new ui::Palette(QGuiApplication::palette(), QPalette::Normal, this))
{
    this->loadWorlds();
}

QVariantList Context::readWorlds() const
{
    return utils::toQVariantList(this->worlds);
}

void Context::setCampaignMap(core::CampaignMap* campaignMap)
{
    if (this->campaignMap != campaignMap)
    {
        wInfo << "campaignMap: `" << this->campaignMap << "' -> `" << campaignMap << "'";

        this->campaignMap = campaignMap;
        this->setWorld(this->campaignMap->getWorld());

        this->campaignMap->setParent(this);

        emit campaignMapChanged();
    }
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

void Context::create(warmonger::core::World* world)
{
    core::CampaignMap* map = new core::CampaignMap(this);
    map->setObjectName(nextCampaignMapName(this->campaignMaps));
    map->setDisplayName(campaignMapDisplayName);
    map->setWorld(world);

    const std::vector<core::TerrainType*> terrainTypes = world->getTerrainTypes();
    if (!terrainTypes.empty())
    {
        const std::vector<core::MapNode*> nodes = core::generateMapNodes(10);
        core::generateMapNodeNames(nodes);
        core::generateMapNodeTerrainTypes(nodes, terrainTypes);

        map->setMapNodes(nodes);
    }

    const std::vector<core::Civilization*>& civilizations = world->getCivilizations();

    std::random_device rd;
    std::mt19937 mtd(rd());
    std::uniform_int_distribution<std::size_t> civsDist(0, civilizations.size() - 1);

    map->createFaction(civilizations.at(civsDist(mtd)));
    map->createFaction(civilizations.at(civsDist(mtd)));

    this->setCampaignMap(map);
}

bool Context::save()
{
    return this->saveAs(this->lastPath.isEmpty() ? defaultPath(this->world, this->campaignMap) : this->lastPath);
}

bool Context::saveAs(const QString& path)
{
    try
    {
        this->lastPath = path;

        io::JsonSerializer serializer;
        io::writeCampaignMap(this->campaignMap, path, serializer);
    }
    catch (utils::IOError& e)
    {
        this->lastErrorCategory = ErrorCategory::IOError;
        this->lastErrorMessage = e.getMessage();
        return false;
    }
    catch (utils::Exception& e)
    {
        this->lastErrorCategory = ErrorCategory::InternalError;
        this->lastErrorMessage = e.getMessage();
        return false;
    }
    catch (...)
    {
        this->lastErrorCategory = ErrorCategory::UnknownError;
        this->lastErrorMessage = unknownErrorMessage;
        return false;
    }

    return true;
}

bool Context::load(const QString& path)
{
    try
    {
        WorldInjector injector(this->worlds);
        io::Context ctx(std::function<void(const QString&, const QString&, Context&)>(injector));

        io::JsonUnserializer unserializer;
        this->setCampaignMap(io::readCampaignMap(path, unserializer).release());
    }
    catch (utils::IOError& e)
    {
        this->lastErrorCategory = ErrorCategory::IOError;
        this->lastErrorMessage = e.getMessage();
        return false;
    }
    catch (utils::ValueError& e)
    {
        this->lastErrorCategory = ErrorCategory::ContentError;
        this->lastErrorMessage = e.getMessage();
        return false;
    }
    catch (utils::Exception& e)
    {
        this->lastErrorCategory = ErrorCategory::InternalError;
        this->lastErrorMessage = e.getMessage();
        return false;
    }
    catch (...)
    {
        this->lastErrorCategory = ErrorCategory::UnknownError;
        this->lastErrorMessage = unknownErrorMessage;
        return false;
    }

    this->lastPath = path;

    return true;
}

void Context::setWorld(core::World* world)
{
    if (this->world != world)
    {
        wInfo << "world: `" << this->world << "' -> `" << world << "'";

        this->world = world;

        emit worldChanged();
        emit worldSurfacesChanged();

        const QVariant surfaceVal = settingsValue(this->world, utils::WorldSettingsKey::preferredSurface);
        const std::vector<ui::WorldSurface*>& surfaces = this->worldSurfaces[this->world];

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

void Context::setWorldSurface(ui::WorldSurface* worldSurface)
{
    if (this->worldSurface != worldSurface)
    {
        if (this->worldSurface != nullptr)
            this->worldSurface->deactivate();

        this->worldSurface = worldSurface;

        wInfo << "worldSurface: `" << this->worldSurface << "' -> `" << worldSurface << "'";

        if (this->worldSurface != nullptr)
        {
            this->worldSurface->activate();

            utils::setSettingsValue(
                this->world, utils::WorldSettingsKey::preferredSurface, this->worldSurface->objectName());
        }
        emit worldSurfaceChanged();
    }
}

void Context::setDefaultWorldSurface()
{
    const std::vector<ui::WorldSurface*>& surfaces = this->worldSurfaces[this->world];
    if (surfaces.empty())
    {
        this->setWorldSurface(nullptr);
    }
    else
    {
        this->setWorldSurface(surfaces.front());
    }
}

void Context::loadWorlds()
{
    io::JsonUnserializer worldUnserializer;

    for (QString worldPath : QDir::searchPaths(utils::searchPaths::world))
    {
        QFileInfo fileInfo(worldPath);
        const QString worldDefinitionPath =
            worldPath + "/" + fileInfo.baseName() + "." + utils::fileExtensions::worldDefinition;

        core::World* world = io::readWorld(worldDefinitionPath, worldUnserializer).release();

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

void Context::loadMapsFromDir(const QDir& mapsDir, core::World* world)
{
    const QStringList nameFilters{"*." + utils::fileExtensions::mapDefinition};

    const QFlags<QDir::Filter> filters = QDir::Files | QDir::Readable;

    std::size_t n{0};
    const QStringList mapEntries = mapsDir.entryList(nameFilters, filters);
    for (const QString& mapFile : mapEntries)
    {
        const QString mapPath = mapsDir.absoluteFilePath(mapFile);

        io::Context worldContext;
        io::addWorldToContext(worldContext, world);
        io::JsonUnserializer mapUnserializer(worldContext);

        core::CampaignMap* map = io::readCampaignMap(mapPath, mapUnserializer).release();

        map->setParent(this);

        this->campaignMaps.push_back(map);
        ++n;
    }

    wInfo << "Loaded " << n << " maps for world `" << world->objectName() << "'";
}

void Context::loadSurfacesFromDir(const QDir& surfacesDir, core::World* world)
{
    QStringList nameFilters;
    nameFilters.append("*." + utils::fileExtensions::surfacePackage);

    const QFlags<QDir::Filter> filters = QDir::Files | QDir::Readable;
    std::size_t n{0};

    const QStringList surfaceEntries = surfacesDir.entryList(nameFilters, filters);
    for (const QString& surfaceFile : surfaceEntries)
    {
        const QString surfacePath = surfacesDir.absoluteFilePath(surfaceFile);

        ui::WorldSurface* surface = new ui::WorldSurface(surfacePath, world, this);

        this->worldSurfaces[world].push_back(surface);
        ++n;
    }

    wInfo << "Loaded " << n << " surfaces for world `" << world->objectName() << "'";
}

static QString nextCampaignMapName(const std::vector<core::CampaignMap*>& campaignMaps)
{
    std::vector<QString> names;

    names.reserve(campaignMaps.size());

    for (const core::CampaignMap* campaignMap : campaignMaps)
        names.push_back(campaignMap->objectName());

    std::size_t postFix{0};
    QString objectName{campaignMapObjectName + QString::number(postFix)};

    while (std::find(names.begin(), names.end(), objectName) != names.end())
        objectName = campaignMapObjectName + QString::number(++postFix);

    return objectName;
}

static QString defaultPath(const core::World* world, const core::CampaignMap* campaignMap)
{
    const QString worldsPath{utils::settingsValue(utils::SettingsKey::worldsDir).toString()};
    const QString fileName{utils::makeFileName(campaignMap->objectName(), utils::fileExtensions::mapDefinition)};
    return utils::makePath(worldsPath, world->objectName(), utils::paths::maps, fileName);
}

void WorldInjector::operator()(const QString& className, const QString& objectName, io::Context& ctx)
{
    if (className == core::World::staticMetaObject.className())
    {
        auto it = std::find_if(this->worlds.begin(), this->worlds.end(), utils::QObjectFinder(objectName));
        if (it != this->worlds.end())
            io::addWorldToContext(ctx, *it);
    }
}

} // namespace wmapeditor
} // namespace warmonger
