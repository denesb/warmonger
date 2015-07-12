#ifndef ERROR_H
#define ERROR_H

#include <QMap>
#include <QString>
#include <QStringList>

namespace warmonger {
namespace core {

class Error
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

    Error(ErrorCode code, const QStringList &args = QStringList());
    virtual ~Error();

    QString getMessage();

protected:
    static const QMap<ErrorCode, QString> messageDefinitions;

    ErrorCode code;
    QStringList args;
};

} // core
} // warmonger

#endif // ERROR_H
