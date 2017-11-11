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

#include "io/MapJsonUnserializer.h"
#include "test/Util.h"
#include "utils/Exception.h"
#include <catch.hpp>

using namespace warmonger;

std::ostream& operator<<(std::ostream& s, const QVariant& v)
{
    switch (v.type())
    {
        case QVariant::Invalid:
        {
            s << "null";
        }
        break;

        case QVariant::Int:
        {
            s << v.toInt();
        }
        break;

        case QVariant::Double:
        {
            s << v.toDouble();
        }
        break;

        case QVariant::String:
        {
            s << v.toString();
        }
        break;

        case QVariant::List:
        {
            s << "[";

            auto list = v.toList();
            for (auto it = list.begin(); it != list.end(); ++it)
            {
                s << *it;
                if (it != --list.end())
                    s << ", ";
            }
            s << "]";
        }
        break;

        case QVariant::Map:
        {
            s << "{";

            auto map = v.toMap();
            for (auto it = map.begin(); it != map.end(); ++it)
            {
                s << it.key() << ": " << it.value();
                if (it != --map.end())
                    s << ", ";
            }
            s << "}";
        }
        break;

        default:
        {
            s << "{?}";
        }
    }

    return s;
}

TEST_CASE("MapNode can be unserialized from JSON", "[MapJsonUnserializer][JSON][Unserialize][HappyPath]")
{
    const auto maps = makeMap();
    const auto map = maps.first.get();
    const auto jmap = maps.second;

    const auto jobj = jmap["mapNodes"].toArray()[0].toObject();
    const QJsonDocument jdoc{jobj};
    const QByteArray rawJson{jdoc.toJson()};

    INFO("The json map-node is: " << rawJson.data());

    const io::MapJsonUnserializer unserializer;

    SECTION("Unserialization succeeds without exceptions")
    {
        REQUIRE_NOTHROW(unserializer.unserializeMapNode(rawJson, map));
        REQUIRE(unserializer.unserializeMapNode(rawJson, map));
    }

    SECTION("Unserializing MapNode")
    {
        const auto mapNode = unserializer.unserializeMapNode(rawJson, map);

        REQUIRE(mapNode->getId() == jobj["id"].toInt());

        const auto neighbours = mapNode->getNeighbours();
        const auto jneighbours = jobj["neighbours"].toObject();
        for (auto it = jneighbours.begin(); it != jneighbours.end(); ++it)
        {
            core::Direction d = core::str2direction(it.key());
            const QString ref = it.value().toString();

            if (!ref.isEmpty())
            {
                REQUIRE(neighbours.at(d) == io::unserializeReference(ref, map));
            }
            else
            {
                REQUIRE(neighbours.at(d) == nullptr);
            }
        }
    }
}

TEST_CASE("MapNode can't be unserialized from JSON", "[MapJsonUnserializer][JSON][Unserialize][ErrorPaths]")
{
    const auto maps = makeMap();
    const auto map = maps.first.get();
    const auto jmap = maps.second;

    const io::MapJsonUnserializer unserializer;
    QJsonObject jobj = jmap["mapNodes"].toArray()[0].toObject();

    SECTION("Invalid JSON")
    {
        QString invalidJson{"{\"id\": \"1\",}"};
        REQUIRE_THROWS_AS(unserializer.unserializeMapNode(invalidJson.toLocal8Bit(), map), utils::ValueError);
    }

    SECTION("Missing id")
    {
        jobj.remove("id");
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeMapNode(jdoc.toJson(), map), utils::ValueError);
    }

    SECTION("Id not a number")
    {
        jobj["id"] = "asd";
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeMapNode(jdoc.toJson(), map), utils::ValueError);
    }

    SECTION("Missing neighbours")
    {
        jobj.remove("neighbours");
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeMapNode(jdoc.toJson(), map), utils::ValueError);
    }

    SECTION("Neighbours not an object")
    {
        jobj["neighbours"] = QJsonArray();
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeMapNode(jdoc.toJson(), map), utils::ValueError);
    }

    SECTION("Neighbours has invalid direction")
    {
        auto neighbours = jobj["neighbours"].toObject();

        neighbours["invalid"] = "";

        jobj["neighbours"] = neighbours;

        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeMapNode(jdoc.toJson(), map), utils::ValueError);
    }
}

