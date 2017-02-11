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

#include "core/Army.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

Army::Army(QObject* parent)
    : QObject(parent)
    , displayName("")
    , type(nullptr)
    , mapNode(nullptr)
    , owner(nullptr)
{
}

void Army::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

void Army::setType(ArmyType* type)
{
    if (this->type != type)
    {
        this->type = type;
        emit typeChanged();
    }
}

void Army::setMapNode(MapNode* mapNode)
{
    if (this->mapNode != mapNode)
    {
        this->mapNode = mapNode;
        emit mapNodeChanged();
    }
}

void Army::setOwner(Faction* owner)
{
    if (this->owner != owner)
    {
        this->owner = owner;
        emit ownerChanged();
    }
}

void Army::setUnits(const std::vector<Unit*>& units)
{
    if (this->units != units)
    {
        this->units = units;
        emit unitsChanged();
    }
}

QVariantList Army::readUnits() const
{
    return utils::toQVariantList(this->units);
}

} // namespace core
} // namespace warmonger
