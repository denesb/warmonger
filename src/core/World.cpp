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

#include "core/World.h"

#include "utils/Format.h"
#include "utils/Logging.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

World::World(const QString& uuid, WorldRules::Type rulesType, QObject* parent)
    : QObject{parent}
    , uuid{uuid}
    , rulesType{rulesType}
    , rules{createWorldRules(this)}
{
}

World::World(ir::Value v, QObject* parent)
    : QObject{parent}
{
    auto map = std::move(v).asMap();
    this->uuid = std::move(map["uuid"]).asString();
    this->name = std::move(map["name"]).asString();
    this->rulesEntryPoint = std::move(map["rulesEntryPoint"]).asString();
    this->rulesType = map["rulesType"].asEnum<WorldRules::Type>();

    this->setObjectName(this->name);

    this->rules = createWorldRules(this);
}

ir::Value World::serialize() const
{
    std::unordered_map<QString, ir::Value> map;

    map["uuid"] = this->uuid;
    map["name"] = this->name;
    map["rulesEntryPoint"] = this->rulesEntryPoint;
    map["rulesType"] = this->rulesType;

    return map;
}

void World::setName(const QString& name)
{
    if (this->name != name)
    {
        this->name = name;
        emit nameChanged();
    }
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

QVariantList World::readBanners() const
{
    return utils::toQVariantList(this->banners);
}

Banner* World::createBanner(QString name)
{
    auto* banner = new Banner(std::move(name), this);

    this->banners.push_back(banner);

    wDebug.format("Created banner {} in world {}", *banner, *this);

    emit bannersChanged();

    return banner;
}

QVariantList World::readCivilizations() const
{
    return utils::toQVariantList(this->civilizations);
}

Civilization* World::createCivilization(QString name)
{
    auto* civilization = new Civilization(std::move(name), this);

    this->civilizations.push_back(civilization);

    wDebug.format("Created civilization {} in world {} ", *civilization, *this);

    emit civilizationsChanged();

    return civilization;
}

Color* World::createColor(QString name)
{
    auto* color = new Color(std::move(name), this);

    this->colors.push_back(color);

    wDebug.format("Created color {} in world {} ", *color, *this);

    emit colorsChanged();

    return color;
}

QVariantList World::readColors() const
{
    return utils::toQVariantList(this->colors);
}

} // namespace core
} // namespace warmonger
