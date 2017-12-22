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
#include "io/Reference.h"
#include "test/Util.h"
#include <catch.hpp>

using namespace warmonger;

TEST_CASE("Banner can be serialized to JSON", "[WorldJsonSerializer][JSON][Serialize][HappyPath]")
{
    auto worlds = makeWorld();
    const QJsonObject jworld = std::get<QJsonObject>(worlds);
    auto world = std::get<std::unique_ptr<core::World>>(worlds).get();

    const io::WorldJsonSerializer serializer;

    auto banner = world->getBanners()[0];
    auto jbanner = jworld["banners"].toArray()[0].toObject();

    const QByteArray json(serializer.serializeBanner(banner));
    const QJsonDocument jdoc(QJsonDocument::fromJson(json));
    const QJsonObject jobj(jdoc.object());

    INFO("The actual serialized banner json is : " << jdoc.toJson().data());
    INFO("The expected banner json is : " << QJsonDocument(jbanner).toJson().data());

    REQUIRE(jobj == jbanner);
}

TEST_CASE("Civilization can be serialized to JSON", "[WorldJsonSerializer][JSON][Serialize][HappyPath]")
{
    auto worlds = makeWorld();
    const QJsonObject jworld = std::get<QJsonObject>(worlds);
    auto world = std::get<std::unique_ptr<core::World>>(worlds).get();

    const io::WorldJsonSerializer serializer;

    auto civilization = world->getCivilizations()[0];
    const auto jcivilization = jworld["civilizations"].toArray()[0].toObject();

    QByteArray json(serializer.serializeCivilization(civilization));
    const QJsonDocument jdoc(QJsonDocument::fromJson(json));
    const QJsonObject jobj(jdoc.object());

    INFO("The actual civilization json is : " << jdoc.toJson().data());
    INFO("The expected civilization json is : " << QJsonDocument(jcivilization).toJson().data());

    REQUIRE(jobj == jcivilization);
}

TEST_CASE("ComponentType can be serialized to JSON", "[WorldJsonSerializer][JSON][Serialize][HappyPath]")
{
    auto worlds = makeWorld();
    const QJsonObject jworld = std::get<QJsonObject>(worlds);
    auto world = std::get<std::unique_ptr<core::World>>(worlds).get();

    const io::WorldJsonSerializer serializer;

    const auto worldComponentTypes{world->getWorldComponentTypes()};
    auto componentType = worldComponentTypes[1];
    const auto jcomponentType = jworld["componentTypes"].toArray()[1].toObject();

    QByteArray json(serializer.serializeWorldComponentType(componentType));
    const QJsonDocument jdoc(QJsonDocument::fromJson(json));
    const QJsonObject jobj(jdoc.object());

    INFO("The actual component-type json is : " << jdoc.toJson().data());
    INFO("The expected component-type json is : " << QJsonDocument(jcomponentType).toJson().data());

    REQUIRE(jobj == jcomponentType);
}

TEST_CASE("World can be serialized to JSON", "[WorldJsonSerializer][JSON][Serialize][HappyPath]")
{
    auto worlds = makeWorld();
    const QJsonObject jworld = std::get<QJsonObject>(worlds);
    auto world = std::get<std::unique_ptr<core::World>>(worlds).get();

    const io::WorldJsonSerializer serializer;

    QByteArray json(serializer.serializeWorld(world));
    const QJsonDocument jdoc(QJsonDocument::fromJson(json));
    const QJsonObject jobj(jdoc.object());

    INFO("The actual world json is : " << jdoc.toJson().data());
    INFO("The expected world json is : " << QJsonDocument(jworld).toJson().data());

    REQUIRE(jobj == jworld);
}
