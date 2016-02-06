#ifndef LOG_LOG_STREAM_H
#define LOG_LOG_STREAM_H

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QSize>
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

class LogStream
{
public:
    LogStream(LogLevel lvl, const QString &name, const QString &file, const QString &func, int line);
    ~LogStream();

    LogStream& operator<<(QChar c);
    LogStream& operator<<(signed short i);
    LogStream& operator<<(float f);
    LogStream& operator<<(const QString & string);
    LogStream& operator<<(char c);
    LogStream& operator<<(unsigned short i);
    LogStream& operator<<(signed int i);
    LogStream& operator<<(unsigned int i);
    LogStream& operator<<(signed long i);
    LogStream& operator<<(unsigned long i);
    LogStream& operator<<(qlonglong i);
    LogStream& operator<<(qulonglong i);
    LogStream& operator<<(double f);
    LogStream& operator<<(QLatin1String string);
    LogStream& operator<<(const QByteArray & array);
    LogStream& operator<<(const char * string);
    LogStream& operator<<(const void * ptr);

    LogStream& operator<<(bool b);

    LogStream& operator<<(const QPoint &point);
    LogStream& operator<<(const QPointF &point);
    LogStream& operator<<(const QRect &rect);
    LogStream& operator<<(const QRectF &rect);
    LogStream& operator<<(const QSize &size);
    LogStream& operator<<(const QSizeF &size);

    LogStream& operator<<(const QJsonValue &value);
    LogStream& operator<<(const QJsonObject &obj);
    LogStream& operator<<(const QJsonArray &array);

    LogStream& operator<<(const QStringList &list);

    LogStream& operator<<(const QObject * const obj);

private:
    QTextStream textStream;
    QString buffer;
    const Logger * const logger;
    LogRecord record;
};

} // namespace log
} // namespace warmonger

#endif // LOG_LOG_STREAM_H
