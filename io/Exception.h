#ifndef IO_EXCEPTION_H
#define IO_EXCEPTION_H

#include <ostream>

#include <QMap>
#include <QString>
#include <QStringList>

#include "core/Exception.h"

namespace warmonger {
namespace io {


/**
 */
class UnresolvedReferenceError :
    public core::Exception
{
public:
    explicit UnresolvedReferenceError(const QString message=QString()) :
        core::Exception(message)
    {
    }
};

class FileIOError :
    public core::Exception
{
public:
    explicit FileIOError(const QString message=QString()) :
        core::Exception(message)
    {
    }
};

} // io
} // warmonger

#endif // IO_EXCEPTION_H
