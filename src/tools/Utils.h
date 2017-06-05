#ifndef W_WWRAPPER_UTILS_H
#define W_WWRAPPER_UTILS_H

#include <sstream>
#include <memory>

#define FAIL(retcode)                                                                                                  \
    std::cerr << logStream->str() << "Sanity check failed" << std::endl;                                               \
    return retcode;

namespace warmonger {
namespace tools {

std::shared_ptr<std::stringstream> setupLogging();

} // namespace tools
} // namespace warmonger

#endif // W_WWRAPPER_UTILS_H
