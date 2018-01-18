/**
 * Copyright (C) 2015-2018 Botond Dénes
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

#include "warmonger/Context.h"

#include <QDir>
#include <QGuiApplication>
#include <QStringList>

#include "io/File.h"
#include "utils/Constants.h"
#include "utils/Logging.h"
#include "utils/QVariantUtils.h"
#include "utils/Settings.h"
#include "utils/Utils.h"

namespace warmonger {

static std::vector<std::unique_ptr<core::Map>> loadWorldMaps(core::World* world);

Context::Context(
    std::unique_ptr<core::World>&& world, std::unique_ptr<ui::WorldSurface>&& worldSurface, QObject* parent)
    : QObject(parent)
    , world(world.release())
    , worldSurface(worldSurface.release())
    , map(nullptr)
    , disabledPalette(new ui::Palette(QGuiApplication::palette(), QPalette::Disabled, this))
    , activePalette(new ui::Palette(QGuiApplication::palette(), QPalette::Active, this))
    , inactivePalette(new ui::Palette(QGuiApplication::palette(), QPalette::Inactive, this))
    , normalPalette(new ui::Palette(QGuiApplication::palette(), QPalette::Normal, this))
{
    this->world->setParent(this);
    this->worldSurface->setParent(this);

    auto maps = loadWorldMaps(this->world);

    for (auto& map : maps)
    {
        map->setParent(this);
        this->maps.push_back(map.release());
    }
}

void Context::setMap(core::Map* map)
{
    if (map->getWorld() != this->world)
    {
        wError << "Cannot set campaign-map " << map << " with a different world";
        return;
    }

    if (this->map != map)
    {
        wInfo << "map: `" << this->map << "' -> `" << map << "'";

        this->map = map;

        this->map->setParent(this);

        emit mapChanged();
    }
}

QVariantList Context::readMaps() const
{
    return utils::toQVariantList(this->maps);
}

std::vector<std::unique_ptr<core::Map>> loadWorldMaps(core::World* world)
{
    const QDir mapsDir{utils::worldMapsPath(world->objectName())};

    const QStringList nameFilters{"*." + utils::fileExtensions::mapDefinition};
    const QFlags<QDir::Filter> filters = QDir::Files | QDir::Readable;

    std::vector<std::unique_ptr<core::Map>> maps;

    const QStringList mapEntries = mapsDir.entryList(nameFilters, filters);
    for (const QString& mapFile : mapEntries)
    {
        const QString mapPath = mapsDir.absoluteFilePath(mapFile);

        maps.push_back(io::readMap(mapPath, world));
    }

    wInfo << "Loaded " << maps.size() << " maps for world `" << world->objectName() << "'";

    return maps;
}

} // namespace warmonger
