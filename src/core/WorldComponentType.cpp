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

#include "core/WorldComponentType.h"

namespace warmonger {
namespace core {

WorldComponentType::WorldComponentType(QObject* parent, long id)
    : ComponentType(parent, id)
{
}

void WorldComponentType::setName(const QString& name)
{
    if (name != this->name)
    {
        this->name = name;
        emit nameChanged();
    }
}

Field* WorldComponentType::createField()
{
    auto field = new Field(this);

    this->fields.push_back(field);

    emit fieldsChanged();

    return field;
}

std::unique_ptr<Field> WorldComponentType::removeField(Field* field)
{
    auto it = std::find(this->fields.cbegin(), this->fields.cend(), field);
    if (it != this->fields.end())
    {
        this->fields.erase(it);
        field->setParent(nullptr);

        emit fieldsChanged();

        return std::unique_ptr<Field>(field);
    }
    else
    {
        return std::unique_ptr<Field>();
    }
}

} // namespace core
} // namespace warmonger