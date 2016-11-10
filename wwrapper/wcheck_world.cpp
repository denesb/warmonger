#include <iostream>
#include <sstream>

#include <boost/log/sinks.hpp>

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

    io::JsonUnserializer unserializer;
    core::World* world{nullptr};

    try
    {
        world = io::readWorld(path, unserializer);
    }
    catch (utils::IOError& ioe)
    {
        wError << "Caught utils::IOError while trying to read world: " << ioe.getMessage();
        FAIL(1);
    }
    catch (utils::ValueError& ve)
    {
        wError << "Caught utils::ValueError while trying to read world: " << ve.getMessage();
        FAIL(1);
    }
    catch (std::exception& e)
    {
        wError << "Caught std::exception while trying to read world: " << e.what();
        FAIL(1);
    }

    wInfo << "Successfully loaded world " << world;

    return 0;
}
