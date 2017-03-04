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
#include "utils/Logging.h"
#include "core/Utils.h"

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

void World::addBanner(std::unique_ptr<Banner>&& banner)
{
    banner->setParent(this);

    this->banners.push_back(banner.get());

    wDebug << "Added banner " << banner.get() << " to world " << this;

    banner.reset();

    emit bannersChanged();
}

QVariantList World::readBanners() const
{
    return utils::toQVariantList(this->banners);
}

QVariantList World::readCivilizations() const
{
    return utils::toQVariantList(this->civilizations);
}

void World::addCivilization(std::unique_ptr<Civilization>&& civilization)
{
    civilization->setParent(this);

    this->civilizations.push_back(civilization.get());

    wDebug << "Added civilization " << civilization.get() << " to world " << this;

    civilization.reset();

    emit civilizationsChanged();
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

void World::addComponentType(std::unique_ptr<ComponentType>&& componentType)
{
    componentType->setParent(this);

    this->componentTypes.push_back(componentType.get());

    wDebug << "Added componentType " << componentType.get() << " to world " << this;

    componentType.reset();

    emit componentTypesChanged();
}

QVariantList World::readComponentTypes() const
{
    return utils::toQVariantList(this->componentTypes);
}

void World::addEntityType(std::unique_ptr<EntityType>&& entityType)
{
    entityType->setParent(this);

    this->entityTypes.push_back(entityType.get());

    wDebug << "Added entityType " << entityType.get() << " to world " << this;

    entityType.reset();

    emit entityTypesChanged();
}

QVariantList World::readEntityTypes() const
{
    return utils::toQVariantList(this->entityTypes);
}

} // namespace core
} // namespace warmonger
