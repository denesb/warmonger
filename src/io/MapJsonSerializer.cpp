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
#include "io/JsonSerializer.hpp"
#include "io/JsonUtils.h"
#include "io/Reference.h"
#include "utils/Exception.h"

namespace warmonger {
namespace core {

static QJsonValue serializeValueToJson(const core::FieldValue& value, const QObject& obj);
static QJsonValue serializeValueToJson(const core::MapNodeNeighbours& value, const QObject& obj);

} // namespace core

namespace io {

static QJsonObject entityToJson(const core::Entity* const obj);
static QJsonObject factionToJson(const core::Faction* const obj);
static QJsonObject mapNodeToJson(const core::MapNode* const obj);

MapJsonSerializer::MapJsonSerializer(QJsonDocument::JsonFormat format)
    : format(format)
{
}

QByteArray MapJsonSerializer::serializeMap(const core::Map* const obj) const
{
    return QJsonDocument(serializeToJson(*obj)).toJson(this->format);
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

static QJsonObject entityToJson(const core::Entity* const obj)
{
    return serializeToJson(*obj);
}

static QJsonObject factionToJson(const core::Faction* const obj)
{
    return serializeToJson(*obj);
}

static QJsonObject mapNodeToJson(const core::MapNode* const obj)
{
    return serializeToJson(*obj);
}

} // namespace io

namespace core {

static QJsonValue serializeValueToJson(const core::FieldValue& value, const QObject& obj)
{
    QJsonValue jval;

    switch (value.getType())
    {
        case core::Field::Type::Integer:
        {
            jval = value.asInteger();
        }
        break;

        case core::Field::Type::Real:
        {
            jval = value.asReal();
        }
        break;

        case core::Field::Type::String:
        {
            jval = value.asString();
        }
        break;

        case core::Field::Type::Reference:
        {
            jval = io::serializeReference(value.asReference());
        }
        break;

        case core::Field::Type::List:
        {
            const auto& list = value.asList();
            QJsonArray jlist;

            for (const auto& element : list)
            {
                jlist.push_back(serializeValueToJson(element, obj));
            }

            jval = jlist;
        }
        break;

        case core::Field::Type::Map:
        {
            const auto& map = value.asMap();
            QJsonObject jmap;

            for (const auto& element : map)
            {
                jmap[element.first] = serializeValueToJson(element.second, obj);
            }

            jval = jmap;
        }
        break;
    }

    return jval;
}

static QJsonValue serializeValueToJson(const core::MapNodeNeighbours& value, const QObject&)
{
    QJsonObject jneighbours;
    for (const auto& neighbour : value)
    {
        QString neighbourName{""};
        if (neighbour.second != nullptr)
        {
            neighbourName = io::serializeReference(neighbour.second);
        }

        jneighbours[core::direction2str(neighbour.first)] = neighbourName;
    }
    return jneighbours;
}

} // namespace core
} // namespace warmonger
