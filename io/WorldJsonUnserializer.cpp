/**
 * \copyright (C) 2015-2017 Botond Dénes
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

#include "io/WorldJsonUnserializer.h"

#include <QJsonObject>

#include "core/EntityType.h"
#include "core/World.h"
#include "core/WorldComponentType.h"
#include "io/JsonUtils.h"
#include "utils/Exception.h"

namespace warmonger {
namespace io {

static std::unique_ptr<core::Banner> bannerFromJson(
    const QJsonObject& jobj, const std::vector<core::Civilization*>& allCivilizations);
static std::unique_ptr<core::Civilization> civilizationFromJson(const QJsonObject& jobj);
static std::unique_ptr<core::ComponentType> componentTypeFromJson(const QJsonObject& jobj);
static std::unique_ptr<core::EntityType> entityTypeFromJson(
    const QJsonObject& jobj, const std::vector<core::ComponentType*>& allComponentTypes);

std::unique_ptr<core::Banner> WorldJsonUnserializer::unserializeBanner(
    const QByteArray& data, const std::vector<core::Civilization*>& allCivilizations) const
{
    QJsonDocument jdoc(parseJson(data));
    return bannerFromJson(jdoc.object(), allCivilizations);
}

std::unique_ptr<core::Civilization> WorldJsonUnserializer::unserializeCivilization(const QByteArray& data) const
{
    QJsonDocument jdoc(parseJson(data));
    return civilizationFromJson(jdoc.object());
}

std::unique_ptr<core::ComponentType> WorldJsonUnserializer::unserializeComponentType(const QByteArray& data) const
{
    QJsonDocument jdoc(parseJson(data));
    return componentTypeFromJson(jdoc.object());
}

std::unique_ptr<core::EntityType> WorldJsonUnserializer::unserializeEntityType(
    const QByteArray& data, const std::vector<core::ComponentType*>& allComponentTypes) const
{
    QJsonDocument jdoc(parseJson(data));
    return entityTypeFromJson(jdoc.object(), allComponentTypes);
}

std::unique_ptr<core::World> WorldJsonUnserializer::unserializeWorld(const QByteArray& data) const
{
    QJsonDocument jdoc(parseJson(data));
    QJsonObject jobj = jdoc.object();

    std::unique_ptr<core::World> obj(new core::World());

    obj->setObjectName(jobj["objectName"].toString());

    obj->setDisplayName(jobj["displayName"].toString());

    const QJsonArray civilizations = jobj["civilizations"].toArray();
    std::for_each(civilizations.begin(), civilizations.end(), [&obj](const auto& val) {
        obj->addCivilization(civilizationFromJson(val.toObject()));
    });

    std::vector<QColor> colors;
    for (const auto&& color : jobj["colors"].toArray())
    {
        colors.emplace_back(color.toString());
    }

    obj->setColors(colors);

    const QJsonArray banners = jobj["banners"].toArray();
    std::for_each(banners.begin(), banners.end(), [&obj](const auto& val) {
        obj->addBanner(bannerFromJson(val.toObject(), obj->getCivilizations()));
    });

    return obj;
}

static std::unique_ptr<core::Banner> bannerFromJson(const QJsonObject& jobj, const std::vector<core::Civilization*>&)
{
    std::unique_ptr<core::Banner> obj(new core::Banner());

    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());

    /*
    if (jobj.contains("civilizations"))
        obj->setCivilizations(fromQJsonArray<std::vector<core::Civilization*>>(
            jobj["civilizations"].toArray(), ReferenceResolver<core::Civilization>(ctx)));
            */

    return obj;
}

static std::unique_ptr<core::Civilization> civilizationFromJson(const QJsonObject& jobj)
{
    auto obj = std::make_unique<core::Civilization>();

    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());

    return obj;
}

static std::unique_ptr<core::ComponentType> componentTypeFromJson(const QJsonObject& jobj)
{
    const QString& name{jobj["name"].toString()};

    if (name.isNull() || name.isEmpty())
    {
        throw utils::ValueError("Failed to unserialize component-type, it doesn't have a name property");
    }

    const QJsonArray jpropertyNames(jobj["propertyNames"].toArray());

    if (jpropertyNames.isEmpty())
    {
        throw utils::ValueError("Failed to unserialize component-type " + name + ", it doesn't have any properties");
    }

    std::vector<QString> propertyNames;

    for (const auto& jpropertyName : jpropertyNames)
    {
        const QString propertyName{jpropertyName.toString()};

        if (propertyName.isNull())
        {
            throw utils::ValueError("Failed to unserialize component-type " + name + ", it has an empty property name");
        }

        propertyNames.push_back(propertyName);
    }

    return std::make_unique<core::WorldComponentType>(name, propertyNames);
}

static std::unique_ptr<core::EntityType> entityTypeFromJson(
    const QJsonObject& jobj, const std::vector<core::ComponentType*>& allComponentTypes)
{
    const QString& name{jobj["name"].toString()};

    if (name.isNull() || name.isEmpty())
    {
        throw utils::ValueError("Failed to unserialize entity-type, it doesn't have a name property");
    }

    const QJsonArray jcomponentTypeNames(jobj["componentTypes"].toArray());

    if (jcomponentTypeNames.isEmpty())
    {
        throw utils::ValueError("Failed to unserialize entity-type " + name + ", it doesn't have any component-types");
    }

    std::vector<core::ComponentType*> componentTypes;

    for (const auto& jcomponentTypeName : jcomponentTypeNames)
    {
        const QString name{jcomponentTypeName.toString()};

        if (name.isEmpty())
        {
            throw utils::ValueError("Failed to unserialize entity-type " + name + ", it has an empty component-type");
        }

        const auto it{std::find_if(allComponentTypes.cbegin(),
            allComponentTypes.cend(),
            [&name](const auto& componentType) { return componentType->getName() == name; })};

        if (it != allComponentTypes.cend())
        {
            componentTypes.push_back(*it);
        }
        else
        {
            throw utils::ValueError(
                "Failed to unserialize entity-type " + name + ", it has a non-existent component-type " + name);
        }
    }

    return std::make_unique<core::EntityType>(name, componentTypes);
}

} // namespace warmonger
} // namespace io
