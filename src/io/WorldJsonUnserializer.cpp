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

#include "core/EntityType.h"
#include "core/World.h"
#include "core/WorldComponentType.h"
#include "io/JsonUtils.h"
#include "io/Reference.h"
#include "utils/Exception.h"

namespace warmonger {
namespace io {

static core::Banner* bannerFromJson(const QJsonObject& jobj, core::World* world);
static core::Civilization* civilizationFromJson(const QJsonObject& jobj, core::World* world);
static core::ComponentType* componentTypeFromJson(const QJsonObject& jobj, core::World* world);
static core::EntityType* entityTypeFromJson(const QJsonObject& jobj, core::World* world);
static std::unique_ptr<core::FieldType> unserializeFieldType(const QJsonValue& jval);
static std::unique_ptr<core::FieldType> unserializeSimpleFieldType(const QJsonValue& jval);
static std::unique_ptr<core::FieldType> unserializeComplexFieldType(const QJsonValue& jval);

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

core::ComponentType* WorldJsonUnserializer::unserializeComponentType(const QByteArray& data, core::World* world) const
{
    QJsonDocument jdoc(parseJson(data));
    return componentTypeFromJson(jdoc.object(), world);
}

core::EntityType* WorldJsonUnserializer::unserializeEntityType(const QByteArray& data, core::World* world) const
{
    QJsonDocument jdoc(parseJson(data));
    return entityTypeFromJson(jdoc.object(), world);
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

    const QJsonArray componentTypes = jobj["componentTypes"].toArray();
    if (componentTypes.isEmpty())
        throw utils::ValueError("Failed to unserialize world, missing, invalid or empty component-types");

    std::for_each(componentTypes.begin(), componentTypes.end(), [&obj](const auto& val) {
        componentTypeFromJson(val.toObject(), obj.get());
    });

    const QJsonArray entityTypes = jobj["entityTypes"].toArray();
    if (entityTypes.isEmpty())
        throw utils::ValueError("Failed to unserialize world, missing, invalid or empty entity-types");

    std::for_each(entityTypes.begin(), entityTypes.end(), [&obj](const auto& val) {
        entityTypeFromJson(val.toObject(), obj.get());
    });

    return obj;
}

static core::Banner* bannerFromJson(const QJsonObject& jobj, core::World* world)
{
    const int id = jobj["id"].toInt(core::WObject::invalidId);

    if (id == core::WObject::invalidId)
        throw utils::ValueError("Failed to unserialize banner, it has missing or invalid id");

    const QString name = jobj["name"].toString();

    if (name.isNull() || name.isEmpty())
        throw utils::ValueError("Failed to unserialize banner, it has missing or invalid name");

    std::vector<core::Civilization*> civilizations;
    if (jobj.contains("civilizations"))
    {
        if (!jobj["civilizations"].isArray())
            throw utils::ValueError("Failed to unserialize banner " + name + ", civilizations is not an array");

        auto jcivilizations = jobj["civilizations"].toArray();

        for (auto jcivilization : jcivilizations)
            civilizations.emplace_back(io::unserializeReferenceAs<core::Civilization>(jcivilization.toString(), world));
    }

    auto obj = world->createBanner(id);
    obj->setName(name);
    obj->setCivilizations(civilizations);

    return obj;
}

static core::Civilization* civilizationFromJson(const QJsonObject& jobj, core::World* world)
{
    const int id = jobj["id"].toInt(-1);

    if (id == core::WObject::invalidId)
        throw utils::ValueError("Failed to unserialize civilization, it has missing or invalid id");

    const QString name = jobj["name"].toString();

    if (name.isNull() || name.isEmpty())
        throw utils::ValueError("Failed to unserialize banner, it has missing or empty name");

    auto obj = world->createCivilization(id);
    obj->setName(name);

    return obj;
}

static core::ComponentType* componentTypeFromJson(const QJsonObject& jobj, core::World* world)
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
        field->setType(unserializeFieldType(fieldType));
    }

    return componentType;
}

