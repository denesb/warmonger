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

#include "core/ComponentType.h"
#include "utils/Logging.h"

namespace warmonger {
namespace core {

Component::Component(ComponentType* type, QObject* parent, int id)
    : WObject(parent, id)
    , type(type)
{
}

void Component::checkAndSetFields(std::unordered_map<QString, FieldValue> fields, std::vector<FieldValue*> values)
{
    auto fieldDefs = this->type->getFields();

    for (std::size_t i = 0; i < fieldDefs.size(); ++i)
    {
        const auto& name = fieldDefs[i]->getName();
        auto it = fields.find(name);

        if (it == fields.end())
            continue;

        auto val = std::move(it->second);
        fields.erase(it);

        const auto type = fieldDefs[i]->getType();
        if (val.getType() != type)
        {
            wWarning << "Attempted to set value of field `" << name << "' to incompatible type " << val.getType();
            continue;
        }

        *values[i] = val;
    }

    if (fields.empty())
        return;

    QString msg{"Attempted to set extra fields: "};
    auto it = fields.begin();
    msg = msg + "`" + it->first + "'";

    while (++it != fields.end())
    {
        msg += msg + ", `" + it->first + "'";
    }

    wWarning << msg << " for " << this->type->getName() << " component " << this;
}

void Component::checkAndSetFields(
    std::unordered_map<QString, FieldValue> fields, std::unordered_map<QString, FieldValue>& values)
{
    auto fieldDefs = this->type->getFields();

    for (auto& fieldDef : fieldDefs)
    {
        const auto& name = fieldDef->getName();
        auto it = fields.find(name);

        if (it == fields.end())
            continue;

        auto val = std::move(it->second);
        fields.erase(it);

        const auto type = fieldDef->getType();
        if (val.getType() != type)
        {
            wWarning << "Attempted to set value of field `" << name << "' to incompatible type " << val.getType();
            continue;
        }

        values[name] = val;
    }

    if (fields.empty())
        return;

    QString msg{"Attempted to set extra fields: "};
    auto it = fields.begin();
    msg = msg + "`" + it->first + "'";

    while (++it != fields.end())
    {
        msg += msg + ", `" + it->first + "'";
    }

    wWarning << msg << " for " << this->type->getName() << " component " << this;
}

} // namespace core
} // namespace warmonger
