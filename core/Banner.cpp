/**
 * \copyright (C) 2015-2016 Botond Dénes
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

#include "core/Banner.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

Banner::Banner(QObject* parent)
    : QObject(parent)
{
}

void Banner::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

QVariantList Banner::readCivilizations() const
{
    return utils::toQVariantList(this->civilizations);
}

void Banner::setCivilizations(const std::vector<Civilization*>& civilizations)
{
    if (this->civilizations != civilizations)
    {
        this->civilizations = civilizations;
        emit civilizationsChanged();
    }
}

} // namespace core
} // namespace warmonger
