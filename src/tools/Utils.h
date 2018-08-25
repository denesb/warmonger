#ifndef W_WWRAPPER_UTILS_H
#define W_WWRAPPER_UTILS_H

#include <iostream>
#include <memory>
#include <sstream>

#include "utils/Logging.h"

namespace warmonger {
namespace tools {

std::shared_ptr<std::stringstream> setupLogging();

template <typename... Args>
void die(std::shared_ptr<std::stringstream> logStream, const char* const msg = nullptr, Args&&... args)
{
    if (msg)
        wError.format(msg, std::forward<Args>(args)...);
    std::cerr << logStream->str() << "Sanity check failed" << std::endl;
    std::exit(1);
}

} // namespace tools
} // namespace warmonger

#endif // W_WWRAPPER_UTILS_H
