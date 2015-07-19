#ifndef LOG_LOG_LEVEL_H
#define LOG_LOG_LEVEL_H

namespace warmonger {
namespace log {

enum LogLevel
{
    NotSet = 0x0000,
    Debug = 0x0010,
    Info = 0x0020,
    Warning = 0x0030,
    Error = 0x0040,
    Critical = 0x0050
};

} // namespace log
} // namespace warmonger

#endif // LOG_LOG_LEVEL_H
