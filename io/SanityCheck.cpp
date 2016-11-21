/**
 * \copyright (C) 2015-2016 Botond Dénes
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

#include <memory>

#include <QString>

#include "core/CampaignMap.h"
#include "core/World.h"
#include "io/File.h"
#include "io/JsonUnserializer.h"
#include "io/SanityCheck.h"
#include "utils/Logging.h"

namespace warmonger {
namespace io {

bool isWorldSane(const QString& path)
{
    std::unique_ptr<core::World> world;

    try
    {
        io::JsonUnserializer unserializer;
        world = io::readWorld(path, unserializer);
    }
    catch (const std::exception& e)
    {
        wError << "Caught exception while trying to load world: " << e.what();
        return false;
    }

    // Check army-types
    if (world->getArmyTypes().empty())
    {
        wError << "The world has no army-types";
        return false;
    }

    // Check banners
    if (world->getBanners().empty())
    {
        wError << "The world has no banners";
        return false;
    }

    // Check civilizations
    if (world->getCivilizations().empty())
    {
        wError << "The world has no civilizations";
        return false;
    }

    // Check colors
    if (world->getColors().empty())
    {
        wError << "The world has no colors";
        return false;
    }

    // Check settlement-types
    if (world->getSettlementTypes().empty())
    {
        wError << "The world has no settlement-types";
        return false;
    }

    // Check terrain-types
    if (world->getTerrainTypes().empty())
    {
        wError << "The world has no terrain-types";
        return false;
    }

    // Check unit-types
    if (world->getUnitTypes().empty())
    {
        wError << "The world has no unit-types";
        return false;
    }

    return true;
}

bool isCampaignMapSane(const QString& path, core::World* world)
{
    std::unique_ptr<core::CampaignMap> campaignMap;

    try
    {
        io::Context ctx;
        io::addWorldToContext(ctx, world);

        io::JsonUnserializer unserializer(ctx);
        campaignMap = io::readCampaignMap(path, unserializer);
    }
    catch (const std::exception& e)
    {
        wError << "Caught exception while trying to load world: " << e.what();
        return false;
    }

    return true;
}

} // namespace io
} // namespace warmonger
