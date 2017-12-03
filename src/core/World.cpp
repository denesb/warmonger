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

#include "core/BuiltInComponentTypes.h"
#include "core/Utils.h"
#include "utils/Logging.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

World::World(const QString& uuid, const std::map<QString, int>& builtInObjectIds, QObject* parent)
    : QObject{parent}
    , uuid{uuid}
    , builtInObjectIds{builtInObjectIds}
    , rulesType{WorldRules::Type::Lua}
    , rules{nullptr}
{
    const auto& builtInComponentTypes = getBuiltInComponentTypesFactories();

    for (const auto& builtInComponentType : builtInComponentTypes)
    {
        const auto it = this->builtInObjectIds.find(std::get<0>(builtInComponentType));

        if (it == this->builtInObjectIds.end())
        {
            this->componentTypes.push_back(std::get<1>(builtInComponentType)(this, WObject::invalidId));
            this->builtInObjectIds.emplace(std::get<0>(builtInComponentType), this->componentTypes.back()->getId());

            wDebug << "Created built-in component-type " << std::get<0>(builtInComponentType) << " with generated id "
                   << this->componentTypes.back()->getId();
        }
        else
        {
            this->componentTypes.push_back(std::get<1>(builtInComponentType)(this, it->second));

            wDebug << "Created built-in component-type " << std::get<0>(builtInComponentType) << " with saved id "
                   << it->second;
        }
    }

    // Just to make sure the next wobjects are created with id 1000+,
    // thus reserving the range 0-998.
    this->dummy = new WObject(this, 999);
}

void World::setName(const QString& name)
{
    if (this->name != name)
    {
        this->name = name;
        emit nameChanged();
    }
}

Banner* World::createBanner(int id)
{
    auto banner = new Banner(this, id);

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

Civilization* World::createCivilization(int id)
{
    auto civilization = new Civilization(this, id);

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

std::vector<WorldComponentType*> World::getWorldComponentTypes() const
{
    std::vector<core::WorldComponentType*> worldComponentTypes;
    for (auto* ct : this->componentTypes)
    {
        if (!ct->isBuiltIn())
            worldComponentTypes.push_back(static_cast<core::WorldComponentType*>(ct));
    }
    return worldComponentTypes;
}

QVariantList World::readComponentTypes() const
{
    return utils::toQVariantList(this->componentTypes);
}

WorldComponentType* World::createWorldComponentType(int id)
{
    auto componentType = new WorldComponentType(this, id);

    this->componentTypes.push_back(componentType);

    wDebug << "Created componentType " << componentType << " in world " << this;

    emit componentTypesChanged();

    return componentType;
}

void World::setRulesEntryPoint(const QString& rulesEntryPoint)
{
    if (this->rulesEntryPoint != rulesEntryPoint)
    {
        this->rulesEntryPoint = rulesEntryPoint;
        emit rulesEntryPointChanged();
    }
}

void World::setRulesType(const WorldRules::Type rulesType)
{
    if (this->rulesType != rulesType)
    {
        this->rulesType = rulesType;
        emit rulesTypeChanged();
    }
}

void World::loadRules(const QString& basePath)
{
    this->rules = createWorldRules(basePath, this);
}

} // namespace core
} // namespace warmonger
