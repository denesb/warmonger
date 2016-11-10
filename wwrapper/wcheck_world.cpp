#include <iostream>
#include <memory>
#include <sstream>

#include <boost/log/sinks.hpp>

#include "core/World.h"
#include "io/File.h"
#include "io/JsonUnserializer.h"
#include "utils/Exception.h"
#include "utils/Logging.h"

#define FAIL(retcode)                                                                                                  \
    std::cerr << logStream->str() << "Sanity check failed" << std::endl;                                 \
    return retcode;

using namespace warmonger;

int main(int argc, char* const argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: wcheck_world /path/to/world.wwd" << std::endl;
        return 1;
    }

    utils::initLogging();

    // Construct the sink
    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> TextSink;
    boost::shared_ptr<TextSink> sink = boost::make_shared<TextSink>();

    boost::shared_ptr<std::stringstream> logStream = boost::make_shared<std::stringstream>();

    // Add a stream to write log to
    sink->locked_backend()->add_stream(logStream);

    // Register the sink in the logging core
    boost::log::core::get()->add_sink(sink);

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
