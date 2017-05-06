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

#include "io/CampaignMapJsonUnserializer.h"
#include "test/Util.h"
#include "test/catch.hpp"
#include "utils/Exception.h"

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

TEST_CASE("MapNode can be unserialized from JSON", "[CampaignMapJsonUnserializer][JSON][Unserialize][HappyPath]")
{
    const auto maps = makeMap();
    const auto map = maps.first.get();
    const auto jmap = maps.second;

    const auto jobj = jmap["mapNodes"].toArray()[0].toObject();
    const QJsonDocument jdoc{jobj};
    const QByteArray rawJson{jdoc.toJson()};

    INFO("The json map-node is: " << rawJson.data());

    const io::CampaignMapJsonUnserializer unserializer;

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

TEST_CASE("MapNode can't be unserialized from JSON", "[CampaignMapJsonUnserializer][JSON][Unserialize][ErrorPaths]")
{
    const auto maps = makeMap();
    const auto map = maps.first.get();
    const auto jmap = maps.second;

    const io::CampaignMapJsonUnserializer unserializer;
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

TEST_CASE("Faction can be unserialized from JSON", "[CampaignMapJsonUnserializer][JSON][Unserialize][HappyPath]")
{
    const auto maps = makeMap();
    const auto map = maps.first.get();
    const auto jmap = maps.second;

    const auto jobj = jmap["factions"].toArray()[0].toObject();
    const QJsonDocument jdoc{jobj};
    const QByteArray rawJson{jdoc.toJson()};

    INFO("The json faction is: " << rawJson.data());

    const io::CampaignMapJsonUnserializer unserializer;

    SECTION("Unserialization succeeds without exceptions")
    {
        REQUIRE_NOTHROW(unserializer.unserializeFaction(rawJson, map));
        REQUIRE(unserializer.unserializeFaction(rawJson, map));
    }

    SECTION("Unserializing Faction")
    {
        const auto faction{unserializer.unserializeFaction(jdoc.toJson(), map)};

        REQUIRE(faction->getId() == jobj["id"].toInt());
        REQUIRE(faction->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(faction->getPrimaryColor().name() == jobj["primaryColor"].toString());
        REQUIRE(faction->getSecondaryColor().name() == jobj["secondaryColor"].toString());
        REQUIRE(faction->getBanner() == io::unserializeReference(jobj["banner"].toString(), map));
        REQUIRE(faction->getCivilization() == io::unserializeReference(jobj["civilization"].toString(), map));
    }
}

TEST_CASE("Faction can't be unserialized from JSON", "[CampaignMapJsonUnserializer][JSON][Unserialize][ErrorPaths]")
{
    const auto maps = makeMap();
    const auto map = maps.first.get();
    const auto jmap = maps.second;

    const io::CampaignMapJsonUnserializer unserializer;
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
        jobj.remove("displayName");
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeFaction(jdoc.toJson(), map), utils::ValueError);
    }

    SECTION("Empty name")
    {
        jobj["displayName"] = "";

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

TEST_CASE("Entity can be unserialized from JSON", "[CampaignMapJsonUnserializer][JSON][Unserialize][HappyPath]")
{
    const auto maps = makeMap();
    const auto map = maps.first.get();
    const auto jmap = maps.second;

    const auto jobj = jmap["entities"].toArray()[0].toObject();
    const QJsonDocument jdoc{jobj};
    const QByteArray rawJson{jdoc.toJson()};

    INFO("The json entity is: " << rawJson.data());

    const io::CampaignMapJsonUnserializer unserializer;

    SECTION("Unserialization succeeds without exceptions")
    {
        REQUIRE_NOTHROW(unserializer.unserializeEntity(rawJson, map));
        REQUIRE(unserializer.unserializeEntity(rawJson, map));
    }

    SECTION("Unserializing Entity")
    {
        const auto entity{unserializer.unserializeEntity(jdoc.toJson(), map)};

        REQUIRE(entity->getId() == jobj["id"].toInt());
        REQUIRE(entity->getType() == io::unserializeReference(jobj["type"].toString(), map));

        const auto& components{entity->getComponents()};
        const auto jcomponents{jobj["components"].toObject()};

        REQUIRE(jcomponents.size() == components.size());

        for (auto it = jcomponents.begin(); it != jcomponents.end(); ++it)
        {
            const auto componentType{io::unserializeReferenceAs<core::ComponentType>(it.key(), map)};
            const auto jcomponent{it->toObject()};

            REQUIRE(entity->getComponent(componentType));

            for (auto field : componentType->getFields())
            {
                const auto fieldName{field->getName()};
                const auto val{jcomponent[fieldName]};
                const auto fieldType{field->getType()};

                INFO("The field name is " << fieldName);

                if (fieldType->id() == core::Field::TypeId::Reference)
                {
                    REQUIRE(io::unserializeReference(val.toString(), map) ==
                        entity->getComponent(componentType)->getField(fieldName).value<core::WObject*>());
                }
                else
                {
                    REQUIRE(val.toVariant() == entity->getComponent(componentType)->getField(fieldName));
                }
            }
        }
    }
}

TEST_CASE("Entity can't be unserialized from JSON", "[CampaignMapJsonUnserializer][JSON][Unserialize][ErrorPaths]")
{
    const auto maps = makeMap();
    const auto map = maps.first.get();
    const auto jmap = maps.second;

    const io::CampaignMapJsonUnserializer unserializer;
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

    SECTION("Missing type")
    {
        jobj.remove("type");
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(jdoc.toJson(), map), utils::ValueError);
    }

    SECTION("Invalid type")
    {
        jobj["type"] = 123;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
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

    auto entityType0 = map->getWorld()->getEntityTypes()[0];
    auto componentType1 = entityType0->getComponentTypes()[1];
    auto jcomponents = jobj["components"].toObject();
    auto jcomponent = jcomponents[io::serializeReference(componentType1)].toObject();

    SECTION("Invalid component")
    {
        jcomponents[io::serializeReference(componentType1)] = 123;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("A field is missing")
    {
        jcomponent.remove("intField");
        jcomponents[io::serializeReference(componentType1)] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Integer field is invalid")
    {
        jcomponent["intField"] = "123";
        jcomponents[io::serializeReference(componentType1)] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Integer field is real")
    {
        jcomponent["intField"] = 123.23;
        jcomponents[io::serializeReference(componentType1)] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Real field is invalid")
    {
        jcomponent["realField"] = QJsonArray();
        jcomponents[io::serializeReference(componentType1)] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("String field is invalid")
    {
        jcomponent["strField"] = 123;
        jcomponents[io::serializeReference(componentType1)] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Reference field is invalid")
    {
        jcomponent["refField"] = 123;
        jcomponents[io::serializeReference(componentType1)] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("List field is invalid")
    {
        jcomponent["intsListField"] = 123;
        jcomponents[io::serializeReference(componentType1)] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }

    SECTION("Dictionary field is invalid")
    {
        jcomponent["realDictField"] = 123;
        jcomponents[io::serializeReference(componentType1)] = jcomponent;
        jobj["components"] = jcomponents;

        REQUIRE_THROWS_AS(unserializer.unserializeEntity(QJsonDocument(jobj).toJson(), map), utils::ValueError);
    }
}

TEST_CASE("CampaignMap can be unserialized from JSON", "[CampaignMapJsonUnserializer][JSON][Unserialize][HappyPath]")
{
    const auto maps = makeMap();
    const auto jmap = maps.second;
    auto world = maps.first->getWorld();

    const QJsonDocument jdoc{jmap};
    const QByteArray rawJson{jdoc.toJson()};

    INFO("The json map is: " << rawJson.data());

    const io::CampaignMapJsonUnserializer unserializer;

    SECTION("Unserialization succeeds without exceptions")
    {
        REQUIRE_NOTHROW(unserializer.unserializeCampaignMap(rawJson, world));
        REQUIRE(unserializer.unserializeCampaignMap(rawJson, world));
    }

    SECTION("Unserializing map")
    {
        const auto map = unserializer.unserializeCampaignMap(rawJson, world);

        REQUIRE(map->getDisplayName() == jmap["displayName"].toString());
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

TEST_CASE("CampaignMap can't be unserialized from JSON", "[CampaignMapJsonUnserializer][JSON][Unserialize][ErrorPaths]")
{
    const auto maps = makeMap();
    const auto map = maps.first.get();
    auto world = maps.first->getWorld();

    auto jmap = maps.second;

    const io::CampaignMapJsonUnserializer unserializer;

    SECTION("Invalid JSON")
    {
        QString invalidJson{"{\"id\": \"1\",}"};
        REQUIRE_THROWS_AS(unserializer.unserializeCampaignMap(invalidJson.toLocal8Bit(), world), utils::ValueError);
    }

    SECTION("Missing name")
    {
        jmap.remove("displayName");
        QJsonDocument jdoc(jmap);

        REQUIRE_THROWS_AS(unserializer.unserializeCampaignMap(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Invalid name")
    {
        jmap["displayName"] = 123;
        QJsonDocument jdoc(jmap);

        REQUIRE_THROWS_AS(unserializer.unserializeCampaignMap(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Missing mapNodes")
    {
        jmap.remove("mapNodes");
        QJsonDocument jdoc(jmap);

        REQUIRE_THROWS_AS(unserializer.unserializeCampaignMap(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Invalid mapNodes")
    {
        jmap["mapNodes"] = 123;
        QJsonDocument jdoc(jmap);

        REQUIRE_THROWS_AS(unserializer.unserializeCampaignMap(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Missing factions")
    {
        jmap.remove("factions");
        QJsonDocument jdoc(jmap);

        REQUIRE_THROWS_AS(unserializer.unserializeCampaignMap(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Invalid factions")
    {
        jmap["factions"] = 123;
        QJsonDocument jdoc(jmap);

        REQUIRE_THROWS_AS(unserializer.unserializeCampaignMap(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Missing entities")
    {
        jmap.remove("entities");
        QJsonDocument jdoc(jmap);

        REQUIRE_THROWS_AS(unserializer.unserializeCampaignMap(jdoc.toJson(), world), utils::ValueError);
    }

    SECTION("Invalid entities")
    {
        jmap["entities"] = 123;
        QJsonDocument jdoc(jmap);

        REQUIRE_THROWS_AS(unserializer.unserializeCampaignMap(jdoc.toJson(), world), utils::ValueError);
    }
}
