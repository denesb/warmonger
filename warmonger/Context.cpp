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

#include "warmonger/Context.h"

#include <QDir>
#include <QGuiApplication>
#include <QStringList>

#include "io/File.h"
#include "io/JsonUnserializer.h"
#include "utils/Constants.h"
#include "utils/QVariantUtils.h"
#include "utils/Settings.h"
#include "utils/Utils.h"

namespace warmonger {

static std::vector<std::unique_ptr<core::CampaignMap>> loadWorldMaps(core::World* world);

Context::Context(
    std::unique_ptr<core::World>&& world, std::unique_ptr<ui::WorldSurface>&& worldSurface, QObject* parent)
    : QObject(parent)
    , world(world.release())
    , worldSurface(worldSurface.release())
    , campaignMap(nullptr)
    , disabledPalette(new ui::Palette(QGuiApplication::palette(), QPalette::Disabled, this))
    , activePalette(new ui::Palette(QGuiApplication::palette(), QPalette::Active, this))
    , inactivePalette(new ui::Palette(QGuiApplication::palette(), QPalette::Inactive, this))
    , normalPalette(new ui::Palette(QGuiApplication::palette(), QPalette::Normal, this))
{
    this->world->setParent(this);
    this->worldSurface->setParent(this);

    auto campaignMaps = loadWorldMaps(this->world);

    for (auto& campaignMap : campaignMaps)
    {
        campaignMap->setParent(this);
        this->campaignMaps.push_back(campaignMap.release());
    }
}

void Context::setCampaignMap(core::CampaignMap* campaignMap)
{
    if (campaignMap->getWorld() != this->world)
    {
        wError << "Cannot set campaign-map " << campaignMap << " with a different world";
        return;
    }

    if (this->campaignMap != campaignMap)
    {
        wInfo << "campaignMap: `" << this->campaignMap << "' -> `" << campaignMap << "'";

        this->campaignMap = campaignMap;

        this->campaignMap->setParent(this);

        emit campaignMapChanged();
    }
}

QVariantList Context::readCampaignMaps() const
{
    return utils::toQVariantList(this->campaignMaps);
}

std::vector<std::unique_ptr<core::CampaignMap>> loadWorldMaps(core::World* world)
{
    const QDir mapsDir{utils::worldMapsPath(world->objectName())};

    const QStringList nameFilters{"*." + utils::fileExtensions::mapDefinition};
    const QFlags<QDir::Filter> filters = QDir::Files | QDir::Readable;

    std::vector<std::unique_ptr<core::CampaignMap>> campaignMaps;

    const QStringList mapEntries = mapsDir.entryList(nameFilters, filters);
    for (const QString& mapFile : mapEntries)
    {
        const QString mapPath = mapsDir.absoluteFilePath(mapFile);

        io::Context worldContext;
        io::addWorldToContext(worldContext, world);
        io::JsonUnserializer mapUnserializer(worldContext);

        campaignMaps.push_back(io::readCampaignMap(mapPath, mapUnserializer));
    }

    wInfo << "Loaded " << campaignMaps.size() << " maps for world `" << world->objectName() << "'";

    return campaignMaps;
}

} // namespace warmonger
