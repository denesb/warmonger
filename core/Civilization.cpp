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

#include <functional>

#include "core/Civilization.h"
#include "core/UnitType.h"
#include "utils/QVariantUtils.h"

using namespace warmonger::core;

Civilization::Civilization(QObject* parent)
    : QObject(parent)
{
}

QString Civilization::getDisplayName() const
{
    return this->displayName;
}

void Civilization::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

std::vector<UnitType*> Civilization::getUnitTypes() const
{
    return this->unitTypes;
}

QVariantList Civilization::readUnitTypes() const
{
    return utils::toQVariantList(this->unitTypes);
}

void Civilization::setUnitTypes(const std::vector<UnitType*>& unitTypes)
{
    if (this->unitTypes != unitTypes)
    {
        this->unitTypes = unitTypes;
        emit unitTypesChanged();
    }
}

void Civilization::addUnitType(UnitType* unitType)
{
    this->unitTypes.push_back(unitType);
    emit unitTypesChanged();
}