static core::EntityType* entityTypeFromJson(const QJsonObject& jobj, core::World* world)
{
    const int id = jobj["id"].toInt(core::WObject::invalidId);

    if (id == core::WObject::invalidId)
        throw utils::ValueError("Failed to unserialize entity-type, it has missing or invalid id");

    const QString& name{jobj["name"].toString()};

    if (name.isNull() || name.isEmpty())
    {
        throw utils::ValueError("Failed to unserialize entity-type, it doesn't have a name property");
    }

    auto entityType = world->createEntityType(id);
    entityType->setName(name);

    const QJsonArray jcomponentTypeRefs(jobj["componentTypes"].toArray());

    if (jcomponentTypeRefs.isEmpty())
    {
        throw utils::ValueError("Failed to unserialize entity-type " + name + ", it doesn't have any component-types");
    }

    for (const auto& jcomponentTypeRef : jcomponentTypeRefs)
    {
        const QString ref{jcomponentTypeRef.toString()};

        if (ref.isEmpty())
        {
            throw utils::ValueError("Failed to unserialize entity-type " + ref + ", it has an empty component-type");
        }

        auto componentType = unserializeReferenceAs<core::ComponentType>(ref, world);

        if (componentType)
        {
            entityType->addComponentType(componentType);
        }
        else
        {
            throw utils::ValueError(
                "Failed to unserialize entity-type " + name + ", it has a non-existent component-type " + ref);
        }
    }

    return entityType;
}

static std::unique_ptr<core::FieldType> unserializeFieldType(const QJsonValue& jval)
{
    if (jval.isString())
    {
        return unserializeSimpleFieldType(jval);
    }
    else if (jval.isObject())
    {
        return unserializeComplexFieldType(jval);
    }

    throw utils::ValueError("Failed to unserialize field-type, type has invalid value");
}

static std::unique_ptr<core::FieldType> unserializeSimpleFieldType(const QJsonValue& jval)
{
    const QMetaEnum typeIdMetaEnum{QMetaEnum::fromType<core::Field::TypeId>()};
    const QString typeStr{jval.toString()};

    if (typeStr.isEmpty())
    {
        throw utils::ValueError("Failed to unserialize field-type, type has empty value");
    }

    bool ok;
    const int val{typeIdMetaEnum.keyToValue(typeStr.toLocal8Bit().data(), &ok)};

    if (!ok)
    {
        throw utils::ValueError(
            "Failed to unserialize field-type, unknown type `" + typeStr + "' specified as type id");
    }

    switch (static_cast<core::Field::TypeId>(val))
    {
        case core::Field::TypeId::Integer:
            return std::make_unique<core::FieldTypes::Integer>();
        case core::Field::TypeId::Real:
            return std::make_unique<core::FieldTypes::Real>();
        case core::Field::TypeId::String:
            return std::make_unique<core::FieldTypes::String>();
        case core::Field::TypeId::Reference:
            return std::make_unique<core::FieldTypes::Reference>();
        case core::Field::TypeId::List:
        case core::Field::TypeId::Map:
            throw utils::ValueError("Failed to unserialize field-type, `" + typeStr + "' is not a simple type");
    }

    throw utils::ValueError("Failed to unserialize field-type, type has invalid value");
}

static std::unique_ptr<core::FieldType> unserializeComplexFieldType(const QJsonValue& jval)
{
    const QMetaEnum typeIdMetaEnum{QMetaEnum::fromType<core::Field::TypeId>()};
    const QJsonObject jcompositeType{jval.toObject()};

    const QString typeIdStr{jcompositeType["id"].toString()};

    if (typeIdStr.isEmpty())
    {
        throw utils::ValueError("Failed to unserialize field-type, type id is missing or empty");
    }

    bool ok;
    const int val{typeIdMetaEnum.keyToValue(typeIdStr.toLocal8Bit().data(), &ok)};

    if (!ok)
    {
        throw utils::ValueError(
            "Failed to unserialize field-type, unknown type `" + typeIdStr + "' specified as type id");
    }

    switch (static_cast<core::Field::TypeId>(val))
    {
        case core::Field::TypeId::Integer:
        case core::Field::TypeId::Real:
        case core::Field::TypeId::String:
        case core::Field::TypeId::Reference:
            throw utils::ValueError("Failed to unserialize field-type, `" + typeIdStr + "' is not a complex type");
        case core::Field::TypeId::List:
            return std::make_unique<core::FieldTypes::List>(unserializeFieldType(jcompositeType["valueType"]));
        case core::Field::TypeId::Map:
            return std::make_unique<core::FieldTypes::Map>(unserializeFieldType(jcompositeType["valueType"]));
    }

    throw utils::ValueError("Failed to unserialize field-type, type has invalid value");
}

} // namespace warmonger
} // namespace io
