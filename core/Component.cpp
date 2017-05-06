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

#include <QVariant>

#include "utils/Logging.h"

namespace warmonger {
namespace core {

Component::Component(QObject* parent, int id)
    : WObject(parent, id)
    , type(nullptr)
{
}

void Component::setType(ComponentType* type)
{
    if (this->type != type)
    {
        this->type = type;
        emit typeChanged();
    }
}

QVariant Component::getField(const QString& name) const
{
    const auto it = this->fields.find(name);

    if (it == this->fields.end())
    {
        wError << "Requested non existent field " << name << " for component " << this;

        return QVariant();
    }
    else
    {
        return it->second;
    }
}

void Component::setField(const QString& name, const QVariant& value)
{
    auto& currentValue = this->fields[name];

    if (currentValue != value)
    {
        currentValue = value;

        emit fieldChanged();
    }
}

} // namespace core
} // namespace warmonger
