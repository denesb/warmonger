/**
 * Copyright (C) 2015-2016 Botond Dénes
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

#include <random>

#include <QGuiApplication>
#include <QStringList>

#include "io/File.h"
#include "io/JsonUnserializer.h"
#include "ui/Context.h"
#include "ui/WorldSurface.h"
#include "utils/Constants.h"
#include "utils/QVariantUtils.h"
#include "utils/Settings.h"
#include "utils/Utils.h"

namespace warmonger {
namespace ui {

Context::Context(QObject* parent)
    : QObject(parent)
    , world(nullptr)
    , worldSurface(nullptr)
    , campaignMap(nullptr)
    , disabledPalette(new Palette(QGuiApplication::palette(), QPalette::Disabled, this))
    , activePalette(new Palette(QGuiApplication::palette(), QPalette::Active, this))
    , inactivePalette(new Palette(QGuiApplication::palette(), QPalette::Inactive, this))
    , normalPalette(new Palette(QGuiApplication::palette(), QPalette::Normal, this))
{
    this->loadWorlds();
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

void Context::setWorld(core::World* world)
{
    if (this->world != world)
    {
        wInfo << "world: `" << this->world << "' -> `" << world << "'";

        this->world = world;

        emit worldChanged();
        emit worldSurfacesChanged();

        const QVariant surfaceVal = settingsValue(this->world, utils::WorldSettingsKey::preferredSurface);
        const std::vector<WorldSurface*>& surfaces = this->worldSurfaces[this->world];

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
    const std::vector<WorldSurface*>& surfaces = this->worldSurfaces[this->world];
    if (surfaces.empty())
    {
        this->setWorldSurface(nullptr);
    }
    else
    {
        this->setWorldSurface(surfaces.front());
    }
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
    QStringList nameFilters;
    nameFilters.append("*." + utils::fileExtensions::mapDefinition);

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

        ui::WorldSurface* surface = new WorldSurface(surfacePath, world, this);

        this->worldSurfaces[world].push_back(surface);
        ++n;
    }

    wInfo << "Loaded " << n << " surfaces for world `" << world->objectName() << "'";
}

} // namespace ui
} // namespace warmonger
