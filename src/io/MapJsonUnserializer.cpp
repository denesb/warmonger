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

#include "io/MapJsonUnserializer.h"

#include <algorithm>
#include <memory>
#include <vector>

#include "core/Hexagon.h"
#include "core/Map.h"
#include "core/World.h"
#include "io/JsonUtils.h"
#include "io/Reference.h"
#include "utils/Exception.h"

namespace warmonger {
namespace io {

static core::Faction* factionFromJson(const QJsonObject& jobj, core::Map* map);
static core::Entity* entityFromJson(const QJsonObject& jobj, core::Map* map);
static void componentFromJson(const QJsonObject& jcomponent, core::Entity* entity, core::Map* map);
static core::FieldValue fieldFromJson(const QJsonValue& jvalue, core::Map* map, core::Field::Type fieldType);
static core::FieldValue fieldFromJson(const QJsonValue& jvalue, core::Map* map);
static std::tuple<core::MapNode*, std::map<core::Direction, QString>> mapNodeFromJson(
    const QJsonObject& jobj, core::Map* map);
static void mapNodesFromJson(const QJsonArray& jarr, core::Map* map);

std::unique_ptr<core::Map> MapJsonUnserializer::unserializeMap(const QByteArray& data, core::World* world) const
{
    QJsonDocument jdoc(parseJson(data));
    QJsonObject jobj = jdoc.object();

    if (world->getUuid() != jobj["world"].toString())
        throw utils::ValueError("Failed to unserialize map, world uuid mismatch");

    std::unique_ptr<core::Map> obj{std::make_unique<core::Map>()};

    const QString name{jobj["name"].toString()};
    if (name.isNull() || name.isEmpty())
        throw utils::ValueError("Failed to unserialize map, it has missing or invalid name");

    obj->setName(name);

    obj->setWorld(world);

    const auto jmapNodes = jobj["mapNodes"].toArray();
    if (jmapNodes.isEmpty())
        throw utils::ValueError("Failed to unserialize map, it has missing or invalid mapNodes");

    mapNodesFromJson(jmapNodes, obj.get());

    auto jfactions = jobj["factions"].toArray();
    if (jfactions.isEmpty())
        throw utils::ValueError("Failed to unserialize map, it has missing or invalid factions");

    std::for_each(jfactions.begin(), jfactions.end(), [&obj](const auto& faction) {
        factionFromJson(faction.toObject(), obj.get());
    });

    auto jentities = jobj["entities"].toArray();
    if (jentities.isEmpty())
        throw utils::ValueError("Failed to unserialize map, it has missing or invalid entities");

    std::for_each(jentities.begin(), jentities.end(), [&obj](const auto& entity) {
        entityFromJson(entity.toObject(), obj.get());
    });

    return obj;
}

core::Entity* MapJsonUnserializer::unserializeEntity(const QByteArray& data, core::Map* map) const
{
    QJsonDocument jdoc(parseJson(data));
    return entityFromJson(jdoc.object(), map);
}

core::Faction* MapJsonUnserializer::unserializeFaction(const QByteArray& data, core::Map* map) const
{
    QJsonDocument jdoc(parseJson(data));
    return factionFromJson(jdoc.object(), map);
}

core::MapNode* MapJsonUnserializer::unserializeMapNode(const QByteArray& data, core::Map* map) const
{
    QJsonDocument jdoc(parseJson(data));
    QJsonObject jobj{jdoc.object()};

    const auto mapNodeData = mapNodeFromJson(jobj, map);
    auto mapNode = std::get<core::MapNode*>(mapNodeData);

    for (auto& neighbour : std::get<1>(mapNodeData))
    {
        core::Direction direction{neighbour.first};
        QString ref{neighbour.second};

        if (!ref.isEmpty())
        {
            mapNode->setNeighbour(direction, unserializeReferenceAs<core::MapNode>(ref, map));
        }
        else
        {
            mapNode->setNeighbour(direction, nullptr);
        }
    }

    return mapNode;
}

static core::Faction* factionFromJson(const QJsonObject& jobj, core::Map* map)
{
    const auto id = jobj["id"].toInt(core::WObject::invalidId);

    if (id == core::WObject::invalidId)
        throw utils::ValueError("Failed to unserialize faction, it has missing or invalid id");

    const auto name = jobj["name"].toString();

    if (name.isNull() || name.isEmpty())
        throw utils::ValueError("Failed to unserialize faction, it has missing or invalid name");

    const auto primaryColor = QColor(jobj["primaryColor"].toString());

    if (!primaryColor.isValid())
        throw utils::ValueError("Failed to unserialize faction, it has missing or invalid primaryColor");

    const auto secondaryColor = QColor(jobj["secondaryColor"].toString());

    if (!secondaryColor.isValid())
        throw utils::ValueError("Failed to unserialize faction, it has missing or invalid secondaryColor");

    auto banner = unserializeReferenceAs<core::Banner>(jobj["banner"].toString(), map);

    if (banner == nullptr)
        throw utils::ValueError("Failed to unserialize faction, it has missing or invalid banner");

    auto civilization = unserializeReferenceAs<core::Civilization>(jobj["civilization"].toString(), map);

    if (civilization == nullptr)
        throw utils::ValueError("Failed to unserialize faction, it has missing or invalid civilization");

    auto obj = map->createFaction(id);

    obj->setName(name);
    obj->setPrimaryColor(primaryColor);
    obj->setSecondaryColor(secondaryColor);
    obj->setBanner(banner);
    obj->setCivilization(civilization);

    return obj;
}

static core::Entity* entityFromJson(const QJsonObject& jobj, core::Map* map)
{
    const auto id = jobj["id"].toInt(core::WObject::invalidId);

    if (id == core::WObject::invalidId)
        throw utils::ValueError("Failed to unserialize entity, it has missing or invalid id");

    auto obj = map->createEntity(id);

    if (!jobj["components"].isArray())
        throw utils::ValueError("Failed to unserialize entity, it has missing or invalid components");

    const auto jcomponents = jobj["components"].toArray();
    for (const auto& jcomponent : jcomponents)
    {
        if (!jcomponent.isObject())
            throw utils::ValueError("Failed to unserialize entity, component is not an object");

        componentFromJson(jcomponent.toObject(), obj, map);
    }

    return obj;
}

static void componentFromJson(const QJsonObject& jcomponent, core::Entity* entity, core::Map* map)
{
    const QString componentTypeName{jcomponent["type"].toString()};
    if (componentTypeName.isNull() || componentTypeName.isEmpty())
        throw utils::ValueError("Failed to unserialize component, invalid or missing type");

    const auto componentType{io::unserializeReferenceAs<core::ComponentType>(componentTypeName, map)};
    auto component{entity->createComponent(componentType)};

    if (!jcomponent["fields"].isObject())
        throw utils::ValueError("Failed to unserialize component, fields is invalid or missing");

    const auto jfields{jcomponent["fields"].toObject()};

    const auto& fields{component->getType()->getFields()};
    for (const auto field : fields)
    {
        const auto jvalue = jfields[field->getName()];

        if (jvalue.isUndefined())
            throw utils::ValueError("Failed to unserialize component, field " + field->getName() + " is missing");

        *component->field(field->getName()) = fieldFromJson(jvalue, map, field->getType());
    }
}

static core::FieldValue fieldFromJson(const QJsonValue& jvalue, core::Map* map, core::Field::Type fieldType)
{
    core::FieldValue value;

    switch (fieldType)
    {
        case core::Field::Type::Integer:
        {
            if (!jvalue.isDouble() || (static_cast<double>(jvalue.toInt()) != jvalue.toDouble()))
                throw utils::ValueError("Failed to unserialize integer field, value is not an integer");

            value = jvalue.toInt();
        }
        break;

        case core::Field::Type::Real:
        {
            if (!jvalue.isDouble())
                throw utils::ValueError("Failed to unserialize real field, value is not a real");

            value = jvalue.toDouble();
        }
        break;

        case core::Field::Type::String:
        {
            if (!jvalue.isString())
                throw utils::ValueError("Failed to unserialize string field, value is not a string");

            value = jvalue.toString();
        }
        break;

        case core::Field::Type::Reference:
        {
            if (!jvalue.isString())
                throw utils::ValueError("Failed to unserialize reference field, value is not a string");

            value = unserializeReference(jvalue.toString(), map);
        }
        break;

        case core::Field::Type::List:
        {
            if (!jvalue.isArray())
                throw utils::ValueError("Failed to unserialize list field, value is not an array");

            auto& list = value.makeList();

            for (auto jelement : jvalue.toArray())
            {
                list.emplace_back(fieldFromJson(jelement, map));
            }
        }
        break;

        case core::Field::Type::Map:
        {
            if (!jvalue.isObject())
                throw utils::ValueError("Failed to unserialize dictionary field, value is not an object");

            auto& mapVal = value.makeMap();

            auto jobj = jvalue.toObject();
            for (auto it = jobj.begin(); it != jobj.end(); ++it)
            {
                mapVal.emplace(it.key(), fieldFromJson(it.value(), map));
            }
        }
        break;
    }

    return value;
}

static core::FieldValue fieldFromJson(const QJsonValue& jvalue, core::Map* map)
{
    core::FieldValue value;

    switch (jvalue.type())
    {
        case QJsonValue::Undefined:
        case QJsonValue::Null:
            throw utils::ValueError("Null json value");

        case QJsonValue::Bool:
            throw utils::ValueError("Boolean json values are not supported");

        case QJsonValue::Double:
        {
            value = jvalue.toDouble();
        }
        break;

        case QJsonValue::String:
        {
            auto str = jvalue.toString();
            if (isReference(str))
                value = unserializeReference(str, map);
            else
                value = str;
        }
        break;

        case QJsonValue::Array:
        {
            auto& list = value.makeList();

            for (auto jelement : jvalue.toArray())
            {
                list.emplace_back(fieldFromJson(jelement, map));
            }
        }
        break;

        case QJsonValue::Object:
        {
            auto& mapVal = value.makeMap();

            auto jobj = jvalue.toObject();
            for (auto it = jobj.begin(); it != jobj.end(); ++it)
            {
                mapVal.emplace(it.key(), fieldFromJson(it.value(), map));
            }
        }
        break;
    }

    return value;
}

static std::tuple<core::MapNode*, std::map<core::Direction, QString>> mapNodeFromJson(
    const QJsonObject& jobj, core::Map* map)
{
    const int id = jobj["id"].toInt(core::WObject::invalidId);

    if (id == core::WObject::invalidId)
        throw utils::ValueError("Failed to unserialize mapNode, it has missing or invalid id");

    const auto jneighbours = jobj["neighbours"].toObject();

    if (jneighbours.isEmpty())
        throw utils::ValueError("Failed to unserialize mapNode, it has missing or invalid neighbours");

    std::map<core::Direction, QString> neighbours;

    for (auto it = jneighbours.constBegin(); it != jneighbours.constEnd(); it++)
    {
        core::Direction direction{core::str2direction(it.key())};
        QString ref{it.value().toString()};

        neighbours.emplace(direction, ref);
    }

    return std::make_tuple(map->createMapNode(id), neighbours);
}

static void mapNodesFromJson(const QJsonArray& jarr, core::Map* map)
{
    std::vector<std::tuple<core::MapNode*, core::Direction, QString>> neighbours;

    for (QJsonValue jval : jarr)
    {
        const auto mapNodeData = mapNodeFromJson(jval.toObject(), map);

        // for now just store the references to the neighbours
        // they will be resolved after all mapnodes have been processed
        for (auto& neighbour : std::get<1>(mapNodeData))
        {
            neighbours.emplace_back(std::get<core::MapNode*>(mapNodeData), neighbour.first, neighbour.second);
        }
    }

    core::MapNode* mn;
    core::Direction d;
    QString neighbourReference;
    for (const auto& neighbour : neighbours)
    {
        std::tie(mn, d, neighbourReference) = neighbour;
        if (neighbourReference.isEmpty())
        {
            mn->setNeighbour(d, nullptr);
        }
        else
        {
            mn->setNeighbour(d, unserializeReferenceAs<core::MapNode>(neighbourReference, map));
        }
    }
}

} // namespace warmonger
} // namespace io
