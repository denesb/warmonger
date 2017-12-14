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

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include "core/BuiltInComponentTypes.h"
#include "core/WorldComponentType.h"
#include "io/JsonUtils.h"
#include "io/WorldJsonUnserializer.h"
#include "test/Util.h"
#include "utils/Exception.h"
#include <catch.hpp>

using namespace warmonger;

TEST_CASE("Banner can be unserialized from JSON", "[WorldJsonUnserializer][JSON][Unserialize][HappyPath]")
{
    std::unique_ptr<core::World> world;
    QJsonObject jworld;
    std::tie(world, jworld) = makeWorld();

    const QJsonObject jobj = jworld["banners"].toArray()[0].toObject();

    const io::WorldJsonUnserializer unserializer;
    const QJsonDocument jdoc{jobj};
    const QByteArray rawJson{jdoc.toJson()};

    INFO("The json banner is: " << rawJson.data());

    SECTION("unserializing Banner doesn't throw")
    {
        REQUIRE_NOTHROW(unserializer.unserializeBanner(rawJson, world.get()));
        REQUIRE(unserializer.unserializeBanner(rawJson, world.get()));
    }

    SECTION("unserializing Banner")
    {
        auto banner = unserializer.unserializeBanner(rawJson, world.get());

        REQUIRE(banner->getId() == jobj["id"].toInt());
        REQUIRE(banner->getName() == jobj["name"].toString());
        if (jobj.contains("civilizations"))
        {
            REQUIRE_REFERENCES(jobj["civilizations"].toArray(), banner->getCivilizations());
        }
        else
        {
            REQUIRE(banner->getCivilizations().empty());
        }
    }
}

