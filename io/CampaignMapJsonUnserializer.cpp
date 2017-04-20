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

#include "io/CampaignMapJsonUnserializer.h"

#include <algorithm>
#include <memory>
#include <vector>

#include "core/CampaignMap.h"
#include "core/Hexagon.h"
#include "core/World.h"
#include "io/JsonUtils.h"
#include "io/Reference.h"
#include "utils/Exception.h"

namespace warmonger {
namespace io {

core::Faction* factionFromJson(const QJsonObject& jobj, core::CampaignMap* map);
core::MapNode* mapNodeFromJson(const QJsonObject& jobj, core::CampaignMap* map);
void mapNodesFromJson(const QJsonArray& jarr, core::CampaignMap* map);

std::unique_ptr<core::CampaignMap> CampaignMapJsonUnserializer::unserializeCampaignMap(
    const QByteArray& data, core::World* world) const
{
    QJsonDocument jdoc(parseJson(data));
    QJsonObject jobj = jdoc.object();

    std::unique_ptr<core::CampaignMap> obj{std::make_unique<core::CampaignMap>()};

    obj->setDisplayName(jobj["displayName"].toString());

    obj->setWorld(world);

    mapNodesFromJson(jobj["mapNodes"].toArray(), obj.get());

    auto jfactions{jobj["factions"].toArray()};
    std::for_each(jfactions.begin(), jfactions.end(), [&obj](const auto& faction) {
        factionFromJson(faction.toObject(), obj.get());
    });

    return obj;
}

core::Entity* CampaignMapJsonUnserializer::unserializeEntity(const QByteArray&, core::CampaignMap*) const
{
    return nullptr;
}

core::Faction* CampaignMapJsonUnserializer::unserializeFaction(
    const QByteArray& data, core::CampaignMap* map) const
{
    QJsonDocument jdoc(parseJson(data));
    return factionFromJson(jdoc.object(), map);
}

core::MapNode* CampaignMapJsonUnserializer::unserializeMapNode(
    const QByteArray& data, core::CampaignMap* map) const
{
    QJsonDocument jdoc(parseJson(data));

    return mapNodeFromJson(jdoc.object(), map);
}

core::Faction* factionFromJson(const QJsonObject& jobj, core::CampaignMap* map)
{
    const auto id = unserializeIdFrom(jobj);
    const auto name = jobj["displayName"].toString();
    const auto primaryColor = QColor(jobj["primaryColor"].toString());
    const auto secondaryColor = QColor(jobj["secondaryColor"].toString());
    auto banner = unserializeReferenceAs<core::Banner>(jobj["banner"].toString(), map);
    auto civilization = unserializeReferenceAs<core::Civilization>(jobj["civilization"].toString(), map);

    auto obj = map->createFaction(id);

    obj->setDisplayName(name);
    obj->setPrimaryColor(primaryColor);
    obj->setSecondaryColor(secondaryColor);
    obj->setBanner(banner);
    obj->setCivilization(civilization);

    return obj;
}

core::MapNode* mapNodeFromJson(const QJsonObject& jobj, core::CampaignMap* map)
{
    return map->createMapNode(unserializeIdFrom(jobj));
}

void mapNodesFromJson(const QJsonArray& jarr, core::CampaignMap* map)
{
    std::vector<std::tuple<core::MapNode*, core::Direction, QString>> neighbours;

    for (QJsonValue jval : jarr)
    {
        QJsonObject jobj = jval.toObject();

        auto mapNode = mapNodeFromJson(jobj, map);

        // for now just store the references to the neighbours
        // they will be resolved after all mapnodes have been processed
        const QJsonObject jneighbours = jobj["neighbours"].toObject();
        for (auto it = jneighbours.constBegin(); it != jneighbours.constEnd(); it++)
        {
            core::Direction d = core::str2direction(it.key());
            neighbours.push_back(std::make_tuple(mapNode, d, it.value().toString()));
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
