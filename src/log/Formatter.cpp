#include "log/Formatter.h"

using namespace warmonger::log;

const QString Formatter::lvlPlaceHolder("%{level}");
const QString Formatter::namePlaceHolder("%{name}");
const QString Formatter::filePlaceHolder("%{file}");
const QString Formatter::funcPlaceHolder("%{func}");
const QString Formatter::linePlaceHolder("%{line}");
const QString Formatter::timePlaceHolder("%{time}");
const QString Formatter::msgPlaceHolder("%{message}");

const QMap<LogLevel, QString> Formatter::logLevel2Str({
    std::make_pair(Debug, QStringLiteral("DEBUG")),
    std::make_pair(Info, QStringLiteral("INFO")),
    std::make_pair(Warning, QStringLiteral("WARNING")),
    std::make_pair(Error, QStringLiteral("ERROR")),
    std::make_pair(Critical, QStringLiteral("CRITICAL"))
});

const QString Formatter::defaultTimeFormat("yyyy,MM,dd HH:mm:ss:zzz");

const QString Formatter::defaultFormat("%{level} {%{name}}: %{message}");

Formatter::Formatter(const QString &format) :
    formatStr(format)
{
}

Formatter::~Formatter()
{
}

void Formatter::setFormatStr(const QString &formatStr)
{
    this->formatStr = formatStr;
}

QString Formatter::format(const LogRecord &record) const
{
    QString msg{this->formatStr};
    msg = msg.replace(Formatter::lvlPlaceHolder, Formatter::logLevel2Str[record.getLevel()]);
    msg = msg.replace(Formatter::namePlaceHolder, record.getLoggerName());
    msg = msg.replace(Formatter::filePlaceHolder, record.getFile());
    msg = msg.replace(Formatter::funcPlaceHolder, record.getFunc());
    msg = msg.replace(Formatter::linePlaceHolder, QString::number(record.getLine()));
    msg = msg.replace(Formatter::timePlaceHolder, record.getTime().toString(Formatter::defaultTimeFormat));
    msg = msg.replace(Formatter::msgPlaceHolder, record.getMsg());

    return msg;
}
