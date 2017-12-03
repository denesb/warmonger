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
#include "io/JsonSerializer.hpp"
#include "io/JsonUtils.h"
#include "io/Reference.h"

namespace warmonger {
namespace io {

static QJsonObject bannerToJson(const core::Banner* const obj);
static QJsonObject civilizationToJson(const core::Civilization* const obj);
static QJsonObject worldComponentTypeToJson(const core::WorldComponentType* const obj);

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

QByteArray WorldJsonSerializer::serializeWorldComponentType(const core::WorldComponentType* const obj) const
{
    QJsonDocument jdoc(worldComponentTypeToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray WorldJsonSerializer::serializeWorld(const core::World* const obj) const
{
    QJsonObject jobj;

    jobj["uuid"] = obj->getUuid();

    QJsonObject jBuildInObjectIds;
    for (const auto builtInObjectId : obj->getBuiltInObjectIds())
    {
        jBuildInObjectIds[builtInObjectId.first] = builtInObjectId.second;
    }
    jobj["builtInObjectIds"] = jBuildInObjectIds;

    jobj["name"] = obj->getName();
    jobj["rulesEntryPoint"] = obj->getRulesEntryPoint();
    jobj["rulesType"] = core::rulesTypeToString(obj->getRulesType());
    jobj["banners"] = toQJsonArray(obj->getBanners(), bannerToJson);
    jobj["civilizations"] = toQJsonArray(obj->getCivilizations(), civilizationToJson);
    jobj["colors"] = toQJsonArray(obj->getColors(), [](const QColor& c) { return c.name(); });
    jobj["componentTypes"] = toQJsonArray(obj->getWorldComponentTypes(), worldComponentTypeToJson);

    return QJsonDocument(jobj).toJson(this->format);
}

static QJsonObject bannerToJson(const core::Banner* const obj)
{
    return serializeToJson(*obj);
}

static QJsonObject civilizationToJson(const core::Civilization* const obj)
{
    return serializeToJson(*obj);
}

static QJsonObject worldComponentTypeToJson(const core::WorldComponentType* const obj)
{
    return serializeToJson(*obj);
}

} // namespace warmonger
} // namespace io
