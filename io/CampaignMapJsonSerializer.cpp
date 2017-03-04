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

#include "io/CampaignMapJsonSerializer.h"

#include "core/CampaignMap.h"
#include "io/JsonUtils.h"

namespace warmonger {
namespace io {

static QJsonObject campaignMapToJson(const core::CampaignMap* const obj);
static QJsonObject componentToJson(const core::Component* const obj);
static QJsonObject entityToJson(const core::Entity* const obj);
static QJsonObject factionToJson(const core::Faction* const obj);
static QJsonObject mapNodeToJson(const core::MapNode* const obj);

CampaignMapJsonSerializer::CampaignMapJsonSerializer(QJsonDocument::JsonFormat format)
    : format(format)
{
}

QByteArray CampaignMapJsonSerializer::serializeCampaignMap(const core::CampaignMap* const obj) const
{
    QJsonDocument jdoc(campaignMapToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray CampaignMapJsonSerializer::serializeComponent(const core::Component* const obj) const
{
    QJsonDocument jdoc(componentToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray CampaignMapJsonSerializer::serializeEntity(const core::Entity* const obj) const
{
    QJsonDocument jdoc(entityToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray CampaignMapJsonSerializer::serializeFaction(const core::Faction* const obj) const
{
    QJsonDocument jdoc(factionToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray CampaignMapJsonSerializer::serializeMapNode(const core::MapNode* const obj) const
{
    QJsonDocument jdoc(mapNodeToJson(obj));
    return jdoc.toJson(this->format);
}

static QJsonObject campaignMapToJson(const core::CampaignMap* const obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["world"] = obj->getWorld()->objectName();
    jobj["mapNodes"] = toQJsonArray(obj->getMapNodes(), mapNodeToJson);
    jobj["factions"] = toQJsonArray(obj->getFactions(), factionToJson);

    return jobj;
}

static QJsonObject componentToJson(const core::Component* const)
{
    return QJsonObject();
}

static QJsonObject entityToJson(const core::Entity* const)
{
    return QJsonObject();
}

static QJsonObject factionToJson(const core::Faction* const obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["primaryColor"] = obj->getPrimaryColor().name();
    jobj["secondaryColor"] = obj->getSecondaryColor().name();
    jobj["banner"] = obj->getBanner()->objectName();
    jobj["civilization"] = obj->getCivilization()->objectName();

    return jobj;
}

static QJsonObject mapNodeToJson(const core::MapNode* const obj)
{
    QJsonObject jobj;

    jobj["objectName"] = obj->objectName();

    QJsonObject jneighbours;
    for (const auto& neighbour : obj->getNeighbours())
    {
        QString neighbourName{""};
        if (neighbour.second != nullptr)
        {
            neighbourName = neighbour.second->objectName();
        }

        jneighbours[core::direction2str(neighbour.first)] = neighbourName;
    }

    jobj["neighbours"] = jneighbours;

    return jobj;
}

} // namespace warmonger
} // namespace io
