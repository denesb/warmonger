#include "log/LogRecord.h"

using namespace warmonger::log;

LogRecord::LogRecord(LogLevel lvl, const QString &loggerName, const QString &file, const QString &func, int line) :
    lvl(lvl),
    loggerName(loggerName),
    file(file),
    func(func),
    line(line),
    time(QDateTime::currentDateTimeUtc()),
    msg()
{
}

LogLevel LogRecord::getLevel() const
{
    return this->lvl;
}

QString LogRecord::getLoggerName() const
{
    return this->loggerName;
}

QString LogRecord::getFile() const
{
    return this->file;
}

QString LogRecord::getFunc() const
{
    return this->func;
}

int LogRecord::getLine() const
{
    return this->line;
}

QDateTime LogRecord::getTime() const
{
    return this->time;
}

QString LogRecord::getMsg() const
{
    return this->msg;
}

void LogRecord::setMsg(const QString &msg)
{
    this->msg = msg;
}
