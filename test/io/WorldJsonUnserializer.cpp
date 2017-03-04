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

#include "core/ComponentType.h"
#include "core/EntityType.h"
#include "core/World.h"
#include "core/WorldComponentType.h"
#include "io/WorldJsonUnserializer.h"
#include "test/catch.hpp"
#include "utils/Exception.h"

using namespace warmonger;

/*
TEST_CASE("World can be serialized to JSON", "[WorldJsonUnerializer]")
{
}
*/

TEST_CASE("EntityType can be serialized to JSON", "[WorldJsonUnserializer][JSON][Unserialize][HappyPath]")
{
    QJsonObject jobj;
    jobj["name"] = "entityType1";

    const QJsonArray jcomponentTypes{"componentType1"};

    jobj["componentTypes"] = jcomponentTypes;

    const QJsonDocument jdoc{jobj};
    const io::WorldJsonUnserializer unserializer;
    const QByteArray rawJson{jdoc.toJson()};

    INFO("The json component-type is: " << rawJson.data());

    const auto componentType1{
        std::make_unique<core::WorldComponentType>("componentType1", std::vector<QString>{QString("property1")})};
    const std::vector<core::ComponentType*> componentTypes{componentType1.get()};

    SECTION("Unserialization succeeds without exceptions")
    {
        REQUIRE_NOTHROW(unserializer.unserializeEntityType(rawJson, componentTypes));
        REQUIRE(unserializer.unserializeEntityType(rawJson, componentTypes));
    }

    SECTION("Unserialization entity-type")
    {
        const auto entityType{unserializer.unserializeEntityType(rawJson, componentTypes)};

        REQUIRE(entityType->getName() == jobj["name"].toString());
        REQUIRE(entityType->getComponentTypes().size() == jcomponentTypes.size());
    }
}

TEST_CASE("EntityType unserialized from JSON - error paths", "[WorldJsonUnserializer][JSON][Unserialize][ErrorPaths]")
{
    const io::WorldJsonUnserializer unserializer;
    const auto componentType1{
        std::make_unique<core::WorldComponentType>("componentType1", std::vector<QString>{QString("property1")})};
    const std::vector<core::ComponentType*> componentTypes{componentType1.get()};

    SECTION("Invalid Json")
    {
        QString invalidJson{"{\"name\": \"name1\",}"};

        REQUIRE_THROWS_AS(
            unserializer.unserializeEntityType(invalidJson.toLocal8Bit(), componentTypes), utils::ValueError);
    }

    SECTION("No name")
    {
        QString invalidJson{"{\"componentTypes\": [\"componentType1\"]}"};

        REQUIRE_THROWS_AS(
            unserializer.unserializeEntityType(invalidJson.toLocal8Bit(), componentTypes), utils::ValueError);
    }

    SECTION("Empty name")
    {
        QString invalidJson{"{\"name\": \"\", \"componentTypes\": [\"componentType1\"]}"};

        REQUIRE_THROWS_AS(
            unserializer.unserializeEntityType(invalidJson.toLocal8Bit(), componentTypes), utils::ValueError);
    }

    SECTION("Name not string")
    {
        QString invalidJson{"{\"name\": 4, \"componentTypes\": [\"componentType1\"]}"};

        REQUIRE_THROWS_AS(
            unserializer.unserializeEntityType(invalidJson.toLocal8Bit(), componentTypes), utils::ValueError);
    }

    SECTION("No component-types")
    {
        QString invalidJson{"{\"name\": \"name1\"}"};

        REQUIRE_THROWS_AS(
            unserializer.unserializeEntityType(invalidJson.toLocal8Bit(), componentTypes), utils::ValueError);
    }

    SECTION("Component-type list is empty")
    {
        QString invalidJson{"{\"name\": \"name1\", \"componentTypes\": []}"};

        REQUIRE_THROWS_AS(
            unserializer.unserializeEntityType(invalidJson.toLocal8Bit(), componentTypes), utils::ValueError);
    }

    SECTION("Component-types has empty name")
    {
        QString invalidJson{"{\"name\": \"name1\", \"componentTypes\": [\"\""
                            "]}"};

        REQUIRE_THROWS_AS(
            unserializer.unserializeEntityType(invalidJson.toLocal8Bit(), componentTypes), utils::ValueError);
    }

    SECTION("Component-type has non-string name")
    {
        QString invalidJson{"{\"name\": \"name1\", \"componentTypes\": [1]}"};

        REQUIRE_THROWS_AS(
            unserializer.unserializeEntityType(invalidJson.toLocal8Bit(), componentTypes), utils::ValueError);
    }

    SECTION("Component-type does not exist")
    {
        QString invalidJson{"{\"name\": \"name1\", \"componentTypes\": [\"nonExistentcomponentType\""
                            "]}"};

        REQUIRE_THROWS_AS(
            unserializer.unserializeEntityType(invalidJson.toLocal8Bit(), componentTypes), utils::ValueError);
    }
}

