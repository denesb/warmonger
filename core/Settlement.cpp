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

#include <algorithm>
#include <iterator>
#include <vector>

#include "core/Settlement.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

Settlement::Settlement(QObject* parent)
    : QObject(parent)
    , type(nullptr)
    , mapNode(nullptr)
    , owner(nullptr)
{
}

Settlement::~Settlement()
{
}

void Settlement::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

void Settlement::setType(SettlementType* type)
{
    if (this->type != type)
    {
        this->type = type;
        emit typeChanged();
    }
}

void Settlement::setMapNode(MapNode* mapNode)
{
    if (this->mapNode != mapNode)
    {
        this->mapNode = mapNode;
        emit mapNodeChanged();
    }
}

void Settlement::setOwner(Faction* owner)
{
    if (this->owner != owner)
    {
        this->owner = owner;
        emit ownerChanged();
    }
}

} // namespace core
} // namespace warmonger
