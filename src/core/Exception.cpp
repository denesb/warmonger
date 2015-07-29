#include "core/Exception.h"

using namespace warmonger::core;

const QMap<Exception::ErrorCode, QString> Exception::messageDefinitions = {
    std::make_pair(Exception::General, QStringLiteral("General exception")),
    std::make_pair(Exception::NullPointer, QStringLiteral("Null pointer exception")),
    std::make_pair(Exception::IO, QStringLiteral("IO error")),
    std::make_pair(Exception::FileNotFound, QStringLiteral("File %1 not found")),
    std::make_pair(Exception::FileOpenFailed, QStringLiteral("Cannot open file %1")),
    std::make_pair(Exception::FileReadFailed, QStringLiteral("Cannot read file %1")),
    std::make_pair(Exception::Serialization, QStringLiteral("Serialization error")),
    std::make_pair(Exception::JsonParse, QStringLiteral("Json parse error while parsing %1: %2")),
    std::make_pair(Exception::UnresolvedReference, QStringLiteral("Unable to resolve reference to <%1>%2")),
    std::make_pair(Exception::ResourceLoadFailed, QStringLiteral("Failed to load resource <%1>%2")),
    std::make_pair(Exception::InvalidValue, QStringLiteral("Invalid value %1"))
};

Exception::Exception(ErrorCode code, const QStringList &args) :
    code(code),
    args(args)
{
}

Exception::~Exception()
{
}

Exception::ErrorCode Exception::getErrorCode() const
{
    return this->code;
}

QString Exception::getMessage() const
{
    QString msg = Exception::messageDefinitions[this->code];
    for (const QString str : this->args)
    {
        msg = msg.arg(str);
    }

    return msg;
}

QString Exception::getMessage(ErrorCode code)
{
    return Exception::messageDefinitions[code];
}
