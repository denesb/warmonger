#ifndef EXCEPTION_H
#define EXCEPTION_H

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
        NullPointer = 0x0001,
        IO = 0x0002,
        FileNotFound = 0x0003,
        CannotOpenFile = 0x0004,
        CannotReadFile = 0x0005,
        // 0x0100 -> 0x0200 Serialization (from and to JSON) related errors
        Serialization = 0x0100,
        JsonParse = 0x0101,
        UnresolvedReference = 0x0102
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

#endif // EXCEPTION_H
