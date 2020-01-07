/** \file
 * Exception classes.
 *
 * \copyright (C) 2015-2020 Botond Dénes
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

#include "utils/Exception.h"

#include <fmt/format.h>

namespace warmonger {
namespace utils {

namespace {

std::string toString(godot::Error godotErrorCode)
{
    switch (godotErrorCode)
    {
        case godot::Error::OK:
            return "OK";
        case godot::Error::FAILED:
            return "FAILED";
        case godot::Error::ERR_UNAVAILABLE:
            return "ERR_UNAVAILABLE";
        case godot::Error::ERR_UNCONFIGURED:
            return "ERR_UNCONFIGURED";
        case godot::Error::ERR_UNAUTHORIZED:
            return "ERR_UNAUTHORIZED";
        case godot::Error::ERR_PARAMETER_RANGE_ERROR:
            return "ERR_PARAMETER_RANGE_ERROR";
        case godot::Error::ERR_OUT_OF_MEMORY:
            return "ERR_OUT_OF_MEMORY";
        case godot::Error::ERR_FILE_NOT_FOUND:
            return "ERR_FILE_NOT_FOUND";
        case godot::Error::ERR_FILE_BAD_DRIVE:
            return "ERR_FILE_BAD_DRIVE";
        case godot::Error::ERR_FILE_BAD_PATH:
            return "ERR_FILE_BAD_PATH";
        case godot::Error::ERR_FILE_NO_PERMISSION:
            return "ERR_FILE_NO_PERMISSION";
        case godot::Error::ERR_FILE_ALREADY_IN_USE:
            return "ERR_FILE_ALREADY_IN_USE";
        case godot::Error::ERR_FILE_CANT_OPEN:
            return "ERR_FILE_CANT_OPEN";
        case godot::Error::ERR_FILE_CANT_WRITE:
            return "ERR_FILE_CANT_WRITE";
        case godot::Error::ERR_FILE_CANT_READ:
            return "ERR_FILE_CANT_READ";
        case godot::Error::ERR_FILE_UNRECOGNIZED:
            return "ERR_FILE_UNRECOGNIZED";
        case godot::Error::ERR_FILE_CORRUPT:
            return "ERR_FILE_CORRUPT";
        case godot::Error::ERR_FILE_MISSING_DEPENDENCIES:
            return "ERR_FILE_MISSING_DEPENDENCIES";
        case godot::Error::ERR_FILE_EOF:
            return "ERR_FILE_EOF";
        case godot::Error::ERR_CANT_OPEN:
            return "ERR_CANT_OPEN";
        case godot::Error::ERR_CANT_CREATE:
            return "ERR_CANT_CREATE";
        case godot::Error::ERR_QUERY_FAILED:
            return "ERR_QUERY_FAILED";
        case godot::Error::ERR_ALREADY_IN_USE:
            return "ERR_ALREADY_IN_USE";
        case godot::Error::ERR_LOCKED:
            return "ERR_LOCKED";
        case godot::Error::ERR_TIMEOUT:
            return "ERR_TIMEOUT";
        case godot::Error::ERR_CANT_CONNECT:
            return "ERR_CANT_CONNECT";
        case godot::Error::ERR_CANT_RESOLVE:
            return "ERR_CANT_RESOLVE";
        case godot::Error::ERR_CONNECTION_ERROR:
            return "ERR_CONNECTION_ERROR";
        case godot::Error::ERR_CANT_AQUIRE_RESOURCE:
            return "ERR_CANT_AQUIRE_RESOURCE";
        case godot::Error::ERR_CANT_FORK:
            return "ERR_CANT_FORK";
        case godot::Error::ERR_INVALID_DATA:
            return "ERR_INVALID_DATA";
        case godot::Error::ERR_INVALID_PARAMETER:
            return "ERR_INVALID_PARAMETER";
        case godot::Error::ERR_ALREADY_EXISTS:
            return "ERR_ALREADY_EXISTS";
        case godot::Error::ERR_DOES_NOT_EXIST:
            return "ERR_DOES_NOT_EXIST";
        case godot::Error::ERR_DATABASE_CANT_READ:
            return "ERR_DATABASE_CANT_READ";
        case godot::Error::ERR_DATABASE_CANT_WRITE:
            return "ERR_DATABASE_CANT_WRITE";
        case godot::Error::ERR_COMPILATION_FAILED:
            return "ERR_COMPILATION_FAILED";
        case godot::Error::ERR_METHOD_NOT_FOUND:
            return "ERR_METHOD_NOT_FOUND";
        case godot::Error::ERR_LINK_FAILED:
            return "ERR_LINK_FAILED";
        case godot::Error::ERR_SCRIPT_FAILED:
            return "ERR_SCRIPT_FAILED";
        case godot::Error::ERR_CYCLIC_LINK:
            return "ERR_CYCLIC_LINK";
        case godot::Error::ERR_INVALID_DECLARATION:
            return "ERR_INVALID_DECLARATION";
        case godot::Error::ERR_DUPLICATE_SYMBOL:
            return "ERR_DUPLICATE_SYMBOL";
        case godot::Error::ERR_PARSE_ERROR:
            return "ERR_PARSE_ERROR";
        case godot::Error::ERR_BUSY:
            return "ERR_BUSY";
        case godot::Error::ERR_SKIP:
            return "ERR_SKIP";
        case godot::Error::ERR_HELP:
            return "ERR_HELP";
        case godot::Error::ERR_BUG:
            return "ERR_BUG";
        case godot::Error::ERR_PRINTER_ON_FIRE:
            return "ERR_PRINTER_ON_FIRE";
        case godot::Error::ERR_WTF:
            return "ERR_WTF";
    }
    std::abort(); // Shouldn't happen.
}

} // anonymous namespace

GodotError::GodotError(godot::Error godotErrorCode, const char* message)
    : Exception(fmt::format("{} due to Godot Error {} ({})", message, toString(godotErrorCode), static_cast<std::underlying_type_t<godot::Error>>(godotErrorCode)))
{
}

GodotError::GodotError(godot::Error godotErrorCode, const std::string& message)
    : Exception(fmt::format("{} due to Godot Error {} ({})", message, toString(godotErrorCode), static_cast<std::underlying_type_t<godot::Error>>(godotErrorCode)))
{
}

} // namespace utils
} // namespace warmonger
