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
#include "io/JsonUtils.h"
#include "io/WorldJsonUnserializer.h"
#include "test/Util.h"
#include "test/catch.hpp"
#include "utils/Exception.h"

using namespace warmonger;

TEST_CASE("Banner can be unserialized from JSON", "[WorldJsonUnserializer][JSON][Unserialize][HappyPath]")
{
    std::unique_ptr<core::World> world;
    QJsonObject jworld;
    std::tie(world, jworld) = makeWorld();

    const QJsonObject jobj = jworld["banners"].toArray()[0].toObject();

    const QJsonDocument jdoc{jobj};
    const io::WorldJsonUnserializer unserializer;
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
        REQUIRE(banner->getDisplayName() == jobj["displayName"].toString());
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

TEST_CASE("Banner can't be unserialized from JSON", "[JsonUnserializer]")
{
    std::unique_ptr<core::World> world;
    QJsonObject jworld;
    std::tie(world, jworld) = makeWorld();

    const io::WorldJsonUnserializer unserializer;
    QJsonObject jobj = jworld["banners"].toArray()[0].toObject();

    SECTION("invalid JSON")
    {
        QString invalidJson{"{\"displayName\": \"name1\",}"};
        REQUIRE_THROWS_AS(unserializer.unserializeBanner(invalidJson.toLocal8Bit(), world.get()), utils::ValueError);
    }

    SECTION("unserializing Banner, missing id")
    {
        jobj.remove("id");
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeBanner(jdoc.toJson(), world.get()), utils::ValueError);
    }

    SECTION("unserializing Banner, id not a number")
    {
        jobj["id"] = "asd";
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeBanner(jdoc.toJson(), world.get()), utils::ValueError);
    }

    SECTION("unserializing Banner, missing name")
    {
        jobj.remove("displayName");
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeBanner(jdoc.toJson(), world.get()), utils::ValueError);
    }

    SECTION("unserializing Banner, civilizations is not an array")
    {
        jobj["civilizations"] = "asd";
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeBanner(jdoc.toJson(), world.get()), utils::ValueError);
    }
}

TEST_CASE("EntityType can be unserialized from JSON", "[WorldJsonUnserializer][JSON][Unserialize][HappyPath]")
{
    std::unique_ptr<core::World> world;
    QJsonObject jworld;
    std::tie(world, jworld) = makeWorld();

    QJsonObject jobj = jworld["entityTypes"].toArray()[0].toObject();

    const QJsonDocument jdoc{jobj};
    const io::WorldJsonUnserializer unserializer;
    const QByteArray rawJson{jdoc.toJson()};

    INFO("The json component-type is: " << rawJson.data());

    SECTION("Unserialization succeeds without exceptions")
    {
        REQUIRE_NOTHROW(unserializer.unserializeEntityType(rawJson, world.get()));
        REQUIRE(unserializer.unserializeEntityType(rawJson, world.get()));
    }

    SECTION("Unserialization entity-type")
    {
        const auto entityType{unserializer.unserializeEntityType(rawJson, world.get())};

        REQUIRE(entityType->getName() == jobj["name"].toString());
        REQUIRE_REFERENCES(jobj["componentTypes"].toArray(), entityType->getComponentTypes());
    }
}

