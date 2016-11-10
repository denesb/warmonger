#include <iostream>
#include <memory>

#include "io/File.h"
#include "io/JsonUnserializer.h"
#include "ui/WorldSurface.h"
#include "utils/Exception.h"
#include "wwrapper/Utils.h"

using namespace warmonger;

/**
 * Sanity-check a world-surface.
 *
 * A "sane" world-surface can be loaded and activated without exceptions;
 */
int main(int argc, char* const argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: wcheck_worldsurface /path/to/world.wwd /path/to/worldsurface.wsp" << std::endl;
        return 1;
    }

    boost::shared_ptr<std::stringstream> logStream = wwrapper::setupLogging();

    QString worldPath{argv[1]};
    QString worldSurfacePath{argv[2]};

    wInfo << "world path: " << worldPath;
    wInfo << "world-surface path: " << worldSurfacePath;

    std::unique_ptr<core::World> world;

    try
    {
        io::JsonUnserializer unserializer;
        world.reset(io::readWorld(worldPath, unserializer));
    }
    catch (std::exception& e)
    {
        wError << "Caught exception while trying to read world: " << e.what();
        FAIL(1);
    }

    std::unique_ptr<ui::WorldSurface> worldSurface;

    try
    {
        worldSurface = std::make_unique<ui::WorldSurface>(worldSurfacePath, world.get());
    }
    catch (std::exception& e)
    {
        wError << "Caught exception while trying to read world-surface: " << e.what();
        FAIL(1);
    }

    try
    {
        worldSurface->activate();
    }
    catch (std::exception& e)
    {
        wError << "Caught exception while trying to activate world-surface: " << e.what();
        FAIL(1);
    }

    wInfo << "Successfully loaded world-surface " << worldSurface.get();

    return 0;
}
