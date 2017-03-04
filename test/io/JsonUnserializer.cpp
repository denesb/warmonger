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
#include <memory>

#include <QJsonDocument>
#include <QJsonObject>

#include "io/Context.h"
#include "io/JsonUnserializer.h"
#include "test/Util.h"
#include "test/catch.hpp"
#include "utils/Exception.h"

using namespace warmonger;

CATCH_TRANSLATE_EXCEPTION(utils::ValueError& e)
{
    return e.getMessage().toStdString();
}

static const QByteArray invalidJson{"{\"displayName\": \"displayName\" \"objectName\": \"objectName\"}"};

TEST_CASE("Banner can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const QJsonObject jobj = jworld["banners"].toArray()[0].toObject();

    const std::vector<core::Civilization*> cs = world->getCivilizations();
    std::for_each(cs.cbegin(), cs.cend(), [&](core::Civilization* o) { ctx.add(o); });

    SECTION("unserializing Banner")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Banner> b(unserializer.unserializeBanner(jdoc.toJson()));

        REQUIRE(b->objectName() == jobj["objectName"].toString());
        REQUIRE(b->getDisplayName() == jobj["displayName"].toString());
        if (jobj.contains("civilizations"))
        {
            arrayEqualsList(jobj["civilizations"].toArray(), b->getCivilizations());
        }
        else
        {
            REQUIRE(b->getCivilizations().empty());
        }
    }
}

TEST_CASE("Banner can't be unserialized from JSON", "[JsonUnserializer]")
{
    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(unserializer.unserializeBanner(invalidJson), utils::ValueError);
    }

    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const QJsonObject jobj = jworld["banners"].toArray()[0].toObject();

    SECTION("unserializing Banner, no civilizations")
    {
        io::Context ctx;

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeBanner(jdoc.toJson()), utils::ValueError);
    }
}

TEST_CASE("CampaignMap can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::CampaignMap*, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> obj{maps.first};
    const QJsonObject jobj{maps.second};
    const std::unique_ptr<core::World> world{obj->getWorld()};

    io::addWorldToContext(ctx, world.get());

    SECTION("unserializing CampaignMap")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::CampaignMap> m(unserializer.unserializeCampaignMap(jdoc.toJson()));

        REQUIRE(m->objectName() == jobj["objectName"].toString());
        REQUIRE(m->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(m->getWorld()->objectName() == jobj["world"].toString());

        const std::vector<core::MapNode*> mns{m->getMapNodes()};
        const QJsonArray jmns(jobj["mapNodes"].toArray());

        REQUIRE(jmns.size() == mns.size());

        for (size_t i = 0; i < mns.size(); i++)
        {
            const core::MapNode* mn{mns[i]};
            const QJsonObject jmn(jmns[i].toObject());

            const QJsonObject jneighbours = jmn["neighbours"].toObject();
            const core::MapNodeNeighbours& neighbours = mn->getNeighbours();

            for (const auto& neighbour : neighbours)
            {
                const QString dirName{core::direction2str(neighbour.first)};
                REQUIRE(jneighbours.contains(dirName));

                REQUIRE((neighbour.second == nullptr) == jneighbours[dirName].toString().isEmpty());

                if (neighbour.second != nullptr)
                {
                    REQUIRE(neighbour.second->objectName() == jneighbours[dirName].toString());
                }
            }
        }

        REQUIRE(m->getFactions().size() == jobj["factions"].toArray().size());
    }
}

TEST_CASE("CampaignMap can't be unserialized from JSON", "[JsonUnserializer]")
{
    const std::pair<core::CampaignMap*, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> obj{maps.first};
    const QJsonObject jobj{maps.second};
    const std::unique_ptr<core::World> world{obj->getWorld()};

    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(unserializer.unserializeCampaignMap(invalidJson), utils::ValueError);
    }

    SECTION("unserializing CampaignMap, no world")
    {
        io::Context ctx;

        const std::vector<core::Banner*> bs = world->getBanners();
        std::for_each(bs.cbegin(), bs.cend(), [&](core::Banner* o) { ctx.add(o); });

        const std::vector<core::Civilization*> fs = world->getCivilizations();
        std::for_each(fs.cbegin(), fs.cend(), [&](core::Civilization* o) { ctx.add(o); });

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeCampaignMap(jdoc.toJson()), utils::ValueError);
    }

    SECTION("unserializing CampaignMap, no banners")
    {
        io::Context ctx;

        ctx.add(world.get());

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeCampaignMap(jdoc.toJson()), utils::ValueError);
    }

    SECTION("unserializing CampaignMap, no civilizations")
    {
        io::Context ctx;

        ctx.add(world.get());

        const std::vector<core::Banner*> bs = world->getBanners();
        std::for_each(bs.cbegin(), bs.cend(), [&](core::Banner* o) { ctx.add(o); });

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeCampaignMap(jdoc.toJson()), utils::ValueError);
    }
}

