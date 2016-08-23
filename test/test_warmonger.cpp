#define CATCH_CONFIG_RUNNER

#include <fstream>

#include <boost/log/sinks.hpp>

#include "test/catch.hpp"

#include "utils/Logging.h"

using namespace warmonger;

int main(int argc, char* const argv[])
{
    utils::initLogging();

    //Construct the sink
    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> TextSink;
    boost::shared_ptr<TextSink> sink = boost::make_shared<TextSink>();

    // Add a stream to write log to
    sink->locked_backend()->add_stream(boost::make_shared<std::ofstream>("test_warmonger.log"));

    // Register the sink in the logging core
    boost::log::core::get()->add_sink(sink);

    return Catch::Session().run(argc, argv);
}
