#ifndef LOG_LOGGER_H
#define LOG_LOGGER_H

#include <QString>
#include <QMap>

#include "log/LogLevel.h"
#include "log/Handler.h"

namespace warmonger {
namespace log {

class Logger
{
public:
    static void init();
    static Logger * get(const QString &name);

    QString getName() const;
    const Logger * getParent() const;

    void addHandler(const std::shared_ptr<Handler> &handler);
    void removeHandler(const std::shared_ptr<Handler> &handler);

    void logRecord(const LogRecord &record) const;

private:
    Logger(const QString &name, Logger *parent);
    ~Logger();

    static QMap<QString, Logger*> loggers;

    QString name;
    Logger *parent;
    QList<std::shared_ptr<Handler>> handlers;
};

} // namespace log
} // namespace warmonger

#endif // LOG_LOGGER_H
