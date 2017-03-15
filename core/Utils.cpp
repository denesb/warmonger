/**
 * Copyright (C) 2015-2017 Botond Dénes
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

#include "core/Utils.h"

#include <algorithm>
#include <iterator>

#include "utils/Exception.h"

namespace warmonger {
namespace core {

QString createObjectName(const QObject* const object, int index)
{
    const QMetaObject* metaObject{object->metaObject()};

    const QString fullClassName{metaObject->className()};

    return fullClassName.section("::", -1) + ":" + QString::number(index);
}

std::pair<QString, int> splitObjectName(const QString& objectName)
{
    const QStringList parts{objectName.split(':')};

    if (parts.size() != 2)
        throw utils::ValueError("Invalid objectName format, name " + objectName + " has more than one : characters");

    bool conversionSucceded{false};

    const int index{parts[1].toInt(&conversionSucceded)};

    if (!conversionSucceded)
        throw utils::ValueError("Invalid objectName, name " + objectName + " has non-numeric index");

    return std::make_pair(parts[0], index);
}

} // namespace core
} // namespace warmonger
