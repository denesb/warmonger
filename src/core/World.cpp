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

#include <cassert>

#include "core/BuiltInComponentTypes.h"
#include "core/Utils.h"
#include "utils/Logging.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

struct IdReserver : public WObject
{
    IdReserver(QObject* parent)
        : WObject(parent, ObjectId(999))
    {
    }
};

World::World(const QString& uuid,
    WorldRules::Type rulesType,
    const std::map<QString, ObjectId>& builtInObjectIds,
    QObject* parent)
    : QObject{parent}
    , uuid{uuid}
    , builtInObjectIds{builtInObjectIds}
    , rulesType{rulesType}
    , rules{createWorldRules(this)}
{
    const auto& builtInComponentTypes = getBuiltInComponentTypesFactories();

    for (const auto& builtInComponentType : builtInComponentTypes)
    {
        const auto it = this->builtInObjectIds.find(std::get<0>(builtInComponentType));

        if (it == this->builtInObjectIds.end())
        {
            this->componentTypes.push_back(std::get<1>(builtInComponentType)(this, ObjectId::Invalid));
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

    // Just to make sure the next WObjects are created with id 1000+,
    // thus reserving the range 0-998.
    new IdReserver(this);
}

void World::setName(const QString& name)
{
    if (this->name != name)
    {
        this->name = name;
        emit nameChanged();
    }
}

QVariantList World::readBanners() const
{
    return utils::toQVariantList(this->banners);
}

Banner* World::createBanner(ObjectId id)
{
    auto banner = new Banner(this, id);

    this->banners.push_back(banner);

    wDebug << "Created banner " << banner << " in world " << this;

    emit bannersChanged();

    return banner;
}

Banner* World::addBanner(std::unique_ptr<Banner> banner)
{
    assert(banner->parent() == this);

    this->banners.push_back(banner.release());
    wDebug << "Added banner " << banners.back() << " to world " << this;
    emit bannersChanged();

    return this->banners.back();
}

QVariantList World::readCivilizations() const
{
    return utils::toQVariantList(this->civilizations);
}

Civilization* World::createCivilization(ObjectId id)
{
    auto civilization = new Civilization(this, id);

    this->civilizations.push_back(civilization);

    wDebug << "Created civilization " << civilization << " in world " << this;

    emit civilizationsChanged();

    return civilization;
}

Civilization* World::addCivilization(std::unique_ptr<Civilization> civilization)
{
    assert(civilization->parent() == this);

    this->civilizations.push_back(civilization.release());
    wDebug << "Added civilization " << civilizations.back() << " to world " << this;
    emit civilizationsChanged();

    return this->civilizations.back();
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

WorldComponentType* World::createWorldComponentType(ObjectId id)
{
    auto componentType = new WorldComponentType(this, id);
    componentType->setWorldRules(this->rules);

    this->componentTypes.push_back(componentType);

    wDebug << "Created WorldComponentType " << componentType << " in world " << this;

    emit componentTypesChanged();

    return componentType;
}

WorldComponentType* World::addWorldComponentType(std::unique_ptr<WorldComponentType> worldComponentType)
{
    assert(worldComponentType->parent() == this);

    auto wct = worldComponentType.get();
    wct->setWorldRules(this->rules);

    this->componentTypes.push_back(worldComponentType.release());

    wDebug << "Added WorldComponentType " << componentTypes.back() << " to world " << this;

    emit componentTypesChanged();

    return wct;
}

void World::setRulesEntryPoint(const QString& rulesEntryPoint)
{
    if (this->rulesEntryPoint != rulesEntryPoint)
    {
        this->rulesEntryPoint = rulesEntryPoint;
        emit rulesEntryPointChanged();
    }
}

void World::loadRules(const QString& basePath)
{
    this->rules->loadRules(basePath, this->rulesEntryPoint);
}

} // namespace core
} // namespace warmonger
