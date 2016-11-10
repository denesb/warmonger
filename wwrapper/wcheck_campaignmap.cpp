#include <iostream>
#include <memory>
#include <sstream>

#include <boost/log/sinks.hpp>

#include "core/CampaignMap.h"
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
    if (argc < 3)
    {
        std::cout << "Usage: wcheck_campaignmap /path/to/world.wwd /path/to/campaignmap.wmd" << std::endl;
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

    QString worldPath{argv[1]};
    QString campaignMapPath{argv[2]};

    wInfo << "world path: " << worldPath;
    wInfo << "campaign-map path: " << campaignMapPath;

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

    wInfo << "Successfully loaded world " << world.get();

    std::unique_ptr<core::CampaignMap> campaignMap;

    try
    {
        io::Context ctx;
        ctx.add(world.get());

        io::JsonUnserializer unserializer(ctx);
        campaignMap.reset(io::readCampaignMap(campaignMapPath, unserializer));
    }
    catch (std::exception& e)
    {
        wError << "Caught exception while trying to read campaignMap: " << e.what();
        FAIL(1);
    }

    wInfo << "Successfully loaded campaignMap " << campaignMap.get();

    return 0;
}
