#ifndef LOG_LOG_LEVEL_H
#define LOG_LOG_LEVEL_H

namespace warmonger {
namespace log {

enum LogLevel
{
    NotSet = 0x0000,
    Debug = 0x0100,
    Info = 0x0200,
    Warning = 0x0300,
    Error = 0x0400,
    Critical = 0x0500,
    Fatal = 0x0600,
    System = 0x0700
};

} // namespace log
} // namespace warmonger

#endif // LOG_LOG_LEVEL_H
