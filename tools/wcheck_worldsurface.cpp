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

#include <iostream>
#include <memory>

#include "io/File.h"
#include "io/JsonUnserializer.h"
#include "io/SanityCheck.h"
#include "tools/Utils.h"
#include "ui/WorldSurface.h"
#include "utils/Exception.h"

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

    boost::shared_ptr<std::stringstream> logStream = tools::setupLogging();

    QString worldPath{argv[1]};
    QString worldSurfacePath{argv[2]};

    wInfo << "world path: " << worldPath;
    wInfo << "world-surface path: " << worldSurfacePath;

    if (!io::isWorldSane(worldPath))
    {
        wError << "World is not sane";
        FAIL(1);
    }

    io::JsonUnserializer unserializer;
    std::unique_ptr<core::World> world;

    try
    {
        io::JsonUnserializer unserializer;
        world = io::readWorld(worldPath, unserializer);
    }
    catch (std::exception& e)
    {
        wError << "Caught exception while trying to read world: " << e.what();
        FAIL(1);
    }

    if (!ui::isWorldSurfaceSane(worldSurfacePath, world.get()))
        FAIL(1);

    return 0;
}
