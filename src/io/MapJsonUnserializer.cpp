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

#include "core/Hexagon.h"
#include "core/Map.h"
#include "io/JsonUnserializer.hpp"

namespace warmonger {
namespace io {

static std::unique_ptr<core::Component> unserializeValueFromJson(
    const QJsonValue& jvalue, QObject* parent, typeTag<std::unique_ptr<core::Component>>);
static core::FieldValue unserializeValueFromJson(const QJsonValue& jvalue, QObject* parent, typeTag<core::FieldValue>);
static std::tuple<std::unique_ptr<core::MapNode>, std::map<core::Direction, QString>> mapNodeFromJson(
    const QJsonObject& jobj, QObject* parent);
static std::vector<std::unique_ptr<core::MapNode>> unserializeValueFromJson(
    const QJsonValue& jvalue, QObject* parent, typeTag<std::vector<std::unique_ptr<core::MapNode>>>);
static core::MapNodeNeighbours unserializeValueFromJson(
    const QJsonValue& jvalue, QObject* parent, typeTag<core::MapNodeNeighbours>);

std::unique_ptr<core::Map> MapJsonUnserializer::unserializeMap(const QByteArray& data, core::World* world) const
{
    QJsonObject jobj = parseJson(data).object();

    if (world->getUuid() != jobj["world"].toString())
        throw utils::ValueError("Failed to unserialize map, world uuid mismatch");

    auto map = std::make_unique<core::Map>();
    map->setWorld(world);

    try
    {
        auto description = core::Map::describe(Visitor<core::Map>());
        unserializeMembersFromJson(jobj, *map, description.getMembers().asTuple());
    }
    catch (utils::ValueError& e)
    {
        throw utils::ValueError(e, "Failed to unserialize map");
    }

    return map;
}

core::Entity* MapJsonUnserializer::unserializeEntity(const QByteArray& data, core::Map* map) const
{
    try
    {
        return map->addEntity(unserializeFromJson<core::Entity>(parseJson(data).object(), map));
    }
    catch (utils::ValueError& e)
    {
        throw utils::ValueError(e, "Failed to unserialize entity");
    }
}

core::Faction* MapJsonUnserializer::unserializeFaction(const QByteArray& data, core::Map* map) const
{
    try
    {
        return map->addFaction(unserializeFromJson<core::Faction>(parseJson(data).object(), map));
    }
    catch (utils::ValueError& e)
    {
        throw utils::ValueError(e, "Failed to unserialize faction");
    }
}

core::MapNode* MapJsonUnserializer::unserializeMapNode(const QByteArray& data, core::Map* map) const
{
    try
    {
        return map->addMapNode(unserializeFromJson<core::MapNode>(parseJson(data).object(), map));
    }
    catch (utils::ValueError& e)
    {
        throw utils::ValueError(e, "Failed to unserialize faction");
    }
}

static std::unique_ptr<core::Component> unserializeValueFromJson(
    const QJsonValue& jvalue, QObject* parent, typeTag<std::unique_ptr<core::Component>>)
{
    if (!jvalue.isObject())
        throw utils::ValueError("Expected object type but value is not object");

    const auto jobj = jvalue.toObject();
    auto type = unserializeKeyFromJson<core::ComponentType*>(jobj, "type", parent);
    auto id = unserializeKeyFromJson<core::ObjectId>(jobj, "id", parent);

    auto component = type->createComponent(parent, id);

    auto description = core::Component::describe(Visitor<core::Component>());
    unserializeMembersFromJson(jobj, *component, description.getMembers().asTuple());

    return component;
}

static core::FieldValue unserializeValueFromJson(const QJsonValue& jvalue, QObject* parent, typeTag<core::FieldValue>)
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
            if (static_cast<double>(jvalue.toInt()) == jvalue.toDouble())
                value = jvalue.toInt();
            else
                value = jvalue.toDouble();
        }
        break;

        case QJsonValue::String:
        {
            const auto str = jvalue.toString();
            if (io::isReference(str))
                value = unserializeReference(str, parent);
            else
                value = str;
        }
        break;

        case QJsonValue::Array:
        {
            auto& list = value.makeList();

            for (auto jelement : jvalue.toArray())
            {
                list.emplace_back(unserializeValueFromJson(jelement, parent, typeTag<core::FieldValue>{}));
            }
        }
        break;

        case QJsonValue::Object:
        {
            auto& mapVal = value.makeMap();

            auto jobj = jvalue.toObject();
            for (auto it = jobj.begin(); it != jobj.end(); ++it)
            {
                mapVal.emplace(it.key(), unserializeValueFromJson(it.value(), parent, typeTag<core::FieldValue>{}));
            }
        }
        break;
    }

    return value;
}

static std::tuple<std::unique_ptr<core::MapNode>, std::map<core::Direction, QString>> mapNodeFromJson(
    const QJsonObject& jobj, QObject* parent)
{
    auto id = unserializeKeyFromJson<core::ObjectId>(jobj, "id", parent);

    if (id == core::ObjectId::Invalid)
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

    return std::make_tuple(std::make_unique<core::MapNode>(parent, id), neighbours);
}

static std::vector<std::unique_ptr<core::MapNode>> unserializeValueFromJson(
    const QJsonValue& jvalue, QObject* parent, typeTag<std::vector<std::unique_ptr<core::MapNode>>>)
{
    if (!jvalue.isArray())
        throw utils::ValueError(QString("Expected vector but value is not array"));

    std::vector<std::unique_ptr<core::MapNode>> mapNodes;
    std::vector<std::tuple<core::MapNode*, core::Direction, QString>> neighbours;

    const auto jarr = jvalue.toArray();
    for (QJsonValue jelement : jarr)
    {
        auto mapNodeData = mapNodeFromJson(jelement.toObject(), parent);
        mapNodes.emplace_back(std::move(std::get<0>(mapNodeData)));

        // for now just store the references to the neighbours
        // they will be resolved after all mapnodes have been processed
        for (auto& neighbour : std::get<1>(mapNodeData))
        {
            neighbours.emplace_back(mapNodes.back().get(), neighbour.first, neighbour.second);
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
            mn->setNeighbour(d, unserializeReferenceAs<core::MapNode>(neighbourReference, parent));
        }
    }

    return mapNodes;
}

static core::MapNodeNeighbours unserializeValueFromJson(
    const QJsonValue& jvalue, QObject* parent, typeTag<core::MapNodeNeighbours>)
{
    core::MapNodeNeighbours neighbours;
    const auto neighboursMap = unserializeValueFromJson(jvalue, parent, typeTag<std::map<QString, core::MapNode*>>{});

    for (const auto& neighbour : neighboursMap)
    {
        neighbours[core::str2direction(neighbour.first)] = neighbour.second;
    }

    return neighbours;
}

} // namespace warmonger
} // namespace io
