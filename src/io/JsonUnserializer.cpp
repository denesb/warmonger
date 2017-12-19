/**
 * \copyright (C) 2015-2017 Botond Dénes
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

#include "io/JsonUnserializer.hpp"

namespace warmonger {
namespace io {

QJsonDocument parseJson(const QByteArray& json)
{
    QJsonParseError parseError;

    QJsonDocument doc(QJsonDocument::fromJson(json, &parseError));

    if (parseError.error != QJsonParseError::NoError)
    {
        throw utils::ValueError(parseError.errorString() + " at offset " + QString::number(parseError.offset));
    }

    return doc;
}

} // namespace io
} // namespace warmonger