TEST_CASE("Faction can be unserialized from JSON", "[MapJsonUnserializer][JSON][Unserialize][HappyPath]")
{
    const auto maps = makeMap();
    const auto map = maps.first.get();
    const auto jmap = maps.second;

    const auto jobj = jmap["factions"].toArray()[0].toObject();
    const QJsonDocument jdoc{jobj};
    const QByteArray rawJson{jdoc.toJson()};

    INFO("The json faction is: " << rawJson.data());

    const io::MapJsonUnserializer unserializer;

    SECTION("Unserialization succeeds without exceptions")
    {
        REQUIRE_NOTHROW(unserializer.unserializeFaction(rawJson, map));
        REQUIRE(unserializer.unserializeFaction(rawJson, map));
    }

    SECTION("Unserializing Faction")
    {
        const auto faction{unserializer.unserializeFaction(jdoc.toJson(), map)};

        REQUIRE(faction->getId() == jobj["id"].toInt());
        REQUIRE(faction->getName() == jobj["name"].toString());
        REQUIRE(faction->getPrimaryColor().name() == jobj["primaryColor"].toString());
        REQUIRE(faction->getSecondaryColor().name() == jobj["secondaryColor"].toString());
        REQUIRE(faction->getBanner() == io::unserializeReference(jobj["banner"].toString(), map));
        REQUIRE(faction->getCivilization() == io::unserializeReference(jobj["civilization"].toString(), map));
    }
}

