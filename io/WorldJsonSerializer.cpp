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

#include "io/WorldJsonSerializer.h"

#include <QMetaEnum>

#include "core/World.h"
#include "io/JsonUtils.h"
#include "io/Reference.h"

namespace warmonger {
namespace io {

static QJsonObject bannerToJson(const core::Banner* const obj);
static QJsonObject civilizationToJson(const core::Civilization* const obj);
static QJsonObject componentTypeToJson(const core::ComponentType* const obj);
static QJsonObject fieldToJson(const core::Field* const obj);
static QJsonValue fieldTypeToJson(const core::FieldType* const obj);
static QJsonObject entityTypeToJson(const core::EntityType* const obj);

WorldJsonSerializer::WorldJsonSerializer(QJsonDocument::JsonFormat format)
    : format(format)
{
}

QByteArray WorldJsonSerializer::serializeBanner(const core::Banner* const obj) const
{
    QJsonDocument jdoc(bannerToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray WorldJsonSerializer::serializeCivilization(const core::Civilization* const obj) const
{
    QJsonDocument jdoc(civilizationToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray WorldJsonSerializer::serializeComponentType(const core::ComponentType* const obj) const
{
    QJsonDocument jdoc(componentTypeToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray WorldJsonSerializer::serializeEntityType(const core::EntityType* const obj) const
{
    QJsonDocument jdoc(entityTypeToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray WorldJsonSerializer::serializeWorld(const core::World* const obj) const
{
    QJsonObject jobj;

    jobj["uuid"] = obj->getUuid();
    jobj["displayName"] = obj->getDisplayName();
    jobj["banners"] = toQJsonArray(obj->getBanners(), bannerToJson);
    jobj["civilizations"] = toQJsonArray(obj->getCivilizations(), civilizationToJson);
    jobj["colors"] = toQJsonArray(obj->getColors(), [](const QColor& c) { return c.name(); });
    jobj["componentTypes"] = toQJsonArray(obj->getComponentTypes(), componentTypeToJson);
    jobj["entityTypes"] = toQJsonArray(obj->getEntityTypes(), entityTypeToJson);

    return QJsonDocument(jobj).toJson(this->format);
}

static QJsonObject bannerToJson(const core::Banner* const obj)
{
    QJsonObject jobj;

    jobj["id"] = obj->getId();
    jobj["displayName"] = obj->getDisplayName();
    jobj["civilizations"] = toQJsonArray(obj->getCivilizations(), io::serializeReference);

    return jobj;
}

static QJsonObject civilizationToJson(const core::Civilization* const obj)
{
    QJsonObject jobj;

    jobj["id"] = obj->getId();
    jobj["displayName"] = obj->getDisplayName();

    return jobj;
}

static QJsonObject componentTypeToJson(const core::ComponentType* const obj)
{
    QJsonObject jobj;

    jobj["id"] = obj->getId();
    jobj["name"] = obj->getName();

    QJsonArray jfields;
    auto fields = obj->getFields();
    std::transform(fields.cbegin(), fields.cend(), std::back_inserter(jfields), fieldToJson);

    jobj["fields"] = jfields;

    return jobj;
}

static QJsonObject fieldToJson(const core::Field* const obj)
{
    QJsonObject jobj;

    jobj["name"] = obj->getName();
    jobj["type"] = fieldTypeToJson(obj->getType());

    return jobj;
}

static QJsonValue fieldTypeToJson(const core::FieldType* const obj)
{
    QJsonValue jval;

    const QMetaEnum typeIdMetaEnum{QMetaEnum::fromType<core::Field::TypeId>()};
    const auto id = obj->id();

    switch(id)
    {
        case core::Field::TypeId::Integer:
        case core::Field::TypeId::Real:
        case core::Field::TypeId::String:
        case core::Field::TypeId::Reference:
        {
            jval = typeIdMetaEnum.valueToKey(static_cast<int>(id));
            break;
        }
        case core::Field::TypeId::List:
        {
            const auto list = static_cast<const core::FieldTypes::List*>(obj);
            QJsonObject jlistType;

            jlistType["id"] = typeIdMetaEnum.valueToKey(static_cast<int>(id));
            jlistType["valueType"] = fieldTypeToJson(list->getValueType());

            jval = jlistType;
            break;
        }
        case core::Field::TypeId::Dictionary:
        {
            const auto dict = static_cast<const core::FieldTypes::Dictionary*>(obj);
            QJsonObject jdictType;

            jdictType["id"] = typeIdMetaEnum.valueToKey(static_cast<int>(id));
            jdictType["valueType"] = fieldTypeToJson(dict->getValueType());

            jval = jdictType;
            break;
        }
    }

    return jval;
}

static QJsonObject entityTypeToJson(const core::EntityType* const obj)
{
    QJsonObject jobj;

    jobj["id"] = obj->getId();
    jobj["name"] = obj->getName();
    jobj["componentTypes"] = toQJsonArray(obj->getComponentTypes(), io::serializeReference);

    return jobj;
}

} // namespace warmonger
} // namespace io
