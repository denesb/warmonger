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

#include "core/World.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

World::World(QObject* parent)
    : QObject(parent)
{
}

void World::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

void World::setArmyTypes(const std::vector<ArmyType*>& armyTypes)
{
    if (this->armyTypes != armyTypes)
    {
        this->armyTypes = armyTypes;
        emit armyTypesChanged();
    }
}

QVariantList World::readArmyTypes() const
{
    return utils::toQVariantList(this->armyTypes);
}

void World::setBanners(const std::vector<Banner*>& banners)
{
    if (this->banners != banners)
    {
        this->banners = banners;
        emit bannersChanged();
    }
}

QVariantList World::readBanners() const
{
    return utils::toQVariantList(this->banners);
}

void World::setCivilizations(const std::vector<Civilization*>& civilizations)
{
    this->civilizations = civilizations;
}

QVariantList World::readCivilizations() const
{
    return utils::toQVariantList(this->civilizations);
}

void World::setColors(const std::vector<QColor>& colors)
{
    if (this->colors != colors)
    {
        this->colors = colors;
        emit colorsChanged();
    }
}

QVariantList World::readColors() const
{
    return utils::toQVariantList(this->colors);
}

void World::setSettlementTypes(const std::vector<SettlementType*>& settlementTypes)
{
    if (this->settlementTypes != settlementTypes)
    {
        this->settlementTypes = settlementTypes;
        emit settlementTypesChanged();
    }
}

QVariantList World::readSettlementTypes() const
{
    return utils::toQVariantList(this->settlementTypes);
}

void World::setTerrainTypes(const std::vector<TerrainType*>& terrainTypes)
{
    if (this->terrainTypes != terrainTypes)
    {
        this->terrainTypes = terrainTypes;
        emit terrainTypesChanged();
    }
}

QVariantList World::readTerrainTypes() const
{
    return utils::toQVariantList(this->terrainTypes);
}

void World::setUnitTypes(const std::vector<UnitType*>& unitTypes)
{
    if (this->unitTypes != unitTypes)
    {
        this->unitTypes = unitTypes;
        emit unitTypesChanged();
    }
}

QVariantList World::readUnitTypes() const
{
    return utils::toQVariantList(this->unitTypes);
}

} // namespace core
} // namespace warmonger
