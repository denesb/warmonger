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

#include <memory>
#include <tuple>

#include <QJsonDocument>

#include "io/JsonSerializer.h"
#include "test/Util.h"
#include "test/catch.hpp"

using namespace warmonger;

TEST_CASE("Banner can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    core::Banner* b = world->getBanners()[0];

    SECTION("serializing Banner")
    {
        io::JsonSerializer serializer;
        const QByteArray json(serializer.serializeBanner(b));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == b->objectName());
        REQUIRE(jobj["displayName"].toString() == b->getDisplayName());
        REQUIRE(jobj["civilizations"].isArray());
        arrayEqualsList(jobj["civilizations"].toArray(), b->getCivilizations());
    }
}

TEST_CASE("CampaignMap can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::CampaignMap*, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> m{maps.first};
    const QJsonObject jobj{maps.second};
    const std::unique_ptr<core::World> world{m->getWorld()};

    SECTION("serializing CampaignMap")
    {
        io::JsonSerializer serializer;
        const QByteArray json(serializer.serializeCampaignMap(m.get()));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == m->objectName());
        REQUIRE(jobj["displayName"].toString() == m->getDisplayName());
        REQUIRE(jobj["world"].toString() == m->getWorld()->objectName());

        REQUIRE(jobj["mapNodes"].isArray() == true);
        REQUIRE(m->getMapNodes().size() == jobj["mapNodes"].toArray().size());

        REQUIRE(jobj["factions"].isArray() == true);
        REQUIRE(m->getFactions().size() == jobj["factions"].toArray().size());
    }
}

TEST_CASE("Civilization can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    core::Civilization* f = world->getCivilizations()[0];

    SECTION("serializing Civilization")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeCivilization(f));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == f->objectName());
        REQUIRE(jobj["displayName"].toString() == f->getDisplayName());
        REQUIRE(jobj["unitTypes"].isArray() == true);
        arrayEqualsList(jobj["unitTypes"].toArray(), f->getUnitTypes());
    }
}

TEST_CASE("MapNode can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::CampaignMap*, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jobj{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const core::MapNode* mn = map->getMapNodes()[0];

    SECTION("serializing MapNode")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeMapNode(mn));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"] == mn->objectName());
        REQUIRE(jobj["displayName"] == mn->getDisplayName());
        REQUIRE(jobj["terrainType"] == mn->getTerrainType()->objectName());
        REQUIRE(jobj["neighbours"].isObject());

        const QJsonObject jneighbours = jobj["neighbours"].toObject();
        const core::MapNodeNeighbours& neighbours = mn->getNeighbours();

        for (auto it = jneighbours.constBegin(); it != jneighbours.constEnd(); it++)
        {
            REQUIRE_NOTHROW(core::str2direction(it.key()));
            REQUIRE(it.value().isString());

            const core::Direction dir{core::str2direction(it.key())};
            const QString neighbourName{it.value().toString()};

            REQUIRE(neighbourName.isEmpty() == (neighbours.at(dir) == nullptr));

            if (!neighbourName.isEmpty())
            {
                REQUIRE(neighbourName == neighbours.at(dir)->objectName());
            }
        }
    }
}

TEST_CASE("Faction can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::CampaignMap*, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jobj{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const core::Faction* f = map->getFactions()[0];

    SECTION("serializing Faction")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeFaction(f));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == f->objectName());
        REQUIRE(jobj["displayName"].toString() == f->getDisplayName());
        REQUIRE(jobj["primaryColor"].toString() == f->getPrimaryColor().name());
        REQUIRE(jobj["secondaryColor"].toString() == f->getSecondaryColor().name());
        REQUIRE(jobj["banner"].toString() == f->getBanner()->objectName());
        REQUIRE(jobj["civilization"].toString() == f->getCivilization()->objectName());
    }
}

TEST_CASE("World can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    SECTION("serializing World")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeWorld(world.get()));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == world->objectName());
        REQUIRE(jobj["displayName"].toString() == world->getDisplayName());

        REQUIRE(jobj["banners"].isArray() == true);
        REQUIRE(world->getBanners().size() == jobj["banners"].toArray().size());

        REQUIRE(jobj["civilizations"].isArray() == true);
        REQUIRE(world->getCivilizations().size() == jobj["civilizations"].toArray().size());

        REQUIRE(jobj["colors"].isArray() == true);
        REQUIRE(world->getColors().size() == jobj["colors"].toArray().size());

        const std::vector<QColor>& colors = world->getColors();
        QJsonArray jcolors = jobj["colors"].toArray();
        for (std::size_t i = 0; i < colors.size(); ++i)
        {
            REQUIRE(colors[i].name() == jcolors[i].toString());
        }

        REQUIRE(jobj["terrainTypes"].isArray() == true);
        REQUIRE(world->getTerrainTypes().size() == jobj["terrainTypes"].toArray().size());

        REQUIRE(jobj["unitTypes"].isArray() == true);
        REQUIRE(world->getUnitTypes().size() == jobj["unitTypes"].toArray().size());

        REQUIRE(jobj["settlementTypes"].isArray() == true);
        REQUIRE(world->getSettlementTypes().size() == jobj["settlementTypes"].toArray().size());
    }
}
