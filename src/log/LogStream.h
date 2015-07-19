#ifndef LOG_LOG_STREAM_H
#define LOG_LOG_STREAM_H

#include <QTextStream>

#include "log/Logger.h"

#define wLog(lvl, name) warmonger::log::LogStream(lvl, name, __FILE__, __func__, __LINE__)
#define wDebug(name) wLog(warmonger::log::Debug, name)
#define wInfo(name) wLog(warmonger::log::Info, name)
#define wWarning(name) wLog(warmonger::log::Warning, name)
#define wError(name) wLog(warmonger::log::Error, name)
#define wCritical(name) wLog(warmonger::log::Critical, name)

namespace warmonger {
namespace log {

class LogStream :
    public QTextStream
{
public:
    LogStream(LogLevel lvl, const QString &name, const QString &file, const QString &func, int line);
    ~LogStream();

private:
    QString buffer;
    const Logger * const logger;
    LogRecord record;
};

} // namespace log
} // namespace warmonger

#endif // LOG_LOG_STREAM_H
