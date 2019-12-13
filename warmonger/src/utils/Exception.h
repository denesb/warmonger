/** \file
 * Exception classes.
 *
 * \copyright (C) 2015-2018 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef W_UTILS_EXCEPTION_H
#define W_UTILS_EXCEPTION_H

#include <sstream>

#include <QString>

#include "utils/ToString.h"

namespace warmonger {
namespace utils {

/**
 * Convenience class to build a message with streams.
 */
struct MsgBuilder
{
    template <typename T>
    MsgBuilder& operator<<(const T& o)
    {
        s << o;
        return *this;
    }

    operator std::string() const
    {
        return s.str();
    }

private:
    std::stringstream s;
};

/**
 * Generic Exception, the father of all exceptions.
 */
class Exception : public std::exception
{
public:
    Exception()
    {
    }

    explicit Exception(const char* message)
        : message(message)
    {
    }

    explicit Exception(const std::string& message)
        : message(message)
    {
    }

    explicit Exception(const QString& message)
        : message(message.toStdString())
    {
    }

    Exception(const Exception& e, const char* message)
        : message(nestMessage(e.what(), message))
    {
    }

    Exception(const Exception& e, const std::string& message)
        : message(nestMessage(e.what(), message))
    {
    }

    Exception(const Exception& e, const QString& message)
        : message(nestMessage(e.what(), message.toStdString()))
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
    std::string nestMessage(const std::string& nestedMessage, const std::string& thisMessage)
    {
        return thisMessage + "\n  caused by: " + nestedMessage;
    }

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
 * Script error.
 *
 * A game script failed to run. This can be any problem that makes the
 * script engine fail.
 */
class ScriptError : public Exception
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

} // namespace utils
} // namespace warmonger

#endif // W_UTILS_EXCEPTION_H
