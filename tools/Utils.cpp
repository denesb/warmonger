#include <boost/log/sinks.hpp>

#include "tools/Utils.h"
#include "utils/Logging.h"

namespace warmonger {
namespace tools {

boost::shared_ptr<std::stringstream> setupLogging()
{
    utils::initLogging();

    // Construct the sink
    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> TextSink;
    boost::shared_ptr<TextSink> sink = boost::make_shared<TextSink>();

    boost::shared_ptr<std::stringstream> logStream = boost::make_shared<std::stringstream>();

    // Add a stream to write log to
    sink->locked_backend()->add_stream(logStream);

    // Register the sink in the logging core
    boost::log::core::get()->add_sink(sink);

    return logStream;
}

} // namespace tools
} // namespace warmonger
