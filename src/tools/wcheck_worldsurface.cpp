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

#include <backward.hpp>
#include <iostream>
#include <memory>

#include "io/File.h"
#include "tools/Utils.h"
#include "ui/WorldSurface.h"
#include "utils/Exception.h"
#include "utils/Logging.h"

namespace backward {

backward::SignalHandling sh;

} // namespace backward

using namespace warmonger;

/**
 * Sanity-check a world-surface.
 *
 * A "sane" world-surface can be loaded and activated without exceptions.
 */
int main(int argc, char* const argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: wcheck_worldsurface /path/to/world.wwd /path/to/worldsurface.wsp" << std::endl;
        return 1;
    }

    std::shared_ptr<std::stringstream> logStream = tools::setupLogging();

    QString worldPath{argv[1]};
    QString worldSurfacePath{argv[2]};

    wInfo << "world path: " << worldPath;
    wInfo << "world-surface path: " << worldSurfacePath;

    std::unique_ptr<core::World> world;

    try
    {
        world = io::readWorld(worldPath);
    }
    catch (std::exception& e)
    {
        wError << "Unexpected exception while trying to load world: " << e.what()
               << " - the world probably isn't right";
        FAIL(1);
    }

    if (!ui::isWorldSurfaceSane(worldSurfacePath, world.get()))
        FAIL(1);

    return 0;
}
