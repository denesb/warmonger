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

#include "io/MapJsonSerializer.h"

#include "core/Map.h"
#include "io/JsonUtils.h"
#include "io/Reference.h"
#include "utils/Exception.h"

namespace warmonger {
namespace io {

static QJsonValue fieldToJson(const core::FieldType* const type, const core::FieldValue& value);
static QJsonObject componentToJson(const core::Component* const obj);
static QJsonObject entityToJson(const core::Entity* const obj);
static QJsonObject factionToJson(const core::Faction* const obj);
static QJsonObject mapNodeToJson(const core::MapNode* const obj);

MapJsonSerializer::MapJsonSerializer(QJsonDocument::JsonFormat format)
    : format(format)
{
}

QByteArray MapJsonSerializer::serializeMap(const core::Map* const obj) const
{
    QJsonObject jobj;

    jobj["name"] = obj->getName();
    jobj["world"] = obj->getWorld()->getUuid();
    jobj["mapNodes"] = toQJsonArray(obj->getMapNodes(), mapNodeToJson);
    jobj["factions"] = toQJsonArray(obj->getFactions(), factionToJson);
    jobj["entities"] = toQJsonArray(obj->getEntities(), entityToJson);

    return QJsonDocument(jobj).toJson(this->format);
}

QByteArray MapJsonSerializer::serializeEntity(const core::Entity* const obj) const
{
    QJsonDocument jdoc(entityToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray MapJsonSerializer::serializeFaction(const core::Faction* const obj) const
{
    QJsonDocument jdoc(factionToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray MapJsonSerializer::serializeMapNode(const core::MapNode* const obj) const
{
    QJsonDocument jdoc(mapNodeToJson(obj));
    return jdoc.toJson(this->format);
}

static QJsonValue fieldToJson(const core::FieldType* const type, const core::FieldValue& value)
{
    QJsonValue jval;

    if (type->id() != value.getTypeId())
        throw utils::ValueError("Type Id mismatch for field");

    switch (type->id())
    {
        case core::Field::TypeId::Integer:
        {
            jval = value.asInteger();
        }
        break;

        case core::Field::TypeId::Real:
        {
            jval = value.asReal();
        }
        break;

        case core::Field::TypeId::String:
        {
            jval = value.asString();
        }
        break;

        case core::Field::TypeId::Reference:
        {
            jval = serializeReference(value.asReference());
        }
        break;

        case core::Field::TypeId::List:
        {
            auto valueType = static_cast<const core::FieldTypes::List*>(type)->getValueType();
            const auto& list = value.asList();
            QJsonArray jlist;

            for (const auto& element : list)
            {
                jlist.push_back(fieldToJson(valueType, element));
            }

            jval = jlist;
        }
        break;

        case core::Field::TypeId::Map:
        {
            auto valueType = static_cast<const core::FieldTypes::Map*>(type)->getValueType();
            const auto& map = value.asMap();
            QJsonObject jmap;

            for (const auto& element : map)
            {
                jmap[element.first] = fieldToJson(valueType, element.second);
            }

            jval = jmap;
        }
        break;
    }

    return jval;
}

static QJsonObject componentToJson(const core::Component* const obj)
{
    const auto& fields = obj->getType()->getFields();
    QJsonObject jobj;

    jobj["type"] = io::serializeReference(obj->getType());

    QJsonObject jfields;

    for (const auto& field : fields)
    {
        jfields[field->getName()] = fieldToJson(field->getType(), *obj->field(field->getName()));
    }

    jobj["fields"] = jfields;

    return jobj;
}

static QJsonObject entityToJson(const core::Entity* const obj)
{
    QJsonObject jobj;

    jobj["id"] = obj->getId();

    QJsonArray jcomponents;
    const auto& components = obj->getComponents();
    for (const auto& component : components)
    {
        jcomponents.push_back(componentToJson(component));
    }

    jobj["components"] = jcomponents;

    return jobj;
}

static QJsonObject factionToJson(const core::Faction* const obj)
{
    QJsonObject jobj;

    jobj["id"] = obj->getId();
    jobj["name"] = obj->getName();
    jobj["primaryColor"] = obj->getPrimaryColor().name();
    jobj["secondaryColor"] = obj->getSecondaryColor().name();
    jobj["banner"] = serializeReference(obj->getBanner());
    jobj["civilization"] = serializeReference(obj->getCivilization());

    return jobj;
}

static QJsonObject mapNodeToJson(const core::MapNode* const obj)
{
    QJsonObject jobj;

    jobj["id"] = obj->getId();

    QJsonObject jneighbours;
    for (const auto& neighbour : obj->getNeighbours())
    {
        QString neighbourName{""};
        if (neighbour.second != nullptr)
        {
            neighbourName = serializeReference(neighbour.second);
        }

        jneighbours[core::direction2str(neighbour.first)] = neighbourName;
    }

    jobj["neighbours"] = jneighbours;

    return jobj;
}

} // namespace warmonger
} // namespace io
