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
#include "io/Reference.h"
#include "test/Util.h"
#include <catch.hpp>

using namespace warmonger;

TEST_CASE("MapNode can be serialized to JSON", "[MapJsonSerializer][JSON][Serialize][HappyPath]")
{
    const auto maps = makeMap();
    const auto map = maps.first.get();
    const auto jmap = maps.second;

    const io::MapJsonSerializer serializer;

    auto mapNode = map->getMapNodes()[0];
    auto jmapNode = jmap["mapNodes"].toArray()[0].toObject();

    const QByteArray json(serializer.serializeMapNode(mapNode));
    const QJsonDocument jdoc(QJsonDocument::fromJson(json));
    const QJsonObject jobj(jdoc.object());

    INFO("The actual serialized mapNode json is : " << jdoc.toJson().data());
    INFO("The expected mapNode json is : " << QJsonDocument(jmapNode).toJson().data());

    REQUIRE(jobj == jmapNode);
}

TEST_CASE("Faction can be serialized to JSON", "[MapJsonSerializer][JSON][Serialize][HappyPath]")
{
    const auto maps = makeMap();
    const auto map = maps.first.get();
    const auto jmap = maps.second;

    const io::MapJsonSerializer serializer;

    auto faction = map->getFactions()[0];
    auto jfaction = jmap["factions"].toArray()[0].toObject();

    const QByteArray json(serializer.serializeFaction(faction));
    const QJsonDocument jdoc(QJsonDocument::fromJson(json));
    const QJsonObject jobj(jdoc.object());

    INFO("The actual serialized faction json is : " << jdoc.toJson().data());
    INFO("The expected faction json is : " << QJsonDocument(jfaction).toJson().data());

    REQUIRE(jobj == jfaction);
}

TEST_CASE("Entity can be serialized to JSON", "[MapJsonSerializer][JSON][Serialize][HappyPath]")
{
    const auto maps = makeMap();
    const auto map = maps.first.get();
    const auto jmap = maps.second;

    const io::MapJsonSerializer serializer;

    auto entity = map->getEntities()[0];
    auto jentity = jmap["entities"].toArray()[0].toObject();

    const QByteArray json(serializer.serializeEntity(entity));
    const QJsonDocument jdoc(QJsonDocument::fromJson(json));
    const QJsonObject jobj(jdoc.object());

    INFO("The actual serialized entity json is : " << jdoc.toJson().data());
    INFO("The expected entity json is : " << QJsonDocument(jentity).toJson().data());

    REQUIRE(jobj == jentity);
}

TEST_CASE("Map can be serialized to JSON", "[MapJsonSerializer][JSON][Serialize][HappyPath]")
{
    const auto maps = makeMap();
    const auto map = maps.first.get();
    const auto jmap = maps.second;

    const io::MapJsonSerializer serializer;

    const QByteArray json(serializer.serializeMap(map));
    const QJsonDocument jdoc(QJsonDocument::fromJson(json));
    const QJsonObject jobj(jdoc.object());

    INFO("The actual serialized map json is : " << jdoc.toJson().data());
    INFO("The expected map json is : " << QJsonDocument(jmap).toJson().data());

    REQUIRE(jobj == jmap);
}