TEST_CASE("Banner can't be unserialized from JSON", "[WorldJsonUnserializer][JSON][Unserialize][ErrorPaths]")
{
    std::unique_ptr<core::World> worldPtr;
    QJsonObject jworld;
    std::tie(worldPtr, jworld) = makeWorld();
    auto world = worldPtr.get();

    const io::WorldJsonUnserializer unserializer;
    QJsonObject jobj = jworld["banners"].toArray()[0].toObject();

    SECTION("Invalid JSON")
    {
        QString invalidJson{"{\"name\": \"name1\",}"};
        REQUIRE_THROWS_AS(unserializer.unserializeBanner(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Missing id")
    {
        jobj.remove("id");
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeBanner(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Id not a number")
    {
        jobj["id"] = "asd";
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeBanner(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Missing name")
    {
        jobj.remove("name");
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeBanner(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Name not string")
    {
        jobj["name"] = 123;
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeBanner(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Civilizations is not an array")
    {
        jobj["civilizations"] = "asd";
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeBanner(jdoc.toJson(), world), utils::ValueError);
    }
}

TEST_CASE("Civilization can be unserialized from JSON", "[WorldJsonUnserializer][JSON][Unserialize][HappyPath]")
{
    std::unique_ptr<core::World> world;
    QJsonObject jworld;
    std::tie(world, jworld) = makeWorld();

    const QJsonObject jobj = jworld["civilizations"].toArray()[0].toObject();

    const QJsonDocument jdoc{jobj};
    const io::WorldJsonUnserializer unserializer;
    const QByteArray rawJson{jdoc.toJson()};

    INFO("The json civilization is: " << rawJson.data());

    SECTION("unserializing Civilization doesn't throw")
    {
        REQUIRE_NOTHROW(unserializer.unserializeCivilization(rawJson, world.get()));
        REQUIRE(unserializer.unserializeCivilization(rawJson, world.get()));
    }

    SECTION("unserializing Civilization")
    {
        auto civilization = unserializer.unserializeCivilization(rawJson, world.get());

        REQUIRE(civilization->getId() == jobj["id"].toInt());
        REQUIRE(civilization->getName() == jobj["name"].toString());
    }
}

TEST_CASE("Civilization can't be unserialized from JSON", "[WorldJsonUnserializer][JSON][Unserialize][ErrorPaths]")
{
    std::unique_ptr<core::World> worldPtr;
    QJsonObject jworld;
    std::tie(worldPtr, jworld) = makeWorld();
    auto world = worldPtr.get();

    QJsonObject jobj = jworld["civilizations"].toArray()[0].toObject();
    const io::WorldJsonUnserializer unserializer;

    SECTION("Invalid JSON")
    {
        QString invalidJson{"{\"name\": \"name1\",}"};

        REQUIRE_THROWS_AS(unserializer.unserializeCivilization(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Missing id")
    {
        jobj.remove("id");
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeCivilization(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Id not a number")
    {
        jobj["id"] = "asd";
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeCivilization(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Missing name")
    {
        jobj.remove("name");
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeCivilization(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Name not string")
    {
        jobj["name"] = 123;
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeCivilization(jdoc.toJson(), world), utils::ValueError);
    }
}

TEST_CASE("ComponentType unserialized from JSON - happy path", "[WorldJsonUnserializer][JSON][Unserialize][HappyPath]")
{
    auto worlds = makeWorld();
    core::World* world = worlds.first.get();
    QJsonObject jworld = worlds.second;

    const io::WorldJsonUnserializer unserializer;

    const QJsonObject jobj = jworld["componentTypes"].toArray()[1].toObject();
    const QJsonDocument jdoc{jobj};
    const QByteArray rawJson{jdoc.toJson()};

    INFO("The json component-type is: " << rawJson.data());

    SECTION("Unserialization succeeds without exceptions")
    {
        REQUIRE_NOTHROW(unserializer.unserializeWorldComponentType(rawJson, world));
        REQUIRE(unserializer.unserializeWorldComponentType(rawJson, world));
    }

    SECTION("Unserialization component-type")
    {
        const auto componentType{unserializer.unserializeWorldComponentType(rawJson, world)};

        const QJsonDocument jdoc{io::parseJson(rawJson)};
        const QJsonObject jobj{jdoc.object()};
        const QJsonArray jfields(jobj["fields"].toArray());

        REQUIRE(componentType->getId() == jobj["id"].toInt());
        REQUIRE(componentType->getName() == jobj["name"].toString());
        REQUIRE(componentType->getFields().size() == jfields.size());

        const auto& fields{componentType->getFields()};

        for (int i = 0; i < jfields.size(); ++i)
        {
            REQUIRE(fields[i]->getName() == jfields[i].toObject()["name"].toString());
        }

        REQUIRE(fields[0]->getType() == core::Field::Type::Integer);
        REQUIRE(fields[1]->getType() == core::Field::Type::Real);
        REQUIRE(fields[2]->getType() == core::Field::Type::String);
        REQUIRE(fields[3]->getType() == core::Field::Type::Reference);
        REQUIRE(fields[4]->getType() == core::Field::Type::List);
        REQUIRE(fields[5]->getType() == core::Field::Type::Map);
    }
}

TEST_CASE("ComponentType can't be unserialized from JSON", "[WorldJsonUnserializer][JSON][Unserialize][ErrorPaths]")
{
    std::unique_ptr<core::World> worldPtr;
    QJsonObject jworld;
    std::tie(worldPtr, jworld) = makeWorld();
    auto world = worldPtr.get();

    const io::WorldJsonUnserializer unserializer;
    QJsonObject jobj = jworld["componentTypes"].toArray()[0].toObject();

    SECTION("Invalid Json")
    {
        QString invalidJson{"{\"name\": \"name1\",}"};

        REQUIRE_THROWS_AS(
            unserializer.unserializeWorldComponentType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Missing id")
    {
        jobj.remove("id");

        REQUIRE_THROWS_AS(
            unserializer.unserializeWorldComponentType(QJsonDocument(jobj).toJson(), world), utils::ValueError);
    }

    SECTION("Id not int")
    {
        jobj["id"] = "asd";

        REQUIRE_THROWS_AS(
            unserializer.unserializeWorldComponentType(QJsonDocument(jobj).toJson(), world), utils::ValueError);
    }

    SECTION("Missing name")
    {
        jobj.remove("name");

        REQUIRE_THROWS_AS(
            unserializer.unserializeWorldComponentType(QJsonDocument(jobj).toJson(), world), utils::ValueError);
    }

    SECTION("Empty name")
    {
        jobj["name"] = "";

        REQUIRE_THROWS_AS(
            unserializer.unserializeWorldComponentType(QJsonDocument(jobj).toJson(), world), utils::ValueError);
    }

    SECTION("Name not string")
    {
        jobj["name"] = 123;

        REQUIRE_THROWS_AS(
            unserializer.unserializeWorldComponentType(QJsonDocument(jobj).toJson(), world), utils::ValueError);
    }

    SECTION("No fields")
    {
        jobj.remove("fields");

        REQUIRE_THROWS_AS(
            unserializer.unserializeWorldComponentType(QJsonDocument(jobj).toJson(), world), utils::ValueError);
    }

    SECTION("Fields list is empty")
    {
        jobj["fields"] = QJsonArray();

        REQUIRE_THROWS_AS(
            unserializer.unserializeWorldComponentType(QJsonDocument(jobj).toJson(), world), utils::ValueError);
    }

    SECTION("Field has empty name")
    {
        jobj["fields"] = QJsonArray{
            QJsonObject{{"name", ""}, {"type", "Integer"}},
        };

        REQUIRE_THROWS_AS(
            unserializer.unserializeWorldComponentType(QJsonDocument(jobj).toJson(), world), utils::ValueError);
    }

    SECTION("Field has non-string name")
    {
        jobj["fields"] = QJsonArray{
            QJsonObject{{"name", 123}, {"type", "Integer"}},
        };

        REQUIRE_THROWS_AS(
            unserializer.unserializeWorldComponentType(QJsonDocument(jobj).toJson(), world), utils::ValueError);
    }

    SECTION("Field has no type")
    {
        jobj["fields"] = QJsonArray{
            QJsonObject{{"name", "intField"}},
        };

        REQUIRE_THROWS_AS(
            unserializer.unserializeWorldComponentType(QJsonDocument(jobj).toJson(), world), utils::ValueError);
    }

    SECTION("Field has non-string type")
    {
        jobj["fields"] = QJsonArray{
            QJsonObject{{"name", "intField"}, {"type", 123}},
        };

        REQUIRE_THROWS_AS(
            unserializer.unserializeWorldComponentType(QJsonDocument(jobj).toJson(), world), utils::ValueError);
    }

    SECTION("Field has empty string type")
    {
        jobj["fields"] = QJsonArray{
            QJsonObject{{"name", "intField"}, {"type", ""}},
        };

        REQUIRE_THROWS_AS(
            unserializer.unserializeWorldComponentType(QJsonDocument(jobj).toJson(), world), utils::ValueError);
    }

    SECTION("Field has unknown type")
    {
        jobj["fields"] = QJsonArray{
            QJsonObject{{"name", "intField"}, {"type", "Unknown"}},
        };

        REQUIRE_THROWS_AS(
            unserializer.unserializeWorldComponentType(QJsonDocument(jobj).toJson(), world), utils::ValueError);
    }
}

TEST_CASE("World can be unserialized from JSON", "[WorldJsonUnserializer][JSON][Unserialize][HappyPath]")
{
    const auto worlds = makeWorld();
    const QJsonObject jobj{worlds.second};

    const QJsonDocument jdoc{jobj};
    const QByteArray rawJson{jdoc.toJson()};

    INFO("The json world is: " << rawJson.data());

    const io::WorldJsonUnserializer unserializer;

    SECTION("Unserialization succeeds without exceptions")
    {
        REQUIRE_NOTHROW(unserializer.unserializeWorld(rawJson));
        REQUIRE(unserializer.unserializeWorld(rawJson));
    }

    SECTION("Unserializing World")
    {
        const std::unique_ptr<core::World> world(unserializer.unserializeWorld(rawJson));

        REQUIRE(world->getUuid() == jobj["uuid"].toString());
        REQUIRE(world->getName() == jobj["name"].toString());
        REQUIRE(world->getRulesEntryPoint() == jobj["rulesEntryPoint"].toString());
        REQUIRE(rulesTypeToString(world->getRulesType()) == jobj["rulesType"].toString());
        REQUIRE(world->getBanners().size() == jobj["banners"].toArray().size());
        REQUIRE(world->getCivilizations().size() == jobj["civilizations"].toArray().size());
        REQUIRE(world->getColors().size() == jobj["colors"].toArray().size());

        const auto& componentTypes{world->getComponentTypes()};
        const auto worldComponentTypes = std::count_if(
            componentTypes.cbegin(), componentTypes.cend(), [](const auto& ct) { return !ct->isBuiltIn(); });

        REQUIRE(worldComponentTypes == jobj["componentTypes"].toArray().size());

        const std::vector<QColor>& colors = world->getColors();
        QJsonArray jcolors = jobj["colors"].toArray();
        for (std::size_t i = 0; i < colors.size(); ++i)
        {
            REQUIRE(colors[i].name() == jcolors[i].toString());
        }

        const auto& builtInObjectIds{world->getBuiltInObjectIds()};
        const QJsonObject jBuiltInObjectIds{jobj["builtInObjectIds"].toObject()};

        for (auto it = jBuiltInObjectIds.begin(); it != jBuiltInObjectIds.end(); ++it)
        {
            REQUIRE(builtInObjectIds.at(it.key()) == it.value().toInt());
        }
    }
}

TEST_CASE("World can't be unserialized from JSON", "[WorldJsonUnserializer][JSON][Unserialize][ErrorPaths]")
{
    const auto worlds = makeWorld();
    QJsonObject jobj{worlds.second};

    const io::WorldJsonUnserializer unserializer;

    SECTION("Invalid JSON")
    {
        QString invalidJson{"{\"name\": \"name1\",}"};

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(invalidJson.toLocal8Bit()), utils::ValueError);
    }

    SECTION("Missing uuid")
    {
        jobj.remove("uuid");

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }

    SECTION("Uuid not string")
    {
        jobj["uuid"] = 123;

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }

    SECTION("Missing name")
    {
        jobj.remove("name");

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }

    SECTION("Name not string")
    {
        jobj["name"] = 123;

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }

    SECTION("Missing civilizations")
    {
        jobj.remove("civilizations");

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }

    SECTION("Civilizations not an array")
    {
        jobj["civilizations"] = 123;

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }

    SECTION("Missing banners")
    {
        jobj.remove("banners");

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }

    SECTION("Banners not an array")
    {
        jobj["banners"] = 123;

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }

    SECTION("Missing colors")
    {
        jobj.remove("colors");

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }

    SECTION("Colors not an array")
    {
        jobj["colors"] = 123;

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }

    SECTION("Missing componentTypes")
    {
        jobj.remove("componentTypes");

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }

    SECTION("ComponentTypes not an array")
    {
        jobj["componentTypes"] = 123;

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }

    SECTION("Missing builtInObjectIds")
    {
        jobj.remove("builtInObjectIds");

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }

    SECTION("BuiltInObjectIds not an array")
    {
        jobj["builtInObjectIds"] = 123;

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }

    SECTION("Invalid builtInObjectId")
    {
        jobj["builtInObjectIds"] = QJsonObject{{core::PositionComponentType::staticMetaObject.className(), "asd"}};

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }

    SECTION("Missing rulesEntryPoint")
    {
        jobj.remove("rulesEntryPoint");

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }

    SECTION("Invalid rulesEntryPoint")
    {
        jobj["rulesEntryPoint"] = 123;

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }

    SECTION("Missing rulesType")
    {
        jobj.remove("rulesType");

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }

    SECTION("Invalid rulesType")
    {
        jobj["rulesType"] = 123;

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);

        jobj["rulesType"] = "NonExisting";

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(QJsonDocument(jobj).toJson()), utils::ValueError);
    }
}