TEST_CASE("ComponentType unserialized from JSON - happy path", "[WorldJsonUnserializer][JSON][Unserialize][HappyPath]")
{
    QJsonObject jobj;
    jobj["name"] = "componentType1";

    const QJsonArray jpropertyNames{QJsonValue("property1"), QJsonValue("property2")};
    jobj["propertyNames"] = jpropertyNames;

    const QJsonDocument jdoc{jobj};
    const io::WorldJsonUnserializer unserializer;
    const QByteArray rawJson{jdoc.toJson()};

    INFO("The json component-type is: " << rawJson.data());

    SECTION("Unserialization succeeds without exceptions")
    {
        REQUIRE_NOTHROW(unserializer.unserializeComponentType(rawJson));
        REQUIRE(unserializer.unserializeComponentType(rawJson));
    }

    SECTION("Unserialization component-type")
    {
        const auto componentType{unserializer.unserializeComponentType(rawJson)};

        REQUIRE(componentType->getName() == jobj["name"].toString());
        REQUIRE(componentType->getPropertyNames().size() == jpropertyNames.size());

        const auto& propertyNames{componentType->getPropertyNames()};

        for (int i = 0; i < jpropertyNames.size(); ++i)
        {
            REQUIRE(propertyNames[i] == jpropertyNames[i].toString());
        }
    }
}

TEST_CASE(
    "ComponentType unserialized from JSON - error paths", "[WorldJsonUnserializer][JSON][Unserialize][ErrorPaths]")
{
    io::WorldJsonUnserializer unserializer;

    SECTION("Invalid Json")
    {
        QString invalidJson{"{\"name\": \"name1\",}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit()), utils::ValueError);
    }

    SECTION("No name")
    {
        QString invalidJson{"{\"propertyNames\": [\"property1\"]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit()), utils::ValueError);
    }

    SECTION("Empty name")
    {
        QString invalidJson{"{\"name\": \"\", \"propertyNames\": [\"property1\"]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit()), utils::ValueError);
    }

    SECTION("Name not string")
    {
        QString invalidJson{"{\"name\": 6, \"propertyNames\": [\"property1\"]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit()), utils::ValueError);
    }

    SECTION("No properties")
    {
        QString invalidJson{"{\"name\": \"name1\"}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit()), utils::ValueError);
    }

    SECTION("Properties list is empty")
    {
        QString invalidJson{"{\"name\": \"name1\", \"properties\": []}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit()), utils::ValueError);
    }

    SECTION("Property has empty name")
    {
        QString invalidJson{"{\"name\": \"name1\", \"properties\": [\"\""
                            "]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit()), utils::ValueError);
    }

    SECTION("Property has non-string name")
    {
        QString invalidJson{"{\"name\": \"name1\", \"properties\": [1]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit()), utils::ValueError);
    }
}
