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
#include "utils/Exception.h"

namespace warmonger {
namespace io {

static std::unique_ptr<core::Banner> bannerFromJson(
    const QJsonObject& jobj, const std::vector<core::Civilization*>& allCivilizations);
static std::unique_ptr<core::Civilization> civilizationFromJson(const QJsonObject& jobj);
static std::unique_ptr<core::ComponentType> componentTypeFromJson(const QJsonObject& jobj);
static std::unique_ptr<core::EntityType> entityTypeFromJson(
    const QJsonObject& jobj, const std::vector<core::ComponentType*>& allComponentTypes);
static std::unique_ptr<core::FieldType> unserializeFieldType(const QJsonValue& jval);

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

    const QJsonArray jfields(jobj["fields"].toArray());

    if (jfields.isEmpty())
    {
        throw utils::ValueError("Failed to unserialize component-type " + name + ", it doesn't have any fields");
    }

    auto componentType{std::make_unique<core::WorldComponentType>()};
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

        auto field{std::make_unique<core::Field>()};

        field->setName(fieldName);
        field->setType(unserializeFieldType(fieldType));

        componentType->addField(std::move(field));
    }

    return componentType;
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

static std::unique_ptr<core::FieldType> unserializeFieldType(const QJsonValue& jval)
{
    const QMetaEnum typeIdMetaEnum{QMetaEnum::fromType<core::Field::TypeId>()};

    if (jval.isString())
    {
        const QString typeStr{jval.toString()};

        if(typeStr.isEmpty())
        {
            throw utils::ValueError("Failed to unserialize field-type, type has empty value");
        }

        bool ok;
        const int val{typeIdMetaEnum.keyToValue(typeStr.toLocal8Bit().data(), &ok)};

        if(!ok)
        {
            throw utils::ValueError("Failed to unserialize field-type, unknown type `" + typeStr + "' specified as type id");
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
            case core::Field::TypeId::Dictionary:
                throw utils::ValueError("Failed to unserialize field-type, `" + typeStr + "' is not a simple type");
        }
    }
    else if (jval.isObject())
    {
        const QJsonObject jcompositeType{jval.toObject()};

        const QString typeIdStr{jcompositeType["id"].toString()};

        if(typeIdStr.isEmpty())
        {
            throw utils::ValueError("Failed to unserialize field-type, type id is missing or empty");
        }

        bool ok;
        const int val{typeIdMetaEnum.keyToValue(typeIdStr.toLocal8Bit().data(), &ok)};

        if(!ok)
        {
            throw utils::ValueError("Failed to unserialize field-type, unknown type `" + typeIdStr + "' specified as type id");
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
            case core::Field::TypeId::Dictionary:
                return std::make_unique<core::FieldTypes::Dictionary>(
                    unserializeFieldType(jcompositeType["valueType"]));
        }
    }
    else
    {
        return std::unique_ptr<core::FieldType>();
    }

    return std::unique_ptr<core::FieldType>();
}

} // namespace warmonger
} // namespace io
