#ifndef LOG_LOG_RECORD_H
#define LOG_LOG_RECORD_H

#include <QDateTime>
#include <QString>

#include "log/LogLevel.h"

namespace warmonger {
namespace log {

class LogRecord
{
public:
    LogRecord(LogLevel lvl, const QString &loggerName, const QString &file, const QString &func, int line);

    LogLevel getLevel() const;
    QString getLoggerName() const;
    QString getFile() const;
    QString getFunc() const;
    int getLine() const;
    QDateTime getTime() const;

    QString getMsg() const;
    void setMsg(const QString &msg);

private:
    LogLevel lvl;
    QString loggerName;
    QString file;
    QString func;
    int line;
    QDateTime time;
    QString msg;
};

} // namespace log
} // namespace warmonger

#endif // LOG_LOG_RECORD_H
