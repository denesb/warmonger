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

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "core/CampaignMap.h"
#include "core/Hexagon.h"
#include "core/World.h"
#include "io/JsonUnserializer.h"
#include "utils/Exception.h"

namespace warmonger {
namespace io {

QJsonDocument parseJson(const QByteArray& json);

std::unique_ptr<core::Banner> bannerFromJson(const QJsonObject& jobj, Context& ctx);
std::unique_ptr<core::Civilization> civilizationsFromJson(const QJsonObject& jobj, Context& ctx);
std::unique_ptr<core::Faction> factionFromJson(const QJsonObject& jobj, Context& ctx);
std::unique_ptr<core::MapNode> mapNodeFromJson(const QJsonObject& jobj, Context& ctx);
std::vector<std::unique_ptr<core::MapNode>> mapNodesFromJson(const QJsonArray& jarr, Context& ctx);

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

/**
 * Convert QJsonArray to Container.
 *
 * Container must be an STL-compatible container having a push_back
 * method. The convertFunc function converts QJsonValue to
 * Container::value_type.
 */
template <typename Container, typename ConvertFunc>
Container fromQJsonArray(const QJsonArray& array, ConvertFunc convertFunc)
{
    Container container;

    std::transform(array.begin(), array.end(), std::back_inserter(container), convertFunc);

    return container;
}

/**
 * Convert QJsonObject to Container.
 *
 * Container must be a mapping type supporting assigment via the []
 * operator. For the conversion a `convertKey` and a `convertValue`
 * function must be supplied, where `convertKey` converts from QString
 * to Container::key_type and `convertValue converts from QJsonValue to
 * Container::mapped_type.
 */
template <typename Container, typename ConvertKeyFunc, typename ConvertValueFunc>
Container fromQJsonObject(const QJsonObject& object, ConvertKeyFunc convertKey, ConvertValueFunc convertValue)
{
    Container container;

    for (auto it = object.begin(); it != object.end(); it++)
    {
        typename Container::key_type key = convertKey(it.key());
        typename Container::mapped_type value = convertValue(it.value());

        container[key] = value;
    }

    return container;
}

/**
 * QJsonValue to int.
 */
inline int qJsonValueToInt(const QJsonValue& value)
{
    return value.toInt();
}

template <class T>
std::vector<std::unique_ptr<T>> objectListFromJson(
    const QJsonArray& array, Context& ctx, std::function<std::unique_ptr<T>(const QJsonObject&)> fromJsonFunc)
{
    std::vector<std::unique_ptr<T>> objectsr
    {
        fromQJsonArray<std::vector<T*>>(
            array, [&](const QJsonValue& v) { return fromJsonFunc(v.toObject()).release(); });

        std::for_each(objects.begin(), objects.end(), std::bind(&Context::add, &ctx, std::placeholders::_1));

        return objects;
    }

    template <class T>
    std::vector<std::unique_ptr<T>> objectListFromJson(const QJsonArray& array,
        Context& ctx,
        std::function<std::unique_ptr<T>(const QJsonObject&, Context& ctx)> fromJsonFunc)
    {
        std::function<std::unique_ptr<T>(const QJsonObject&)> func =
            std::bind(fromJsonFunc, std::placeholders::_1, ctx);
        return objectListFromJson<T>(array, ctx, func);
    }

    template <class T>
    std::vector<T*> hierarchyNodesFromJson(
        const QJsonArray& json, std::function<std::unique_ptr<T>(const QJsonObject&, Context&)> fromJson, Context& ctx)
    {
        return fromQJsonArray<std::vector<T*>>(json, [&](const QJsonValue& v) {
            T* obj = fromJson(v.toObject(), ctx).release();
            ctx.add(obj);
            return obj;
        });
    }

    std::unique_ptr<core::Banner> JsonUnserializer::unserializeBanner(const QByteArray& data)
    {
        QJsonDocument jdoc(parseJson(data));
        return bannerFromJson(jdoc.object(), this->ctx);
    }

    std::unique_ptr<core::CampaignMap> JsonUnserializer::unserializeCampaignMap(const QByteArray& data)
    {
        QJsonDocument jdoc(parseJson(data));
        QJsonObject jobj = jdoc.object();

        std::unique_ptr<core::CampaignMap> obj(new core::CampaignMap());

        obj->setObjectName(jobj["objectName"].toString());

        obj->setDisplayName(jobj["displayName"].toString());

        obj->setWorld(resolveReference<core::World>(this->ctx, jobj["world"].toString()));

        const auto mapNodes{mapNodesFromJson(jobj["mapNodes"].toArray(), this->ctx)};

        std::for_each(mapNodes.begin(), mapNodes.end(), [&obj](auto& mapNode) { obj->addMapNode(std::move(mapNode)); });

        obj->setFactions(objectListFromJson<core::Faction>(jobj["factions"].toArray(), this->ctx, factionFromJson));

        return obj;
    }

    std::unique_ptr<core::Civilization> JsonUnserializer::unserializeCivilization(const QByteArray& data)
    {
        QJsonDocument jdoc(parseJson(data));
        return civilizationsFromJson(jdoc.object(), this->ctx);
    }

    std::unique_ptr<core::MapNode> JsonUnserializer::unserializeMapNode(const QByteArray& data)
    {
        QJsonDocument jdoc(parseJson(data));
        return mapNodeFromJson(jdoc.object(), this->ctx);
    }

    std::unique_ptr<core::Faction> JsonUnserializer::unserializeFaction(const QByteArray& data)
    {
        QJsonDocument jdoc(parseJson(data));
        return factionFromJson(jdoc.object(), this->ctx);
    }

    std::unique_ptr<core::World> JsonUnserializer::unserializeWorld(const QByteArray& data)
    {
        QJsonDocument jdoc(parseJson(data));
        QJsonObject jobj = jdoc.object();

        std::unique_ptr<core::World> obj(new core::World());

        obj->setObjectName(jobj["objectName"].toString());

        obj->setDisplayName(jobj["displayName"].toString());

        obj->setCivilizations(
            objectListFromJson<core::Civilization>(jobj["civilizations"].toArray(), this->ctx, civilizationsFromJson));

        std::vector<QColor> colors;
        for (const auto&& color : jobj["colors"].toArray())
        {
            colors.emplace_back(color.toString());
        }

        obj->setColors(colors);

        obj->setBanners(hierarchyNodesFromJson<core::Banner>(jobj["banners"].toArray(), bannerFromJson, this->ctx));

        return obj;
    }

    QJsonDocument parseJson(const QByteArray& json)
    {
        QJsonParseError parseError;

        QJsonDocument doc(QJsonDocument::fromJson(json, &parseError));

        if (parseError.error != QJsonParseError::NoError)
        {
            throw utils::ValueError(parseError.errorString() + " at " + parseError.offset);
        }

        return doc;
    }

    std::unique_ptr<core::Banner> bannerFromJson(const QJsonObject& jobj, Context& ctx)
    {
        std::unique_ptr<core::Banner> obj(new core::Banner());

        obj->setObjectName(jobj["objectName"].toString());
        obj->setDisplayName(jobj["displayName"].toString());
        if (jobj.contains("civilizations"))
            obj->setCivilizations(fromQJsonArray<std::vector<core::Civilization*>>(
                jobj["civilizations"].toArray(), ReferenceResolver<core::Civilization>(ctx)));

        return obj;
    }

    std::unique_ptr<core::Civilization> civilizationsFromJson(const QJsonObject& jobj, Context& ctx)
    {
        ReferenceResolver<core::UnitType> unitTypeRefResolver(ctx);

        std::unique_ptr<core::Civilization> obj(new core::Civilization());
        obj->setObjectName(jobj["objectName"].toString());
        obj->setDisplayName(jobj["displayName"].toString());

        return obj;
    }

    std::unique_ptr<core::Faction> factionFromJson(const QJsonObject& jobj, Context& ctx)
    {
        std::unique_ptr<core::Faction> obj(new core::Faction());
        obj->setObjectName(jobj["objectName"].toString());
        obj->setDisplayName(jobj["displayName"].toString());
        obj->setPrimaryColor(QColor(jobj["primaryColor"].toString()));
        obj->setSecondaryColor(QColor(jobj["secondaryColor"].toString()));
        obj->setBanner(resolveReference<core::Banner>(ctx, jobj["banner"].toString()));
        obj->setCivilization(resolveReference<core::Civilization>(ctx, jobj["civilization"].toString()));

        return obj;
    }

    std::unique_ptr<core::MapNode> mapNodeFromJson(const QJsonObject& jobj, Context& ctx)
    {
        std::unique_ptr<core::MapNode> obj(new core::MapNode());
        obj->setObjectName(jobj["objectName"].toString());
        obj->setDisplayName(jobj["displayName"].toString());
        obj->setTerrainType(resolveReference<core::TerrainType>(ctx, jobj["terrainType"].toString()));

        return obj;
    }

    std::vector<std::unique_ptr<core::MapNode>> mapNodesFromJson(const QJsonArray& jarr, Context& ctx);
    {
        std::vector<std::unique_ptr<core::MapNode>> mapNodes;
        std::vector<std::tuple<core::MapNode*, core::Direction, QString>> neighbours;

        for (QJsonValue jval : jarr)
        {
            QJsonObject jobj = jval.toObject();

            mapNodes.emplace_back(mapNodeFromJson(jobj, ctx));

            core::MapNode* mn = mapNode.back();
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

} // namespace io
} // namespace warmonger
