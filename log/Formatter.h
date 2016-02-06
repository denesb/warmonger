#ifndef LOG_FORMATTER_H
#define LOG_FORMATTER_H

#include <QMap>
#include "log/LogRecord.h"

namespace warmonger {
namespace log {

class Formatter
{
public:
    static const QString lvlPlaceHolder;
    static const QString namePlaceHolder;
    static const QString filePlaceHolder;
    static const QString funcPlaceHolder;
    static const QString linePlaceHolder;
    static const QString timePlaceHolder;
    static const QString msgPlaceHolder;

    static const QMap<LogLevel, QString> logLevel2Str;

    static const QString defaultTimeFormat;
    static const QString defaultFormat;

    Formatter(const QString &format = Formatter::defaultFormat);
    virtual ~Formatter();

    void setFormatStr(const QString &formatStr);

    virtual QString format(const LogRecord &record) const;

private:
    QString formatStr;
};

} // namespace log
} // namespace warmonger

#endif // LOG_FORMATTER_H
