#ifndef CORE_EXCEPTION_H
#define CORE_EXCEPTION_H

#include <QMap>
#include <QString>
#include <QStringList>

#include "log/LogStream.h"

namespace warmonger {
namespace core {

class Exception
{
public:
    enum ErrorCode
    {
        // 0x0000 -> 0x00ff General errors
        General = 0x0000,
        NullPointer,
        WrongType,
        // 0x0100 -> 0x01ff IO errors
        IO = 0x0100,
        FileIO,
        // 0x0200 -> 0x02ff Serialization (from and to JSON) related errors
        Serialization = 0x0200,
        JsonParse,
        UnresolvedReference,
        ResourceLoadFailed,
        InvalidValue,
        NullPath,
        EntityAlreadyLoaded
    };

    Exception(ErrorCode code, const QString &details = QString());
    virtual ~Exception();

    ErrorCode getErrorCode() const;
    QString getMsg() const;
    QString getDetails() const;

protected:
    static const QMap<ErrorCode, QString> messageDefinitions;

    ErrorCode code;
    QString msg;
    QString details;
};

class GameException
{
public:

    GameException(const QString &message) :
        message(message)
    {}

    QString getMessage() const
    {
        return this->message;
    }

private:
    const QString message;
};

log::LogStream& operator<<(log::LogStream &stream, const GameException &e);

class GameRuleViolationError :
    public GameException
{
public:
    GameRuleViolationError(const QString &message) :
        GameException(message)
    {}
};

log::LogStream& operator<<(log::LogStream &stream, const GameRuleViolationError &e);

class UnitRecruitError :
    public GameRuleViolationError
{
public:
    UnitRecruitError(const QString &message) :
        GameRuleViolationError(message)
    {}
};

log::LogStream& operator<<(log::LogStream &stream, const UnitRecruitError &e);

} // core
} // warmonger

#endif // CORE_EXCEPTION_H
