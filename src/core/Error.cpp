#include "core/Error.h"

using namespace warmonger::core;


const QMap<Error::ErrorCode, QString> Error::messageDefinitions = {
    std::make_pair(Error::General, QStringLiteral("General error")),
    std::make_pair(Error::NullPointer, QStringLiteral("Null pointer error")),
    std::make_pair(Error::IO, QStringLiteral("IO Error")),
    std::make_pair(Error::FileNotFound, QStringLiteral("File %1 not found")),
    std::make_pair(Error::CannotOpenFile, QStringLiteral("Cannot open file %1")),
    std::make_pair(Error::CannotReadFile, QStringLiteral("Cannot read file %1")),
    std::make_pair(Error::Serialization, QStringLiteral("Serialization error")),
    std::make_pair(Error::JsonParse, QStringLiteral("Json parse error: %1")),
    std::make_pair(Error::UnresolvedReference, QStringLiteral("Unable to resolve reference to (%1)%2"))
};


Error::Error(Error::ErrorCode code, const QStringList &args) :
    code(code),
    args(args)
{
}

Error::~Error()
{
}

QString Error::getMessage()
{
    QString msg = Error::messageDefinitions[this->code];
    for (const QString str : this->args)
    {
        msg = msg.arg(str);
    }

    return msg;
}