TEST_CASE("EntityType can't be unserialized from JSON", "[WorldJsonUnserializer][JSON][Unserialize][ErrorPaths]")
{
    auto worlds = makeWorld();
    core::World* world = worlds.first.get();

    const QString validComponentTypeRef{io::serializeReference(world->getComponentTypes()[0])};

    const io::WorldJsonUnserializer unserializer;

    SECTION("Invalid Json")
    {
        QString invalidJson{"{\"name\": \"name1\",}"};

        REQUIRE_THROWS_AS(unserializer.unserializeEntityType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("No name")
    {
        QString invalidJson{QString("{\"componentTypes\": [\"") + validComponentTypeRef + "\"]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeEntityType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Empty name")
    {
        QString invalidJson{QString("{\"name\": \"\", \"componentTypes\": [\"") + validComponentTypeRef + "\"]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeEntityType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Name not string")
    {
        QString invalidJson{QString("{\"name\": 4, \"componentTypes\": [\"") + validComponentTypeRef + "\"]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeEntityType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("No component-types")
    {
        QString invalidJson{"{\"name\": \"name1\"}"};

        REQUIRE_THROWS_AS(unserializer.unserializeEntityType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Component-type list is empty")
    {
        QString invalidJson{"{\"name\": \"name1\", \"componentTypes\": []}"};

        REQUIRE_THROWS_AS(unserializer.unserializeEntityType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Component-types has empty name")
    {
        QString invalidJson{"{\"name\": \"name1\", \"componentTypes\": [\"\""
                            "]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeEntityType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Component-type has non-string name")
    {
        QString invalidJson{"{\"name\": \"name1\", \"componentTypes\": [1]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeEntityType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Component-type does not exist")
    {
        QString invalidJson{"{\"name\": \"name1\", \"componentTypes\": [\"nonExistentcomponentType\""
                            "]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeEntityType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }
}

TEST_CASE("ComponentType unserialized from JSON - happy path", "[WorldJsonUnserializer][JSON][Unserialize][HappyPath]")
{
    auto worlds = makeWorld();
    core::World* world = worlds.first.get();

    QString jsonStr{"{"
                    "\"name\": \"componentType1\","
                    "\"fields\": ["
                    "   {\"name\": \"intField\", \"type\": \"Integer\"},"
                    "   {\"name\": \"realField\", \"type\": \"Real\"},"
                    "   {\"name\": \"strField\", \"type\": \"String\"},"
                    "   {\"name\": \"refField\", \"type\": \"Reference\"},"
                    "   {\"name\": \"intsListField\", \"type\": {\"id\": \"List\", \"valueType\": \"Integer\"}},"
                    "   {\"name\": \"realDictField\", \"type\": {\"id\": \"Dictionary\", \"valueType\": \"Real\"}},"
                    "   {\"name\": \"dictOfRefListsField\", \"type\": {\"id\": \"Dictionary\", \"valueType\": {"
                    "       \"id\": \"List\", \"valueType\": \"Reference\"}}}"
                    "]}"};

    const io::WorldJsonUnserializer unserializer;
    const QByteArray rawJson{jsonStr.toLocal8Bit()};

    INFO("The json component-type is: " << rawJson.data());

    SECTION("Unserialization succeeds without exceptions")
    {
        REQUIRE_NOTHROW(unserializer.unserializeComponentType(rawJson, world));
        REQUIRE(unserializer.unserializeComponentType(rawJson, world));
    }

    SECTION("Unserialization component-type")
    {
        const auto componentType{unserializer.unserializeComponentType(rawJson, world)};

        const QJsonDocument jdoc{io::parseJson(rawJson)};
        const QJsonObject jobj{jdoc.object()};
        const QJsonArray jfields(jobj["fields"].toArray());

        REQUIRE(componentType->getName() == jobj["name"].toString());
        REQUIRE(componentType->getFields().size() == jfields.size());

        const auto& fields{componentType->getFields()};

        for (int i = 0; i < jfields.size(); ++i)
        {
            REQUIRE(fields[i]->getName() == jfields[i].toObject()["name"].toString());
        }

        REQUIRE(fields[0]->getType()->id() == core::Field::TypeId::Integer);
        REQUIRE(fields[1]->getType()->id() == core::Field::TypeId::Real);
        REQUIRE(fields[2]->getType()->id() == core::Field::TypeId::String);
        REQUIRE(fields[3]->getType()->id() == core::Field::TypeId::Reference);

        REQUIRE(fields[4]->getType()->id() == core::Field::TypeId::List);

        const auto listType{dynamic_cast<core::FieldTypes::List*>(fields[4]->getType())};
        REQUIRE(listType != nullptr);
        REQUIRE(listType->getValueType()->id() == core::Field::TypeId::Integer);

        REQUIRE(fields[5]->getType()->id() == core::Field::TypeId::Dictionary);

        const auto dictType{dynamic_cast<core::FieldTypes::Dictionary*>(fields[5]->getType())};
        REQUIRE(dictType != nullptr);
        REQUIRE(dictType->getValueType()->id() == core::Field::TypeId::Real);

        REQUIRE(fields[6]->getType()->id() == core::Field::TypeId::Dictionary);

        const auto outerDictType{dynamic_cast<core::FieldTypes::Dictionary*>(fields[6]->getType())};
        REQUIRE(outerDictType != nullptr);
        REQUIRE(outerDictType->getValueType()->id() == core::Field::TypeId::List);

        const auto innerListType{dynamic_cast<core::FieldTypes::List*>(outerDictType->getValueType())};
        REQUIRE(innerListType != nullptr);
        REQUIRE(innerListType->getValueType()->id() == core::Field::TypeId::Reference);
    }
}

TEST_CASE(
    "ComponentType unserialized from JSON - error paths", "[WorldJsonUnserializer][JSON][Unserialize][ErrorPaths]")
{
    auto worlds = makeWorld();
    core::World* world = worlds.first.get();

    io::WorldJsonUnserializer unserializer;

    SECTION("Invalid Json")
    {
        QString invalidJson{"{\"name\": \"name1\",}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("No name")
    {
        QString invalidJson{"{\"fields\": [{\"name\": \"f0\", \"type\": \"String\"}]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Empty name")
    {
        QString invalidJson{"{\"name\": \"\", \"fields\": [{\"name\": \"f0\", \"type\": \"String\"}]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Name not string")
    {
        QString invalidJson{"{\"name\": 3, \"fields\": [{\"name\": \"f0\", \"type\": \"String\"}]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("No fields")
    {
        QString invalidJson{"{\"name\": \"name1\"}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Fields list is empty")
    {
        QString invalidJson{"{\"name\": \"ct0\", \"fields\": []}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Field has empty name")
    {
        QString invalidJson{"{\"name\": \"ct0\", \"fields\": [{\"name\": \"\", \"type\": \"String\"}]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Field has non-string name")
    {
        QString invalidJson{"{\"name\": \"ct0\", \"fields\": [{\"name\": 7.0, \"type\": \"String\"}]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Field has no type")
    {
        QString invalidJson{"{\"name\": \"ct0\", \"fields\": [{\"name\": \"f0\"}]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Field has invalid type")
    {
        QString invalidJson{"{\"name\": \"ct0\", \"fields\": [{\"name\": \"f0\", \"type\": []}]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Field has empty string type")
    {
        QString invalidJson{"{\"name\": \"ct0\", \"fields\": [{\"name\": \"f0\", \"type\": \"\"}]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Field has unknown type")
    {
        QString invalidJson{"{\"name\": \"ct0\", \"fields\": [{\"name\": \"f0\", \"type\": \"Unknown\"}]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Field has invalid type")
    {
        QString invalidJson{"{\"name\": \"ct0\", \"fields\": [{\"name\": \"f0\", \"type\": \"List\"}]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Complex field type has no id")
    {
        QString invalidJson{
            "{\"name\": \"ct0\", \"fields\": [{\"name\": \"f0\", \"type\": {\"valueType\": \"Integer\"}}]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Complex field type has empty id")
    {
        QString invalidJson{"{\"name\": \"ct0\", \"fields\": [{\"name\": \"f0\", \"type\": {\"id\": \"\", "
                            "\"valueType\": \"Integer\"}}]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Complex field type has unknown id")
    {
        QString invalidJson{"{\"name\": \"ct0\", \"fields\": [{\"name\": \"f0\", \"type\": {\"id\": \"Unknown\", "
                            "\"valueType\": \"Integer\"}}]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Complex field type has invalid id")
    {
        QString invalidJson{"{\"name\": \"ct0\", \"fields\": [{\"name\": \"f0\", \"type\": {\"id\": \"String\", "
                            "\"valueType\": \"Integer\"}}]}"};

        REQUIRE_THROWS_AS(unserializer.unserializeComponentType(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }
}
