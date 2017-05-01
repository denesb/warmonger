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

#include "core/Faction.h"
#include "core/Civilization.h"
#include "core/World.h"

namespace warmonger {
namespace core {

Faction::Faction(QObject* parent, int id)
    : WObject(parent, id)
    , civilization(nullptr)
{
}

void Faction::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

void Faction::setPrimaryColor(const QColor& primaryColor)
{
    if (this->primaryColor != primaryColor)
    {
        this->primaryColor = primaryColor;
        emit primaryColorChanged();
    }
}

void Faction::setSecondaryColor(const QColor& secondaryColor)
{
    if (this->secondaryColor != secondaryColor)
    {
        this->secondaryColor = secondaryColor;
        emit secondaryColorChanged();
    }
}

void Faction::setBanner(Banner* banner)
{
    if (this->banner != banner)
    {
        this->banner = banner;
        emit bannerChanged();
    }
}

void Faction::setCivilization(Civilization* civilization)
{
    if (this->civilization != civilization)
    {
        this->civilization = civilization;
        emit civilizationChanged();
    }
}

} // namespace core
} // namespace warmonger
