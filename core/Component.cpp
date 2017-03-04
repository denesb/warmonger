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

#include "core/Component.h"

#include "utils/Logging.h"

namespace warmonger {
namespace core {

Component::Component(ComponentType* type, QObject* parent)
    : QObject(parent)
    , type(type)
{
}

QVariant& Component::operator[](const QString& name)
{
    static QVariant invalidPropertyValue{};

    const auto it = this->properties.find(name);

    if (it == this->properties.end())
    {
        return it->second;
    }
    else
    {
        wError << "Requested non existent property " << name << " for component " << this;

        // in case some previously returned instance was modified
        invalidPropertyValue.clear();

        return invalidPropertyValue;
    }
}

} // namespace core
} // namespace warmonger
