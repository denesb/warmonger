#include "log/LogStream.h"

using namespace warmonger::log;

LogStream::LogStream(LogLevel lvl, const QString &name, const QString &file, const QString &func, int line) :
    textStream(),
    buffer(),
    logger(Logger::get(name)),
    record(lvl, name, file, func, line)
{
    this->textStream.setString(&this->buffer, QIODevice::WriteOnly);
}

LogStream::~LogStream()
{
    this->record.setMsg(this->buffer);
    this->logger->logRecord(this->record);
}

LogStream& LogStream::operator<<(QChar c)
{
    this->textStream << c;
    return *this;
}

LogStream& LogStream::operator<<(signed short i)
{
    this->textStream << i;
    return *this;
}

LogStream& LogStream::operator<<(float f)
{
    this->textStream << f;
    return *this;
}

LogStream& LogStream::operator<<(const QString & string)
{
    this->textStream << string;
    return *this;
}

LogStream& LogStream::operator<<(char c)
{
    this->textStream << c;
    return *this;
}

LogStream& LogStream::operator<<(unsigned short i)
{
    this->textStream << i;
    return *this;
}

LogStream& LogStream::operator<<(signed int i)
{
    this->textStream << i;
    return *this;
}

LogStream& LogStream::operator<<(unsigned int i)
{
    this->textStream << i;
    return *this;
}

LogStream& LogStream::operator<<(signed long i)
{
    this->textStream << i;
    return *this;
}

LogStream& LogStream::operator<<(unsigned long i)
{
    this->textStream << i;
    return *this;
}

LogStream& LogStream::operator<<(qlonglong i)
{
    this->textStream << i;
    return *this;
}

LogStream& LogStream::operator<<(qulonglong i)
{
    this->textStream << i;
    return *this;
}

LogStream& LogStream::operator<<(double f)
{
    this->textStream << f;
    return *this;
}

LogStream& LogStream::operator<<(QLatin1String string)
{
    this->textStream << string;
    return *this;
}

LogStream& LogStream::operator<<(const QByteArray & array)
{
    this->textStream << array;
    return *this;
}

LogStream& LogStream::operator<<(const char * string)
{
    this->textStream << string;
    return *this;
}

LogStream& LogStream::operator<<(const void * ptr)
{
    this->textStream << ptr;
    return *this;
}

LogStream& LogStream::operator<<(const QPoint &point)
{
    this->textStream << "(" << point.x() << "," << point.y() << ")";
    return *this;
}
