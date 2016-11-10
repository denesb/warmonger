#ifndef W_UTILS_EXCEPTION_H
#define W_UTILS_EXCEPTION_H

#include <QString>

namespace warmonger {
namespace utils {

/**
 * Generic Exception, the father of all exceptions.
 */
class Exception : public std::exception
{
public:
    explicit Exception(const QString& message = QString())
        : message(message.toStdString())
    {
    }

    QString getMessage() const noexcept
    {
        return QString::fromStdString(this->message);
    }

    const char* what() const noexcept override
    {
        return this->message.c_str();
    }

protected:
    const std::string message;
};

/**
 * Error reading/writing to some i/o device.
 */
class IOError : public Exception
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
class ValueError : public Exception
{
public:
    using Exception::Exception;
};

/**
 * A game rule was violated
 */
class GameRuleError : public Exception
{
public:
    using Exception::Exception;
};

} // utils
} // warmonger

#endif // W_UTILS_EXCEPTION_H
