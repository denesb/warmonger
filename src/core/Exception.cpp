#include "core/Exception.h"

using namespace warmonger::core;

const QMap<Exception::ErrorCode, QString> Exception::messageDefinitions = {
    std::make_pair(Exception::General, QStringLiteral("Error")),
    std::make_pair(Exception::NullPointer, QStringLiteral("Pointer is null")),
    std::make_pair(Exception::WrongType, QStringLiteral("Failed to extract value from QVariant, enclosed value has wrong type")),
    std::make_pair(Exception::IO, QStringLiteral("IO error")),
    std::make_pair(Exception::FileIO, QStringLiteral("File IO error")),
    std::make_pair(Exception::Serialization, QStringLiteral("Serialization error")),
    std::make_pair(Exception::JsonParse, QStringLiteral("Json parse error")),
    std::make_pair(Exception::UnresolvedReference, QStringLiteral("Unable to resolve reference")),
    std::make_pair(Exception::ResourceLoadFailed, QStringLiteral("Failed to load resource")),
    std::make_pair(Exception::InvalidValue, QStringLiteral("Invalid value")),
    std::make_pair(Exception::NullPath, QStringLiteral("No path specified for file operation")),
    std::make_pair(Exception::EntityAlreadyLoaded, QStringLiteral("Entity is already loaded"))
};

Exception::Exception(ErrorCode code, const QString &details) :
    code(code),
    msg(Exception::messageDefinitions[code]),
    details(details)
{
}

Exception::~Exception()
{
}

Exception::ErrorCode Exception::getErrorCode() const
{
    return this->code;
}

QString Exception::getMsg() const
{
    return this->msg;
}

QString Exception::getDetails() const
{
    return this->details;
}

namespace warmonger {
namespace core {

log::LogStream& operator<<(log::LogStream &stream, const GameException &e)
{
    stream << "warmonger::core::GameException<" << e.getMessage() << ">";
    return stream;
}

log::LogStream& operator<<(log::LogStream &stream, const GameRuleViolationError &e)
{
    stream << "warmonger::core::GameRuleViolationError<" << e.getMessage()
        << ">";
    return stream;
}

log::LogStream& operator<<(log::LogStream &stream, const UnitRecruitError &e)
{
    stream << "warmonger::core::UnitRecruitError<" << e.getMessage()
        << ">";
    return stream;
}

} // namespace core
} // namespace warmonger
