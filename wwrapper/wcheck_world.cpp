#include <iostream>
#include <memory>

#include "core/World.h"
#include "io/File.h"
#include "io/JsonUnserializer.h"
#include "utils/Exception.h"
#include "wwrapper/Utils.h"

using namespace warmonger;

/**
 * Sanity-check a world.
 *
 * A "sane" world can be loaded without exceptions and it has at least one of
 * every type.
 */
int main(int argc, char* const argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: wcheck_world /path/to/world.wwd" << std::endl;
        return 1;
    }

    boost::shared_ptr<std::stringstream> logStream = wwrapper::setupLogging();

    QString path{argv[1]};

    wInfo << "path: " << path;

    std::unique_ptr<core::World> world;

    try
    {
        io::JsonUnserializer unserializer;
        world.reset(io::readWorld(path, unserializer));
    }
    catch (std::exception& e)
    {
        wError << "Caught exception while trying to read world: " << e.what();
        FAIL(1);
    }

    wInfo << "Successfully loaded world " << world.get();

    bool fail{false};

    // Check army-types
    if (world->getArmyTypes().empty())
    {
        wError << "The world has no army-types";
        fail = true;
    }

    // Check banners
    if (world->getBanners().empty())
    {
        wError << "The world has no banners";
        fail = true;
    }

    // Check civilizations
    if (world->getCivilizations().empty())
    {
        wError << "The world has no civilizations";
        fail = true;
    }

    // Check colors
    if (world->getColors().empty())
    {
        wError << "The world has no colors";
        fail = true;
    }

    // Check settlement-types
    if (world->getSettlementTypes().empty())
    {
        wError << "The world has no settlement-types";
        fail = true;
    }

    // Check unit-types
    if (world->getUnitTypes().empty())
    {
        wError << "The world has no unit-types";
        fail = true;
    }

    // Check terrain-types
    if (world->getTerrainTypes().empty())
    {
        wError << "The world has no terrain-types";
        fail = true;
    }

    if (fail)
    {
        wError << "The world failed the sanity check";
        FAIL(1);
    }

    return 0;
}
