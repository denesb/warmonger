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
#include "core/Utils.h"
#include "utils/Logging.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

World::World(const QString& uuid, QObject* parent)
    : QObject(parent)
    , uuid(uuid)
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

Banner* World::createBanner()
{
    auto banner = new Banner(this);

    this->banners.push_back(banner);

    wDebug << "Created banner " << banner << " in world " << this;

    emit bannersChanged();

    return banner;
}

QVariantList World::readBanners() const
{
    return utils::toQVariantList(this->banners);
}

QVariantList World::readCivilizations() const
{
    return utils::toQVariantList(this->civilizations);
}

Civilization* World::createCivilization()
{
    auto civilization = new Civilization(this);

    this->civilizations.push_back(civilization);

    wDebug << "Created civilization " << civilization << " in world " << this;

    emit civilizationsChanged();

    return civilization;
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

QVariantList World::readComponentTypes() const
{
    return utils::toQVariantList(this->componentTypes);
}

WorldComponentType* World::createWorldComponentType(long id)
{
    auto componentType = new WorldComponentType(this, id);

    this->componentTypes.push_back(componentType);

    wDebug << "Created componentType " << componentType << " in world " << this;

    emit componentTypesChanged();

    return componentType;
}

EntityType* World::createEntityType()
{
    auto entityType = new EntityType(this);

    this->entityTypes.push_back(entityType);

    wDebug << "Created entityType " << entityType << " in world " << this;

    emit entityTypesChanged();

    return entityType;
}

QVariantList World::readEntityTypes() const
{
    return utils::toQVariantList(this->entityTypes);
}

} // namespace core
} // namespace warmonger
