#ifndef LOG_HANDLER_H
#define LOG_HANDLER_H

#include <memory>
#include <QList>

#include "log/LogLevel.h"
#include "log/Formatter.h"
#include "log/Filter.h"

namespace warmonger {
namespace log {

class Handler
{
public:
    Handler();
    virtual ~Handler();

    Handler& operator<<(const LogRecord &record);
    void setLevel(LogLevel lvl);
    void setFormatter(std::shared_ptr<Formatter> formatter);
    void addFilter(std::shared_ptr<Filter> filter);
    void removeFiler(std::shared_ptr<Filter> filter);

private:
    virtual void handle(const QString &msg) = 0;

    LogLevel lvl;
    std::shared_ptr<Formatter> formatter;
    QList<std::shared_ptr<Filter>> filters;
};

} // namespace log
} // namespace warmonger

#endif // LOG_HANDLER_H
