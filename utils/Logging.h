#ifndef W_UTILS_LOGGING_H
#define W_UTILS_LOGGING_H

#include "boost/log/trivial.hpp"

#include "utils/ToString.h"

namespace warmonger {
namespace utils {

std::string trimSrcFilePath(const char* fileName);

void initLogging();

#define wLog(lvl) BOOST_LOG_TRIVIAL(lvl) << warmonger::utils::trimSrcFilePath(__FILE__) << ":" << __LINE__ << " "
#define wTrace   wLog(trace) << __FUNCTION__ << "() "
#define wDebug   wLog(debug) << __FUNCTION__ << "() "
#define wInfo    wLog(info)
#define wWarning wLog(warning)
#define wError   wLog(error)
#define wFatal   wLog(fatal)

} // namespace utils
} // namespace warmonger

#endif // W_UTILS_LOGGING_H
