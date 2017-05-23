#ifndef W_WWRAPPER_UTILS_H
#define W_WWRAPPER_UTILS_H

#include <sstream>

#include <boost/shared_ptr.hpp>

#define FAIL(retcode)                                                                                                  \
    std::cerr << logStream->str() << "Sanity check failed" << std::endl;                                               \
    return retcode;

namespace warmonger {
namespace tools {

boost::shared_ptr<std::stringstream> setupLogging();

} // namespace tools
} // namespace warmonger

#endif // W_WWRAPPER_UTILS_H
