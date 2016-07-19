#ifndef W_UTILS_LOGGING_H
#define W_UTILS_LOGGING_H

#include "boost/log/trivial.hpp"

#include "utils/ToString.h"

namespace warmonger {
namespace utils {

std::string trim(const char * fileName);

void initLogging();

#define wLog(lvl) BOOST_LOG_TRIVIAL(lvl) << __FILE__ << ":" << __LINE__ << " "
#define wTrace   wLog(severity_level::trace)
#define wDebug   wLog(severity_level::debug)
#define wInfo    wLog(severity_level::info)
#define wWarning wLog(severity_level::warning)
#define wError   wLog(severity_level::error)
#define wFatal   wLog(severity_level::fatal)

} // namespace utils
} // namespace warmonger

#endif // W_UTILS_LOGGING_H
