/**
 * \copyright (C) 2015-2018 Botond Dénes
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

#include "core/ComponentType.h"

#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

ComponentType::ComponentType(QObject* parent, ObjectId id)
    : WObject(parent, id)
{
}

QVariantList ComponentType::readFields() const
{
    return utils::toQVariantList(this->getFields());
}

FieldsHelper::FieldsHelper(std::initializer_list<FieldParams> fieldParams)
{
    for (auto& param : fieldParams)
    {
        this->fields.push_back(new Field(param.name, param.type, &this->parent));
    }
}

} // namespace core
} // namespace warmonger