TEST_CASE("Faction can't be unserialized from JSON", "[MapJsonUnserializer][JSON][Unserialize][ErrorPaths]")
{
    const auto maps = makeMap();
    const auto map = maps.first.get();
    const auto jmap = maps.second;

    const io::MapJsonUnserializer unserializer;
    QJsonObject jobj = jmap["factions"].toArray()[0].toObject();

    SECTION("Invalid JSON")
    {
        QString invalidJson{"{\"id\": \"1\",}"};
        REQUIRE_THROWS_AS(unserializer.unserializeFaction(invalidJson.toLocal8Bit(), map), utils::ValueError);
    }

    SECTION("Missing id")
    {
        jobj.remove("id");
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeFaction(jdoc.toJson(), map), utils::ValueError);
    }

    SECTION("Id not a number")
    {
        jobj["id"] = "asd";
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeFaction(jdoc.toJson(), map), utils::ValueError);
    }

    SECTION("Missing name")
    {
        jobj.remove("name");
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeFaction(jdoc.toJson(), map), utils::ValueError);
    }

    SECTION("Empty name")
    {
        jobj["name"] = "";

        REQUIRE_THROWS_AS(unserializer.unserializeFaction(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Missing primaryColor")
    {
        jobj.remove("primaryColor");
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeFaction(jdoc.toJson(), map), utils::ValueError);
    }

    SECTION("Empty primaryColor")
    {
        jobj["primaryColor"] = "";

        REQUIRE_THROWS_AS(unserializer.unserializeFaction(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Invalid primaryColor")
    {
        jobj["primaryColor"] = "asdfasdf";

        REQUIRE_THROWS_AS(unserializer.unserializeFaction(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Missing secondaryColor")
    {
        jobj.remove("secondaryColor");
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeFaction(jdoc.toJson(), map), utils::ValueError);
    }

    SECTION("Empty secondaryColor")
    {
        jobj["secondaryColor"] = "";

        REQUIRE_THROWS_AS(unserializer.unserializeFaction(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Invalid secondaryColor")
    {
        jobj["secondaryColor"] = "asdfasdf";

        REQUIRE_THROWS_AS(unserializer.unserializeFaction(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Missing banner")
    {
        jobj.remove("banner");
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeFaction(jdoc.toJson(), map), utils::ValueError);
    }

    SECTION("Missing civilization")
    {
        jobj.remove("civilization");
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeFaction(jdoc.toJson(), map), utils::ValueError);
    }
}

TEST_CASE("Entity can be unserialized from JSON", "[MapJsonUnserializer][JSON][Unserialize][HappyPath]")
{
    const auto maps = makeMap();
    const auto map = maps.first.get();
    const auto jmap = maps.second;

    const auto jobj = jmap["entities"].toArray()[0].toObject();
    const QJsonDocument jdoc{jobj};
    const QByteArray rawJson{jdoc.toJson()};

    INFO("The json entity is: " << rawJson.data());

    const io::MapJsonUnserializer unserializer;

    SECTION("Unserialization succeeds without exceptions")
    {
        REQUIRE_NOTHROW(unserializer.unserializeEntity(rawJson, map));
        REQUIRE(unserializer.unserializeEntity(rawJson, map));
    }

    SECTION("Unserializing Entity")
    {
        const auto entity{unserializer.unserializeEntity(jdoc.toJson(), map)};

        REQUIRE(entity->getId() == jobj["id"].toInt());

        const auto& components{entity->getComponents()};
        const auto jcomponents = jobj["components"].toArray();

        REQUIRE(jcomponents.size() == components.size());

        for (const auto jcomponentValue : jcomponents)
        {
            const auto jcomponent{jcomponentValue.toObject()};
            const auto componentType{
                io::unserializeReferenceAs<core::ComponentType>(jcomponent["type"].toString(), map)};
            const auto jfields{jcomponent["fields"].toObject()};

            REQUIRE(entity->getComponent(componentType));

            for (auto field : componentType->getFields())
            {
                const auto fieldName{field->getName()};
                const auto val{jfields[fieldName]};

                INFO("The field name is " << fieldName);

                auto fieldValue{entity->getComponent(componentType)->field(fieldName)};

                switch (field->getType())
                {
                    case core::Field::Type::Integer:
                        REQUIRE(val.toInt() == fieldValue->asInteger());
                        break;

                    case core::Field::Type::Real:
                        REQUIRE(val.toDouble() == fieldValue->asReal());
                        break;

                    case core::Field::Type::String:
                        REQUIRE(val.toString() == fieldValue->asString());
                        break;

                    case core::Field::Type::Reference:
                        REQUIRE(io::unserializeReference(val.toString(), map) == fieldValue->asReference());
                        break;

                    case core::Field::Type::List:
                        // REQUIRE(val.toString() == *fieldValue->asString()); TODO
                        break;

                    case core::Field::Type::Map:
                        // REQUIRE(val.toString() == *fieldValue->asString()); TODO
                        break;
                }
            }
        }
    }
}

TEST_CASE("Entity can't be unserialized from JSON", "[MapJsonUnserializer][JSON][Unserialize][ErrorPaths]")
{
    const auto maps = makeMap();
    const auto map = maps.first.get();
    const auto jmap = maps.second;

    const io::MapJsonUnserializer unserializer;
    QJsonObject jobj = jmap["entities"].toArray()[0].toObject();

    SECTION("Invalid JSON")
    {
        QString invalidJson{"{\"id\": \"1\",}"};
        REQUIRE_THROWS_AS(unserializer.unserializeEntity(invalidJson.toLocal8Bit(), map), utils::ValueError);
    }

    SECTION("Missing id")
    {
        jobj.remove("id");
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(jdoc.toJson(), map), utils::ValueError);
    }

    SECTION("Id not a number")
    {
        jobj["id"] = "asd";
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(jdoc.toJson(), map), utils::ValueError);
    }

    SECTION("Missing components")
    {
        jobj.remove("components");
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(jdoc.toJson(), map), utils::ValueError);
    }

    SECTION("Invalid components")
    {
        jobj["components"] = 123;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    const auto& componentTypes = map->getWorld()->getComponentTypes();
    auto componentType1 = componentTypes[componentTypes.size() - 1];
    auto jcomponents = jobj["components"].toArray();
    auto jcomponent = jcomponents[1].toObject();
    auto jfields = jcomponent["fields"].toObject();

    SECTION("Invalid component")
    {
        jcomponents[0] = 123;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Missing type")
    {
        jcomponent.remove("type");
        jcomponents[1] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Invalid type")
    {
        jcomponent["type"] = 123;
        jcomponents[1] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Missing fields")
    {
        jcomponent.remove("fields");
        jcomponents[1] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Invalid fields")
    {
        jcomponent["fields"] = 123;
        jcomponents[1] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("A field is missing")
    {
        jfields.remove("intField");
        jcomponent["fields"] = jfields;
        jcomponents[1] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Integer field is invalid")
    {
        jfields["intField"] = "123";
        jcomponent["fields"] = jfields;
        jcomponents[1] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Integer field is real")
    {
        jfields["intField"] = 123.23;
        jcomponent["fields"] = jfields;
        jcomponents[1] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Real field is invalid")
    {
        jfields["realField"] = QJsonArray();
        jcomponent["fields"] = jfields;
        jcomponents[1] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("String field is invalid")
    {
        jfields["strField"] = 123;
        jcomponent["fields"] = jfields;
        jcomponents[1] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Reference field is invalid")
    {
        jfields["refField"] = 123;
        jcomponent["fields"] = jfields;
        jcomponents[1] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("List field is invalid")
    {
        jfields["listField"] = 123;
        jcomponent["fields"] = jfields;
        jcomponents[1] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Map field is invalid")
    {
        jfields["mapField"] = 123;
        jcomponent["fields"] = jfields;
        jcomponents[1] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }
}

TEST_CASE("Map can be unserialized from JSON", "[MapJsonUnserializer][JSON][Unserialize][HappyPath]")
{
    const auto maps = makeMap();
    const auto jmap = maps.second;
    auto world = maps.first->getWorld();

    const QJsonDocument jdoc{jmap};
    const QByteArray rawJson{jdoc.toJson()};

    INFO("The json map is: " << rawJson.data());

    const io::MapJsonUnserializer unserializer;

    SECTION("Unserialization succeeds without exceptions")
    {
        REQUIRE_NOTHROW(unserializer.unserializeMap(rawJson, world));
        REQUIRE(unserializer.unserializeMap(rawJson, world));
    }

    SECTION("Unserializing map")
    {
        const auto map = unserializer.unserializeMap(rawJson, world);

        REQUIRE(map->getName() == jmap["name"].toString());
        REQUIRE(map->getWorld() == world);

        const auto jmapNodes = jmap["mapNodes"].toArray();
        const auto& mapNodes = map->getMapNodes();
        REQUIRE(map->getMapNodes().size() == jmapNodes.size());

        for (const auto& jmapNodeVal : jmapNodes)
        {
            const auto jmapNode = jmapNodeVal.toObject();

            const auto mapNodeIt = std::find_if(mapNodes.cbegin(), mapNodes.cend(), [&jmapNode](const auto& mapNode) {
                return mapNode->getId() == jmapNode["id"].toInt();
            });

            REQUIRE(mapNodeIt != mapNodes.cend());

            auto mapNode = *mapNodeIt;

            const auto jneighbours = jmapNode["neighbours"].toObject();
            for (auto it = jneighbours.begin(); it != jneighbours.end(); ++it)
            {
                core::Direction direction{core::str2direction(it.key())};

                const auto jneighbour = it.value().toString();
                if (jneighbour.isEmpty())
                    REQUIRE(mapNode->getNeighbour(direction) == nullptr);
                else
                    REQUIRE(mapNode->getNeighbour(direction) == io::unserializeReference(jneighbour, map.get()));
            }
        }

        REQUIRE(map->getFactions().size() == jmap["factions"].toArray().size());
        REQUIRE(map->getEntities().size() == jmap["entities"].toArray().size());
    }
}

TEST_CASE("Map can't be unserialized from JSON", "[MapJsonUnserializer][JSON][Unserialize][ErrorPaths]")
{
    const auto maps = makeMap();
    const auto map = maps.first.get();
    auto world = maps.first->getWorld();

    auto jmap = maps.second;

    const io::MapJsonUnserializer unserializer;

    SECTION("Invalid JSON")
    {
        QString invalidJson{"{\"id\": \"1\",}"};
        REQUIRE_THROWS_AS(unserializer.unserializeMap(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Wrong world")
    {
        core::World w("wrong-uuid");

        REQUIRE_THROWS_AS(unserializer.unserializeMap(QJsonDocument(jmap).toJson(), &w), utils::ValueError);
    }

    SECTION("Missing name")
    {
        jmap.remove("name");
        QJsonDocument jdoc(jmap);

        REQUIRE_THROWS_AS(unserializer.unserializeMap(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Invalid name")
    {
        jmap["name"] = 123;
        QJsonDocument jdoc(jmap);

        REQUIRE_THROWS_AS(unserializer.unserializeMap(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Missing mapNodes")
    {
        jmap.remove("mapNodes");
        QJsonDocument jdoc(jmap);

        REQUIRE_THROWS_AS(unserializer.unserializeMap(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Invalid mapNodes")
    {
        jmap["mapNodes"] = 123;
        QJsonDocument jdoc(jmap);

        REQUIRE_THROWS_AS(unserializer.unserializeMap(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Missing factions")
    {
        jmap.remove("factions");
        QJsonDocument jdoc(jmap);

        REQUIRE_THROWS_AS(unserializer.unserializeMap(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Invalid factions")
    {
        jmap["factions"] = 123;
        QJsonDocument jdoc(jmap);

        REQUIRE_THROWS_AS(unserializer.unserializeMap(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Missing entities")
    {
        jmap.remove("entities");
        QJsonDocument jdoc(jmap);

        REQUIRE_THROWS_AS(unserializer.unserializeMap(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Invalid entities")
    {
        jmap["entities"] = 123;
        QJsonDocument jdoc(jmap);

        REQUIRE_THROWS_AS(unserializer.unserializeMap(jdoc.toJson(), world), utils::ValueError);
    }
}
