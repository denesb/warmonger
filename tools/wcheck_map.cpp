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

#include <iostream>
#include <memory>

#include "core/Map.h"
#include "io/File.h"
#include "io/SanityCheck.h"
#include "tools/Utils.h"
#include "utils/Logging.h"

using namespace warmonger;

/**
 * Sanity-check a campaign-map.
 *
 * A "sane" campaign-map can be loaded without exceptions.
 */
int main(int argc, char* const argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: wcheck_map /path/to/world.wwd /path/to/map.wmd" << std::endl;
        return 1;
    }

    boost::shared_ptr<std::stringstream> logStream = tools::setupLogging();

    QString worldPath{argv[1]};
    QString mapPath{argv[2]};

    wInfo << "world path: " << worldPath;
    wInfo << "campaign-map path: " << mapPath;

    if (!io::isWorldSane(worldPath))
    {
        wError << "World is not sane";
        FAIL(1);
    }

    std::unique_ptr<core::World> world(io::readWorld(worldPath));

    wInfo << "Successfully loaded world " << world.get();

    if (!io::isMapSane(mapPath, world.get()))
        FAIL(1);

    return 0;
}
