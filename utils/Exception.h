/** \file
 * Exception classes.
 *
 * \copyright (C) 2015-2016 Botond Dénes
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
