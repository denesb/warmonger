#ifndef LOG_FILTER_H
#define LOG_FILTER_H

#include "log/LogRecord.h"

namespace warmonger {
namespace log {

class Filter
{
public:
    Filter();
    virtual ~Filter();

    virtual bool filter(const LogRecord &record) = 0;
};

} // namespace log
} // namespace warmonger

#endif // LOG_FILTER_H
