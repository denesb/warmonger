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
#include <QMetaEnum>

#include "core/World.h"
#include "core/WorldComponentType.h"
#include "io/JsonUnserializer.hpp"
#include "io/JsonUtils.h"
#include "io/Reference.h"
#include "utils/Exception.h"

namespace warmonger {
namespace io {

static core::Banner* bannerFromJson(const QJsonObject& jobj, core::World* world);
static core::Civilization* civilizationFromJson(const QJsonObject& jobj, core::World* world);
static core::WorldComponentType* worldComponentTypeFromJson(const QJsonObject& jobj, core::World* world);

core::Banner* WorldJsonUnserializer::unserializeBanner(const QByteArray& data, core::World* world) const
{
    QJsonDocument jdoc(parseJson(data));
    return bannerFromJson(jdoc.object(), world);
}

core::Civilization* WorldJsonUnserializer::unserializeCivilization(const QByteArray& data, core::World* world) const
{
    QJsonDocument jdoc(parseJson(data));
    return civilizationFromJson(jdoc.object(), world);
}

core::WorldComponentType* WorldJsonUnserializer::unserializeWorldComponentType(
    const QByteArray& data, core::World* world) const
{
    QJsonDocument jdoc(parseJson(data));
    return worldComponentTypeFromJson(jdoc.object(), world);
}

std::unique_ptr<core::World> WorldJsonUnserializer::unserializeWorld(const QByteArray& data) const
{
    QJsonDocument jdoc(parseJson(data));
    QJsonObject jobj = jdoc.object();

    const QString uuid = jobj["uuid"].toString();
    if (uuid.isNull() || uuid.isEmpty())
        throw utils::ValueError("Failed to unserialize world, missing or invalid uuid");

    std::map<QString, int> builtInObjectIds;
    const QJsonObject jBuiltInObjectIds{jobj["builtInObjectIds"].toObject()};

    if (jBuiltInObjectIds.isEmpty())
        throw utils::ValueError("Failed to unserialize world, missing or invalid built-in object-ids");

    for (auto it = jBuiltInObjectIds.begin(); it != jBuiltInObjectIds.end(); ++it)
    {
        const int id{it.value().toInt(core::WObject::invalidId)};

        if (id == core::WObject::invalidId)
            throw utils::ValueError("Failed to unserialize world, invalid built-in object-id");

        builtInObjectIds.emplace(it.key(), id);
    }

    auto obj{std::make_unique<core::World>(uuid, builtInObjectIds)};

    const QString name = jobj["name"].toString();
    if (name.isNull() || name.isEmpty())
        throw utils::ValueError("Failed to unserialize world, missing or invalid name");

    obj->setName(name);

    const QString rulesEntryPoint{jobj["rulesEntryPoint"].toString()};
    if (rulesEntryPoint.isNull() || rulesEntryPoint.isEmpty())
        throw utils::ValueError("Failed to unserialize world, missing or invalid rules entry point");

    obj->setRulesEntryPoint(rulesEntryPoint);

    const QString rulesType{jobj["rulesType"].toString()};
    if (rulesType.isNull() || rulesType.isEmpty())
        throw utils::ValueError("Failed to unserialize world, missing or invalid rules type");

    obj->setRulesType(core::rulesTypeFromString(rulesType));

    const QJsonArray civilizations = jobj["civilizations"].toArray();
    if (civilizations.isEmpty())
        throw utils::ValueError("Failed to unserialize world, missing, invalid or empty civilizations");

    std::for_each(civilizations.begin(), civilizations.end(), [&obj](const auto& val) {
        civilizationFromJson(val.toObject(), obj.get());
    });

    const QJsonArray jcolors = jobj["colors"].toArray();
    if (jcolors.isEmpty())
        throw utils::ValueError("Failed to unserialize world, missing, invalid or empty colors");

    std::vector<QColor> colors;
    for (const auto&& color : jcolors)
    {
        colors.emplace_back(color.toString());
    }

    obj->setColors(colors);

    const QJsonArray banners = jobj["banners"].toArray();
    if (banners.isEmpty())
        throw utils::ValueError("Failed to unserialize world, missing, invalid or empty banners");

    std::for_each(
        banners.begin(), banners.end(), [&obj](const auto& val) { bannerFromJson(val.toObject(), obj.get()); });

    const QJsonArray worldComponentTypes = jobj["componentTypes"].toArray();
    if (worldComponentTypes.isEmpty())
        throw utils::ValueError("Failed to unserialize world, missing, invalid or empty component-types");

    std::for_each(worldComponentTypes.begin(), worldComponentTypes.end(), [&obj](const auto& val) {
        worldComponentTypeFromJson(val.toObject(), obj.get());
    });

    return obj;
}

static core::Banner* bannerFromJson(const QJsonObject& jobj, core::World* world)
{
    try
    {
        return world->addBanner(unserializeFromJson<core::Banner>(jobj, world));
    }
    catch (...)
    {
        std::throw_with_nested(utils::ValueError("Failed to unserialize banner"));
    }
}

static core::Civilization* civilizationFromJson(const QJsonObject& jobj, core::World* world)
{
    try
    {
        return world->addCivilization(unserializeFromJson<core::Civilization>(jobj, world));
    }
    catch (...)
    {
        std::throw_with_nested(utils::ValueError("Failed to unserialize civilization"));
    }
}

static core::WorldComponentType* worldComponentTypeFromJson(const QJsonObject& jobj, core::World* world)
{
    const int id = jobj["id"].toInt(core::WObject::invalidId);

    if (id == core::WObject::invalidId)
        throw utils::ValueError("Failed to unserialize component-type, it has missing or invalid id");

    const QString& name{jobj["name"].toString()};

    if (name.isNull() || name.isEmpty())
        throw utils::ValueError("Failed to unserialize component-type, it doesn't have a name property");

    const QJsonArray jfields(jobj["fields"].toArray());

    if (jfields.isEmpty())
        throw utils::ValueError("Failed to unserialize component-type " + name + ", it doesn't have any fields");

    auto componentType = world->createWorldComponentType(id);
    componentType->setName(name);

    for (const auto& jfieldValue : jfields)
    {
        const QJsonObject jfield{jfieldValue.toObject()};
        const QString fieldName{jfield["name"].toString()};

        if (fieldName.isNull() || fieldName.isEmpty())
        {
            throw utils::ValueError("Failed to unserialize component-type " + name + ", it has a field without name");
        }

        const QJsonValue fieldType{jfield["type"]};

        if (fieldType.isUndefined() || (!fieldType.isString() && !fieldType.isObject()))
        {
            throw utils::ValueError("Failed to unserialize component-type " + name + ", the field " + fieldName +
                " has invalid or missing");
        }

        auto field = componentType->createField();

        field->setName(fieldName);

        {
            const QMetaEnum typeIdMetaEnum{QMetaEnum::fromType<core::Field::Type>()};
            bool ok{true};
            auto e = typeIdMetaEnum.keyToValue(jfield["type"].toString().toLocal8Bit().data(), &ok);

            if (!ok)
                throw utils::ValueError("Failed to unserialize field-type, type has invalid value");

            field->setType(static_cast<core::Field::Type>(e));
        }
    }

    return componentType;
}

} // namespace warmonger
} // namespace io
