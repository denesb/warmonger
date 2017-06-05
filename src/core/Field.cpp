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

#include "core/Field.h"

namespace warmonger {
namespace core {

Field::Field(QObject* parent)
    : QObject(parent)
{
}

Field::Field(const QString& name, std::unique_ptr<FieldType>&& type, QObject* parent)
    : QObject(parent)
    , name(name)
    , type(type.release())
{
    this->type->setParent(this);
}

void Field::setName(const QString& name)
{
    if (this->name != name)
    {
        this->name = name;
        emit nameChanged();
    }
}

void Field::setType(std::unique_ptr<FieldType>&& type)
{
    type->setParent(this);
    this->type = type.release();

    emit typeChanged();
}

namespace FieldTypes {

List::List(std::unique_ptr<FieldType>&& valueType)
    : valueType(valueType.release())
{
    this->valueType->setParent(this);
}

Dictionary::Dictionary(std::unique_ptr<FieldType>&& valueType)
    : valueType(valueType.release())
{
    this->valueType->setParent(this);
}

} // namspace FieldTypes

} // namespace core
} // namespace warmonger
