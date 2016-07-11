#include "log/LogStream.h"

using namespace warmonger::log;

LogStream::LogStream(LogLevel lvl, const QString &name, const QString &file, const QString &func, int line) :
    QTextStream(),
    buffer(),
    logger(Logger::get(name)),
    record(lvl, name, file, func, line)
{
    this->setString(&this->buffer, QIODevice::WriteOnly);
}

LogStream::~LogStream()
{
    this->record.setMsg(this->buffer);
    this->logger->logRecord(this->record);
}
