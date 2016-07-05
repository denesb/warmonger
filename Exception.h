#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QString>

namespace warmonger {

/**
 * Generic Exception, the father of all exceptions.
 */
class Exception :
    std::exception
{
public:
    explicit Exception(const QString &message = QString()) :
        message(message)
    {}

    QString getMessage() const noexcept
    {
        return this->message;
    }

    const char * what() const noexcept override
    {
        return this->message.toStdString().c_str();
    }

protected:
    const QString message;
};

/**
 * Error reading/writing to some i/o device.
 */
class IOError :
    public Exception
{
public:
    using Exception::Exception;
};

/**
 * Invalid value error.
 *
 * A parameter has an invalid value. This occurs when parameters
 * come from "outside", e.g. files, scripts, network.
 */
class ValueError :
    public Exception
{
public:
    using Exception::Exception;
};

/**
 * A game rule was violated
 */
class GameRuleError :
    public Exception
{
public:
    using Exception::Exception;
};

} // warmonger

#endif // EXCEPTION_H