TEST_CASE("Civilization can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const std::vector<core::UnitType*> uts = world->getUnitTypes();
    std::for_each(uts.cbegin(), uts.cend(), [&](core::UnitType* o) { ctx.add(o); });
    const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
    std::for_each(sts.cbegin(), sts.cend(), [&](core::SettlementType* o) { ctx.add(o); });

    const QJsonObject jobj = jworld["civilizations"].toArray()[0].toObject();

    SECTION("unserializing Civilization")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Civilization> f(unserializer.unserializeCivilization(jdoc.toJson()));

        REQUIRE(f->objectName() == jobj["objectName"].toString());
        REQUIRE(f->getDisplayName() == jobj["displayName"].toString());
        arrayEqualsList(jobj["unitTypes"].toArray(), f->getUnitTypes());
    }
}

TEST_CASE("Civilization can't be unserialized from JSON", "[JsonUnserializer]")
{
    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const QJsonObject jobj = jworld["civilizations"].toArray()[0].toObject();

    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(unserializer.unserializeCivilization(invalidJson), utils::ValueError);
    }

    SECTION("unserializing Civilization, no unitTypes")
    {
        io::Context ctx;

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeCivilization(jdoc.toJson()), utils::ValueError);
    }
}

TEST_CASE("MapNode can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::CampaignMap*, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jmap{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const std::vector<core::TerrainType*> tts = world->getTerrainTypes();
    std::for_each(tts.cbegin(), tts.cend(), [&](core::TerrainType* o) { ctx.add(o); });

    const QJsonObject jobj = jmap["mapNodes"].toArray()[0].toObject();

    SECTION("unserializing MapNode")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::MapNode> mn(unserializer.unserializeMapNode(jdoc.toJson()));

        REQUIRE(mn->objectName() == jobj["objectName"].toString());
        REQUIRE(mn->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(mn->getTerrainType()->objectName() == jobj["terrainType"].toString());
    }
}

TEST_CASE("MapNode can't be unserialized from JSON", "[JsonUnserializer]")
{
    const std::pair<core::CampaignMap*, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jmap{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const QJsonObject jobj = jmap["mapNodes"].toArray()[0].toObject();

    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(unserializer.unserializeMapNode(invalidJson), utils::ValueError);
    }

    SECTION("unserializing MapNode, no terrainTypes")
    {
        io::Context ctx;

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeMapNode(jdoc.toJson()), utils::ValueError);
    }
}

TEST_CASE("Faction can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::CampaignMap*, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jmap{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const std::vector<core::Banner*> b = world->getBanners();
    std::for_each(b.cbegin(), b.cend(), [&](core::Banner* o) { ctx.add(o); });

    const std::vector<core::Civilization*> fs = world->getCivilizations();
    std::for_each(fs.cbegin(), fs.cend(), [&](core::Civilization* o) { ctx.add(o); });

    const QJsonObject jobj = jmap["factions"].toArray()[0].toObject();

    SECTION("unserializing Faction")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Faction> p(unserializer.unserializeFaction(jdoc.toJson()));

        REQUIRE(p->objectName() == jobj["objectName"].toString());
        REQUIRE(p->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(p->getPrimaryColor().name() == jobj["primaryColor"].toString());
        REQUIRE(p->getSecondaryColor().name() == jobj["secondaryColor"].toString());
        REQUIRE(p->getBanner()->objectName() == jobj["banner"].toString());
        REQUIRE(p->getCivilization()->objectName() == jobj["civilization"].toString());
    }
}

TEST_CASE("Faction can't be unserialized from JSON", "[JsonUnserializer]")
{
    const std::pair<core::CampaignMap*, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jmap{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const QJsonObject jobj = jmap["factions"].toArray()[0].toObject();

    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(unserializer.unserializeFaction(invalidJson), utils::ValueError);
    }

    SECTION("unserializing Faction, no Banners")
    {
        io::Context ctx;

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        const std::vector<core::Civilization*> fs = world->getCivilizations();
        std::for_each(fs.cbegin(), fs.cend(), [&](core::Civilization* o) { ctx.add(o); });

        REQUIRE_THROWS_AS(unserializer.unserializeFaction(jdoc.toJson()), utils::ValueError);
    }

    SECTION("unserializing Faction, no Civilizations")
    {
        io::Context ctx;

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        const std::vector<core::Banner*> b = world->getBanners();
        std::for_each(b.cbegin(), b.cend(), [&](core::Banner* o) { ctx.add(o); });

        REQUIRE_THROWS_AS(unserializer.unserializeFaction(jdoc.toJson()), utils::ValueError);
    }
}

TEST_CASE("World can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> obj{worlds.first};
    const QJsonObject jobj{worlds.second};

    SECTION("unserializing World")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::World> world(unserializer.unserializeWorld(jdoc.toJson()));

        REQUIRE(world->objectName() == jobj["objectName"].toString());
        REQUIRE(world->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(world->getBanners().size() == jobj["banners"].toArray().size());
        REQUIRE(world->getCivilizations().size() == jobj["civilizations"].toArray().size());
        REQUIRE(world->getColors().size() == jobj["colors"].toArray().size());

        const std::vector<QColor>& colors = world->getColors();
        QJsonArray jcolors = jobj["colors"].toArray();
        for (std::size_t i = 0; i < colors.size(); ++i)
        {
            REQUIRE(colors[i].name() == jcolors[i].toString());
        }
    }
}

TEST_CASE("World can't be unserialized from JSON", "[JsonUnserializer]")
{
    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(unserializer.unserializeWorld(invalidJson), utils::ValueError);
    }
}
