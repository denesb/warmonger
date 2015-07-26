#ifndef CORE_EXCEPTION_H
#define CORE_EXCEPTION_H

#include <QMap>
#include <QString>
#include <QStringList>

namespace warmonger {
namespace core {

class Exception
{
public:
    enum ErrorCode
    {
        // 0x0000 -> 0x0100 General errors
        General = 0x0000,
        NullPointer,
        // 0x0100 -> 0x0200 IO errors
        IO = 0x0002,
        FileNotFound,
        FileOpenFailed,
        FileReadFailed,
        // 0x0200 -> 0x0300 Serialization (from and to JSON) related errors
        Serialization = 0x0100,
        JsonParse,
        UnresolvedReference,
        ResourceLoadFailed,
        InvalidValue
    };

    Exception(ErrorCode code, const QStringList &args = QStringList());
    virtual ~Exception();

    ErrorCode getErrorCode() const;
    QString getMessage() const;

    static QString getMessage(ErrorCode code);

protected:
    static const QMap<ErrorCode, QString> messageDefinitions;

    ErrorCode code;
    QStringList args;
};

} // core
} // warmonger

#endif // CORE_EXCEPTION_H
