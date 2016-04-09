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

QVariantTypeError::QVariantTypeError(const QString &message) :
    Exception(message)
{
}

ValueError::ValueError(const QString &message) :
    Exception(message)
{
}

GameRuleViolationError::GameRuleViolationError(const QString &message) :
    Exception(message)
{
}

UnitRecruitError::UnitRecruitError(const QString &message) :
    GameRuleViolationError(message)
{
}

MapEditingError::MapEditingError(const QString &message) :
    GameRuleViolationError(message)
{
}
