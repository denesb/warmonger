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

static Field* getFieldDefinition(const ComponentType* const type, const QString& name);

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
    const auto field{getFieldDefinition(this->type, name)};
    if (!field)
    {
        wWarning << "Attempt to get value of non-existing field `" << name << "' from `" << type->getName()
                 << "' component";
        return QVariant();
    }
    else
    {
        const auto it = this->fields.find(name);
        return it == this->fields.end() ? QVariant() : it->second;
    }
}

void Component::setField(const QString& name, const QVariant& value)
{
    const auto field{getFieldDefinition(this->type, name)};
    if (!field)
    {
        wWarning << "Attempt to get value of non-existing field `" << name << "' from `" << type->getName()
                 << "' component";
    }
    else
    {
        // TODO: type validation
        auto& currentValue = this->fields[name];

        if (currentValue != value)
        {
            currentValue = value;

            emit fieldChanged();
        }
    }
}

static Field* getFieldDefinition(const ComponentType* const type, const QString& name)
{
    const auto& fields = type->getFields();
    const auto it =
        std::find_if(fields.begin(), fields.end(), [&name](const auto& field) { return field->getName() == name; });
    return it == fields.end() ? nullptr : *it;
}

} // namespace core
} // namespace warmonger
