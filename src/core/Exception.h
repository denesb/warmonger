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
        // 0x0000 -> 0x00ff General errors
        General = 0x0000,
        NullPointer,
        WrongType,
        // 0x0100 -> 0x01ff IO errors
        IO = 0x0100,
        FileNotFound,
        FileOpenFailed,
        FileReadFailed,
        FileWriteFailed,
        // 0x0200 -> 0x02ff Serialization (from and to JSON) related errors
        Serialization = 0x0200,
        JsonParse,
        UnresolvedReference,
        ResourceLoadFailed,
        InvalidValue,
        UknownPath,
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
