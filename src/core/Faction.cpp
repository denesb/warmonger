/**
 * Copyright (C) 2015-2018 Botond Dénes
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

#include "utils/Logging.h"

namespace warmonger {
namespace core {

template <typename T>
static T* lookupNamedObject(const std::vector<T*>& objects, const ir::Value& nameVal);

Faction::Faction(QObject* parent, ObjectId id)
    : WObject(parent, id)
    , primaryColor(nullptr)
    , secondaryColor(nullptr)
    , banner(nullptr)
    , civilization(nullptr)
{
}

Faction::Faction(ir::Value v, const World& world, QObject* parent)
    : WObject(parent, v.getObjectId())
{
    auto obj = std::move(v).asObject();
    this->name = std::move(obj["name"]).asString();
    this->primaryColor = lookupNamedObject(world.getColors(), obj["primaryColor"]);
    this->secondaryColor = lookupNamedObject(world.getColors(), obj["secondaryColor"]);
    this->banner = lookupNamedObject(world.getBanners(), obj["banner"]);
    this->civilization = lookupNamedObject(world.getCivilizations(), obj["civilization"]);
}

ir::Value Faction::serialize() const
{
    std::unordered_map<QString, ir::Value> obj;
    obj["id"] = this->getId().get();
    obj["name"] = this->name;

    if (this->primaryColor)
        obj["primaryColor"] = this->primaryColor->getName();
    else
        obj["primaryColor"] = {};

    if (this->secondaryColor)
        obj["secondaryColor"] = this->secondaryColor->getName();
    else
        obj["secondaryColor"] = {};

    if (this->banner)
        obj["banner"] = this->banner->getName();
    else
        obj["banner"] = {};

    if (this->civilization)
        obj["civilization"] = this->civilization->getName();
    else
        obj["civilization"] = {};

    return obj;
}

void Faction::setName(const QString& name)
{
    if (this->name != name)
    {
        this->name = name;
        emit nameChanged();
    }
}

void Faction::setPrimaryColor(Color* primaryColor)
{
    if (this->primaryColor == primaryColor)
    {
        return;
    }

    if (primaryColor == this->secondaryColor)
    {
        wWarning.format(
            "New value for primaryColor (`{}') cannot be the same as secondaryColor", primaryColor->getName());
        return;
    }

    this->primaryColor = primaryColor;
    emit primaryColorChanged();
}

void Faction::setSecondaryColor(Color* secondaryColor)
{
    if (this->secondaryColor == secondaryColor)
    {
        return;
    }

    if (secondaryColor == this->primaryColor)
    {
        wWarning.format(
            "New value for secondaryColor (`{}') cannot be the same as primaryColor", secondaryColor->getName());
        return;
    }

    this->secondaryColor = secondaryColor;
    emit secondaryColorChanged();
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

template <typename T>
static T* lookupNamedObject(const std::vector<T*>& objects, const ir::Value& nameVal)
{
    if (nameVal.getType() == ir::Type::Null)
        return nullptr;

    const auto& name = nameVal.asString();

    auto it = std::find_if(
        objects.begin(), objects.end(), [&name](const T* const object) { return object->getName() == name; });

    if (it == objects.end())
        throw utils::ValueError(fmt::format("Failed to find named-object `{}'", name));

    return *it;
}

} // namespace core
} // namespace warmonger
