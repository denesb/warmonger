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

#include "core/World.h"
#include "io/JsonUtils.h"

namespace warmonger {
namespace io {

static QJsonObject bannerToJson(const core::Banner* const obj);
static QJsonObject civilizationToJson(const core::Civilization* const obj);
static QJsonObject componentTypeToJson(const core::ComponentType* const obj);
static QJsonObject entityTypeToJson(const core::EntityType* const obj);
static QJsonObject worldToJson(const core::World* const obj);

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
    QJsonDocument jdoc(worldToJson(obj));
    return jdoc.toJson(this->format);
}

static QJsonObject bannerToJson(const core::Banner* const obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["civilizations"] = toQJsonArray(obj->getCivilizations(), qObjectName);

    return jobj;
}

static QJsonObject civilizationToJson(const core::Civilization* const obj)
{
    return namesToJson(obj);
}

static QJsonObject componentTypeToJson(const core::ComponentType* const)
{
    return QJsonObject();
}

static QJsonObject entityTypeToJson(const core::EntityType* const)
{
    return QJsonObject();
}

QJsonObject worldToJson(const core::World* obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["banners"] = toQJsonArray(obj->getBanners(), bannerToJson);
    jobj["civilizations"] = toQJsonArray(obj->getCivilizations(), civilizationToJson);
    jobj["colors"] = toQJsonArray(obj->getColors(), [](const QColor& c) { return c.name(); });

    return jobj;
}

} // namespace warmonger
} // namespace io
