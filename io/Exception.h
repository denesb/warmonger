#ifndef IO_EXCEPTION_H
#define IO_EXCEPTION_H

#include <QString>

#include "core/Exception.h"

namespace warmonger {
namespace io {

/**
 * Error resolving a refrence to a game-object.
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

/**
 * Error reading/writing a file.
 */
class FileIOError :
    public core::Exception
{
public:
    explicit FileIOError(const QString message=QString()) :
        core::Exception(message)
    {
    }
};

/**
 * Json parse error.
 */
class JsonParseError :
    public core::Exception
{
public:
    explicit JsonParseError(const QString message=QString()) :
        core::Exception(message)
    {
    }
};

} // io
} // warmonger

#endif // IO_EXCEPTION_H
