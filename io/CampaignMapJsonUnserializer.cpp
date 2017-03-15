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
#include "io/Context.h"
#include "io/JsonUtils.h"
#include "utils/Exception.h"

namespace warmonger {
namespace io {

std::unique_ptr<core::Faction> factionFromJson(const QJsonObject& jobj, Context& ctx);
std::unique_ptr<core::MapNode> mapNodeFromJson(const QJsonObject& jobj);
std::vector<std::unique_ptr<core::MapNode>> mapNodesFromJson(const QJsonArray& jarr);

/**
 * Resolve reference `name` to `Type`.
 */
template <typename Type>
Type* resolveReference(Context& ctx, const QString& name)
{
    Type* obj = ctx.get<Type*>(name);
    if (obj == nullptr)
    {
        QString className(Type::staticMetaObject.className());
        throw utils::ValueError(QString("Unable to resolve reference `%1' to `%2'").arg(name).arg(className));
    }

    return obj;
}

/**
 * Functor object that retrieves object from the ctx.
 */
template <typename Type>
class ReferenceResolver
{
public:
    ReferenceResolver(Context& ctx)
        : ctx(ctx)
    {
    }

    Type* operator()(const QString& n) const
    {
        return resolveReference<Type>(this->ctx, n);
    }

    Type* operator()(const QJsonValue& v) const
    {
        return resolveReference<Type>(this->ctx, v.toString());
    }

private:
    Context& ctx;
};

std::unique_ptr<core::CampaignMap> CampaignMapJsonUnserializer::unserializeCampaignMap(
    const QByteArray& data, core::World* world) const
{
    Context ctx;
    addWorldToContext(ctx, world);

    QJsonDocument jdoc(parseJson(data));
    QJsonObject jobj = jdoc.object();

    std::unique_ptr<core::CampaignMap> obj{std::make_unique<core::CampaignMap>()};

    obj->setObjectName(jobj["objectName"].toString());

    obj->setDisplayName(jobj["displayName"].toString());

    obj->setWorld(resolveReference<core::World>(ctx, jobj["world"].toString()));

    auto mapNodes{mapNodesFromJson(jobj["mapNodes"].toArray())};

    std::for_each(mapNodes.begin(), mapNodes.end(), [&obj](auto& mapNode) { obj->addMapNode(std::move(mapNode)); });

    auto jfactions{jobj["factions"].toArray()};
    std::for_each(jfactions.begin(), jfactions.end(), [&obj, &ctx](const auto& faction) {
        obj->addFaction(factionFromJson(faction.toObject(), ctx));
    });

    return obj;
}

std::unique_ptr<core::Entity> CampaignMapJsonUnserializer::unserializeEntity(const QByteArray&, core::World*) const
{
    return std::unique_ptr<core::Entity>();
}

std::unique_ptr<core::Faction> CampaignMapJsonUnserializer::unserializeFaction(
    const QByteArray& data, core::World* world) const
{
    QJsonDocument jdoc(parseJson(data));

    Context ctx;
    addWorldToContext(ctx, world);

    return factionFromJson(jdoc.object(), ctx);
}

std::unique_ptr<core::MapNode> CampaignMapJsonUnserializer::unserializeMapNode(
    const QByteArray& data, core::World* world) const
{
    QJsonDocument jdoc(parseJson(data));

    Context ctx;
    addWorldToContext(ctx, world);

    return mapNodeFromJson(jdoc.object());
}

std::unique_ptr<core::Faction> factionFromJson(const QJsonObject& jobj, Context& ctx)
{
    std::unique_ptr<core::Faction> obj{std::make_unique<core::Faction>()};
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setPrimaryColor(QColor(jobj["primaryColor"].toString()));
    obj->setSecondaryColor(QColor(jobj["secondaryColor"].toString()));
    obj->setBanner(resolveReference<core::Banner>(ctx, jobj["banner"].toString()));
    obj->setCivilization(resolveReference<core::Civilization>(ctx, jobj["civilization"].toString()));

    return obj;
}

std::unique_ptr<core::MapNode> mapNodeFromJson(const QJsonObject& jobj)
{
    std::unique_ptr<core::MapNode> obj{std::make_unique<core::MapNode>()};
    obj->setObjectName(jobj["objectName"].toString());

    return obj;
}

std::vector<std::unique_ptr<core::MapNode>> mapNodesFromJson(const QJsonArray& jarr)
{
    Context ctx;
    std::vector<std::unique_ptr<core::MapNode>> mapNodes;
    std::vector<std::tuple<core::MapNode*, core::Direction, QString>> neighbours;

    for (QJsonValue jval : jarr)
    {
        QJsonObject jobj = jval.toObject();

        mapNodes.emplace_back(mapNodeFromJson(jobj));

        core::MapNode* mn = mapNodes.back().get();
        ctx.add(mn);

        // for now just store the references to the neighbours
        // they will be resolved after all mapnodes have been processed
        const QJsonObject jneighbours = jobj["neighbours"].toObject();
        for (auto it = jneighbours.constBegin(); it != jneighbours.constEnd(); it++)
        {
            core::Direction d = core::str2direction(it.key());
            neighbours.push_back(std::make_tuple(mn, d, it.value().toString()));
        }
    }

    core::MapNode* mn;
    core::Direction d;
    QString neighbourName;
    for (const auto& neighbour : neighbours)
    {
        std::tie(mn, d, neighbourName) = neighbour;
        if (neighbourName.isEmpty())
        {
            mn->setNeighbour(d, nullptr);
        }
        else
        {
            mn->setNeighbour(d, resolveReference<core::MapNode>(ctx, neighbourName));
        }
    }

    return mapNodes;
}

} // namespace warmonger
} // namespace io
