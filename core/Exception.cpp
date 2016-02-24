#include "core/Exception.h"

using namespace warmonger::core;

Exception::Exception(const QString &message) :
    message(message)
{
}

QString Exception::getMessage() const noexcept
{
    return this->message;
}

const char * Exception::what() const noexcept
{
    return this->message.toStdString().c_str();
}


GameException::GameException(const QString &message) :
    message(message)
{
}

const QString GameException::name{"GameException"};

QString GameException::getMessage() const
{
    return this->message;
}

UnresolvedReferenceError::UnresolvedReferenceError(const QString &message) :
    GameException(message)
{
}

const QString UnresolvedReferenceError::name{"UnresolvedReferenceError"};


QVariantTypeError::QVariantTypeError(const QString &message) :
    GameException(message)
{
}

const QString QVariantTypeError::name{"QVariantTypeError"};


ValueError::ValueError(const QString &message) :
    GameException(message)
{
}

const QString ValueError::name{"ValueError"};


IOError::IOError(const QString &message) :
    GameException(message)
{
}

const QString IOError::name{"IOError"};


JsonParseError::JsonParseError(const QString &message) :
    GameException(message)
{
}

const QString JsonParseError::name{"JsonParseError"};


GameRuleViolationError::GameRuleViolationError(const QString &message) :
    GameException(message)
{
}

const QString GameRuleViolationError::name{"GameRuleViolationError"};


UnitRecruitError::UnitRecruitError(const QString &message) :
    GameRuleViolationError(message)
{
}

const QString UnitRecruitError::name{"UnitRecruitError"};


MapEditingError::MapEditingError(const QString &message) :
    GameRuleViolationError(message)
{
}

const QString MapEditingError::name{"MapEditingError"};


namespace warmonger {
namespace core {

log::LogStream& operator<<(log::LogStream &stream, const GameException &e)
{
    logException(stream, e);
    return stream;
}

log::LogStream& operator<<(log::LogStream &stream, const UnresolvedReferenceError &e)
{
    logException(stream, e);
    return stream;
}

log::LogStream& operator<<(log::LogStream &stream, const QVariantTypeError &e)
{
    logException(stream, e);
    return stream;
}

log::LogStream& operator<<(log::LogStream &stream, const ValueError &e)
{
    logException(stream, e);
    return stream;
}

log::LogStream& operator<<(log::LogStream &stream, const IOError &e)
{
    logException(stream, e);
    return stream;
}

log::LogStream& operator<<(log::LogStream &stream, const JsonParseError &e)
{
    logException(stream, e);
    return stream;
}

log::LogStream& operator<<(log::LogStream &stream, const GameRuleViolationError &e)
{
    logException(stream, e);
    return stream;
}

log::LogStream& operator<<(log::LogStream &stream, const UnitRecruitError &e)
{
    logException(stream, e);
    return stream;
}

log::LogStream& operator<<(log::LogStream &stream, const MapEditingError &e)
{
    logException(stream, e);
    return stream;
}

} // namespace core
} // namespace warmonger
