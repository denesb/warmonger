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

TEST_CASE("Army can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::CampaignMap*, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jmap{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const std::vector<core::ArmyType*> ats = world->getArmyTypes();
    std::for_each(ats.cbegin(), ats.cend(), [&](core::ArmyType* o) { ctx.add(o); });

    const std::vector<core::Faction*> fs = map->getFactions();
    std::for_each(fs.cbegin(), fs.cend(), [&](core::Faction* o) { ctx.add(o); });

    const std::vector<core::MapNode*> mns = map->getMapNodes();
    std::for_each(mns.cbegin(), mns.cend(), [&](core::MapNode* o) { ctx.add(o); });

    const std::vector<core::Unit*> us = map->getUnits();
    std::for_each(us.cbegin(), us.cend(), [&](core::Unit* o) { ctx.add(o); });

    const QJsonObject jobj = jmap["armies"].toArray()[0].toObject();

    SECTION("unserializing Army")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Army> a(unserializer.unserializeArmy(jdoc.toJson()));

        REQUIRE(a->objectName() == jobj["objectName"].toString());
        REQUIRE(a->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(a->getType()->objectName() == jobj["type"].toString());
        REQUIRE(a->getMapNode()->objectName() == jobj["mapNode"].toString());
        REQUIRE(a->getOwner()->objectName() == jobj["owner"].toString());
        arrayEqualsList(jobj["units"].toArray(), a->getUnits());
    }
}

TEST_CASE("Army can't be unserialized from JSON", "[JsonUnserializer]")
{
    const std::pair<core::CampaignMap*, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jmap{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const QJsonObject jobj = jmap["armies"].toArray()[0].toObject();

    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(unserializer.unserializeArmy(invalidJson), utils::ValueError);
    }

    SECTION("unserializing Army, no armyTypes")
    {
        io::Context ctx;

        const std::vector<core::Unit*> us = map->getUnits();
        std::for_each(us.cbegin(), us.cend(), [&](core::Unit* o) { ctx.add(o); });

        const std::vector<core::MapNode*> mns = map->getMapNodes();
        std::for_each(mns.cbegin(), mns.cend(), [&](core::MapNode* o) { ctx.add(o); });

        const std::vector<core::Faction*> fs = map->getFactions();
        std::for_each(fs.cbegin(), fs.cend(), [&](core::Faction* o) { ctx.add(o); });

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeArmy(jdoc.toJson()), utils::ValueError);
    }

    SECTION("unserializing Army, no units")
    {
        io::Context ctx;

        const std::vector<core::ArmyType*> ats = world->getArmyTypes();
        std::for_each(ats.cbegin(), ats.cend(), [&](core::ArmyType* o) { ctx.add(o); });

        const std::vector<core::MapNode*> mns = map->getMapNodes();
        std::for_each(mns.cbegin(), mns.cend(), [&](core::MapNode* o) { ctx.add(o); });

        const std::vector<core::Faction*> fs = map->getFactions();
        std::for_each(fs.cbegin(), fs.cend(), [&](core::Faction* o) { ctx.add(o); });

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeArmy(jdoc.toJson()), utils::ValueError);
    }

    SECTION("unserializing Army, no mapNodes")
    {
        io::Context ctx;

        const std::vector<core::ArmyType*> ats = world->getArmyTypes();
        std::for_each(ats.cbegin(), ats.cend(), [&](core::ArmyType* o) { ctx.add(o); });

        const std::vector<core::Unit*> us = map->getUnits();
        std::for_each(us.cbegin(), us.cend(), [&](core::Unit* o) { ctx.add(o); });

        const std::vector<core::Faction*> fs = map->getFactions();
        std::for_each(fs.cbegin(), fs.cend(), [&](core::Faction* o) { ctx.add(o); });

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeArmy(jdoc.toJson()), utils::ValueError);
    }

    SECTION("unserializing Army, no factions")
    {
        io::Context ctx;

        const std::vector<core::ArmyType*> ats = world->getArmyTypes();
        std::for_each(ats.cbegin(), ats.cend(), [&](core::ArmyType* o) { ctx.add(o); });

        const std::vector<core::Unit*> us = map->getUnits();
        std::for_each(us.cbegin(), us.cend(), [&](core::Unit* o) { ctx.add(o); });

        const std::vector<core::MapNode*> mns = map->getMapNodes();
        std::for_each(mns.cbegin(), mns.cend(), [&](core::MapNode* o) { ctx.add(o); });

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeArmy(jdoc.toJson()), utils::ValueError);
    }
}

TEST_CASE("ArmyType can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const QJsonObject jobj = jworld["armyTypes"].toArray()[0].toObject();

    SECTION("unserializing ArmyType")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::ArmyType> at(unserializer.unserializeArmyType(jdoc.toJson()));

        REQUIRE(at->isHierarchyRoot());
        REQUIRE(at->objectName() == jobj["objectName"].toString());
        REQUIRE(at->getDisplayName() == jobj["displayName"].toString());
    }

    const std::vector<core::ArmyType*> ats = world->getArmyTypes();
    std::for_each(ats.cbegin(), ats.cend(), [&](core::ArmyType* o) { ctx.add(o); });

    SECTION("unserializing ArmyType - all properties inherited")
    {
        const QJsonObject parentJobj = jworld["armyTypes"].toArray()[0].toObject();
        QJsonObject jobj;
        jobj["objectName"] = "childAt0";
        jobj["hierarchyParent"] = parentJobj["objectName"];

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::ArmyType> at(unserializer.unserializeArmyType(jdoc.toJson()));

        REQUIRE(!at->isHierarchyRoot());
        REQUIRE(at->getHierarchyParent()->objectName() == parentJobj["objectName"].toString());
        REQUIRE(at->objectName() == jobj["objectName"].toString());
        REQUIRE(at->getDisplayName() == parentJobj["displayName"].toString());
    }
}

TEST_CASE("ArmyType can't be unserialized from JSON", "[JsonUnserializer]")
{
    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(unserializer.unserializeArmyType(invalidJson), utils::ValueError);
    }

    SECTION("unserializing ArmyType, no parent armyTypes")
    {
        QJsonObject jobj;
        jobj["objectName"] = "armyTypeChild0";
        jobj["hierarchyParent"] = "nonExistentArmyType0";

        io::Context ctx;

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeArmyType(jdoc.toJson()), utils::ValueError);
    }
}

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

    ctx.add(world.get());

    const std::vector<core::TerrainType*> tts = world->getTerrainTypes();
    std::for_each(tts.cbegin(), tts.cend(), [&](core::TerrainType* o) { ctx.add(o); });

    const std::vector<core::Banner*> bs = world->getBanners();
    std::for_each(bs.cbegin(), bs.cend(), [&](core::Banner* o) { ctx.add(o); });

    const std::vector<core::Civilization*> cs = world->getCivilizations();
    std::for_each(cs.cbegin(), cs.cend(), [&](core::Civilization* o) { ctx.add(o); });

    const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
    std::for_each(sts.cbegin(), sts.cend(), [&](core::SettlementType* o) { ctx.add(o); });

    const std::vector<core::UnitType*> uts = world->getUnitTypes();
    std::for_each(uts.cbegin(), uts.cend(), [&](core::UnitType* o) { ctx.add(o); });

    const std::vector<core::ArmyType*> ats = world->getArmyTypes();
    std::for_each(ats.cbegin(), ats.cend(), [&](core::ArmyType* o) { ctx.add(o); });

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
        REQUIRE(m->getSettlements().size() == jobj["settlements"].toArray().size());
        REQUIRE(m->getSettlements().size() == jobj["units"].toArray().size());
        REQUIRE(m->getArmies().size() == jobj["armies"].toArray().size());
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

        const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
        std::for_each(sts.cbegin(), sts.cend(), [&](core::SettlementType* o) { ctx.add(o); });

        const std::vector<core::TerrainType*> tts = world->getTerrainTypes();
        std::for_each(tts.cbegin(), tts.cend(), [&](core::TerrainType* o) { ctx.add(o); });

        const std::vector<core::UnitType*> uts = world->getUnitTypes();
        std::for_each(uts.cbegin(), uts.cend(), [&](core::UnitType* o) { ctx.add(o); });

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeCampaignMap(jdoc.toJson()), utils::ValueError);
    }

    SECTION("unserializing Map, no terrainTypes")
    {
        io::Context ctx;

        ctx.add(world.get());

        const std::vector<core::Banner*> bs = world->getBanners();
        std::for_each(bs.cbegin(), bs.cend(), [&](core::Banner* o) { ctx.add(o); });

        const std::vector<core::Civilization*> fs = world->getCivilizations();
        std::for_each(fs.cbegin(), fs.cend(), [&](core::Civilization* o) { ctx.add(o); });

        const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
        std::for_each(sts.cbegin(), sts.cend(), [&](core::SettlementType* o) { ctx.add(o); });

        const std::vector<core::UnitType*> uts = world->getUnitTypes();
        std::for_each(uts.cbegin(), uts.cend(), [&](core::UnitType* o) { ctx.add(o); });

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeCampaignMap(jdoc.toJson()), utils::ValueError);
    }

    SECTION("unserializing Map, no banners")
    {
        io::Context ctx;

        ctx.add(world.get());

        const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
        std::for_each(sts.cbegin(), sts.cend(), [&](core::SettlementType* o) { ctx.add(o); });

        const std::vector<core::TerrainType*> tts = world->getTerrainTypes();
        std::for_each(tts.cbegin(), tts.cend(), [&](core::TerrainType* o) { ctx.add(o); });

        const std::vector<core::UnitType*> uts = world->getUnitTypes();
        std::for_each(uts.cbegin(), uts.cend(), [&](core::UnitType* o) { ctx.add(o); });

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeCampaignMap(jdoc.toJson()), utils::ValueError);
    }

    SECTION("unserializing Map, no civilizations")
    {
        io::Context ctx;

        ctx.add(world.get());

        const std::vector<core::Banner*> bs = world->getBanners();
        std::for_each(bs.cbegin(), bs.cend(), [&](core::Banner* o) { ctx.add(o); });

        const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
        std::for_each(sts.cbegin(), sts.cend(), [&](core::SettlementType* o) { ctx.add(o); });

        const std::vector<core::TerrainType*> tts = world->getTerrainTypes();
        std::for_each(tts.cbegin(), tts.cend(), [&](core::TerrainType* o) { ctx.add(o); });

        const std::vector<core::UnitType*> uts = world->getUnitTypes();
        std::for_each(uts.cbegin(), uts.cend(), [&](core::UnitType* o) { ctx.add(o); });

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeCampaignMap(jdoc.toJson()), utils::ValueError);
    }

    SECTION("unserializing Map, no settlementTypes")
    {
        io::Context ctx;

        ctx.add(world.get());

        const std::vector<core::Banner*> bs = world->getBanners();
        std::for_each(bs.cbegin(), bs.cend(), [&](core::Banner* o) { ctx.add(o); });

        const std::vector<core::Civilization*> cs = world->getCivilizations();
        std::for_each(cs.cbegin(), cs.cend(), [&](core::Civilization* o) { ctx.add(o); });

        const std::vector<core::TerrainType*> tts = world->getTerrainTypes();
        std::for_each(tts.cbegin(), tts.cend(), [&](core::TerrainType* o) { ctx.add(o); });

        const std::vector<core::UnitType*> uts = world->getUnitTypes();
        std::for_each(uts.cbegin(), uts.cend(), [&](core::UnitType* o) { ctx.add(o); });

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeCampaignMap(jdoc.toJson()), utils::ValueError);
    }

    SECTION("unserializing Map, no unitTypes")
    {
        io::Context ctx;

        ctx.add(world.get());

        const std::vector<core::TerrainType*> tts = world->getTerrainTypes();
        std::for_each(tts.cbegin(), tts.cend(), [&](core::TerrainType* o) { ctx.add(o); });
        const std::vector<core::Civilization*> fs = world->getCivilizations();
        std::for_each(fs.cbegin(), fs.cend(), [&](core::Civilization* o) { ctx.add(o); });
        const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
        std::for_each(sts.cbegin(), sts.cend(), [&](core::SettlementType* o) { ctx.add(o); });

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

TEST_CASE("Settlement can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::CampaignMap*, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jmap{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
    std::for_each(sts.cbegin(), sts.cend(), [&](core::SettlementType* o) { ctx.add(o); });

    const std::vector<core::MapNode*> mns = map->getMapNodes();
    std::for_each(mns.cbegin(), mns.cend(), [&](core::MapNode* o) { ctx.add(o); });

    const std::vector<core::Faction*> fs = map->getFactions();
    std::for_each(fs.cbegin(), fs.cend(), [&](core::Faction* o) { ctx.add(o); });

    const QJsonObject jobj = jmap["settlements"].toArray()[0].toObject();

    SECTION("unserializing Settlement")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Settlement> s(unserializer.unserializeSettlement(jdoc.toJson()));

        REQUIRE(s->objectName() == jobj["objectName"].toString());
        REQUIRE(s->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(s->getType()->objectName() == jobj["type"].toString());
        REQUIRE(s->getMapNode()->objectName() == jobj["mapNode"].toString());
        REQUIRE(s->getOwner()->objectName() == jobj["owner"].toString());
    }
}

TEST_CASE("Settlement can't be unserialized from JSON", "[JsonUnserializer]")
{
    const std::pair<core::CampaignMap*, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jmap{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const QJsonObject jobj = jmap["settlements"].toArray()[0].toObject();

    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(unserializer.unserializeSettlement(invalidJson), utils::ValueError);
    }

    SECTION("unserializing Settlement, no settlementTypes")
    {
        io::Context ctx;

        const std::vector<core::MapNode*> mns = map->getMapNodes();
        std::for_each(mns.cbegin(), mns.cend(), [&](core::MapNode* o) { ctx.add(o); });

        const std::vector<core::Faction*> fs = map->getFactions();
        std::for_each(fs.cbegin(), fs.cend(), [&](core::Faction* o) { ctx.add(o); });

        QJsonDocument jdoc(jobj);
        io::JsonUnserializer unserializer(ctx);
        REQUIRE_THROWS_AS(unserializer.unserializeSettlement(jdoc.toJson()), utils::ValueError);
    }

    SECTION("unserializing Settlement, no mapNodes")
    {
        io::Context ctx;

        const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
        std::for_each(sts.cbegin(), sts.cend(), [&](core::SettlementType* o) { ctx.add(o); });

        const std::vector<core::Faction*> fs = map->getFactions();
        std::for_each(fs.cbegin(), fs.cend(), [&](core::Faction* o) { ctx.add(o); });

        QJsonDocument jdoc(jobj);
        io::JsonUnserializer unserializer(ctx);
        REQUIRE_THROWS_AS(unserializer.unserializeSettlement(jdoc.toJson()), utils::ValueError);
    }

    SECTION("unserializing Settlement, no palyers")
    {
        io::Context ctx;

        const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
        std::for_each(sts.cbegin(), sts.cend(), [&](core::SettlementType* o) { ctx.add(o); });

        const std::vector<core::MapNode*> mns = map->getMapNodes();
        std::for_each(mns.cbegin(), mns.cend(), [&](core::MapNode* o) { ctx.add(o); });

        QJsonDocument jdoc(jobj);
        io::JsonUnserializer unserializer(ctx);
        REQUIRE_THROWS_AS(unserializer.unserializeSettlement(jdoc.toJson()), utils::ValueError);
    }
}

TEST_CASE("SettlementType can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    SECTION("unserializing SettlementType")
    {
        const QJsonObject jobj = jworld["settlementTypes"].toArray()[0].toObject();

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::SettlementType> st(unserializer.unserializeSettlementType(jdoc.toJson()));

        REQUIRE(st->isHierarchyRoot());
        REQUIRE(st->objectName() == jobj["objectName"].toString());
        REQUIRE(st->getDisplayName() == jobj["displayName"].toString());
    }

    const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
    std::for_each(sts.cbegin(), sts.cend(), [&](core::SettlementType* o) { ctx.add(o); });

    SECTION("unserializing SettlementType - all properties all inherited")
    {
        const QJsonObject parentJobj = jworld["settlementTypes"].toArray()[0].toObject();
        QJsonObject jobj;
        jobj["objectName"] = "childSt0";
        jobj["hierarchyParent"] = parentJobj["objectName"];

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::SettlementType> st(unserializer.unserializeSettlementType(jdoc.toJson()));

        REQUIRE(!st->isHierarchyRoot());
        REQUIRE(st->getHierarchyParent()->objectName() == parentJobj["objectName"].toString());
        REQUIRE(st->objectName() == jobj["objectName"].toString());
        REQUIRE(st->getDisplayName() == parentJobj["displayName"].toString());
    }
}

TEST_CASE("SettlementType can't be unserialized from JSON", "[JsonUnserializer]")
{
    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const QJsonObject jobj = jworld["settlementTypes"].toArray()[0].toObject();

    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(unserializer.unserializeSettlementType(invalidJson), utils::ValueError);
    }

    SECTION("unserializing SettlementType, no parent terrainTypes")
    {
        QJsonObject jobj;
        jobj["objectName"] = "terrainTypeChild0";
        jobj["hierarchyParent"] = "nonExistentSettlementType0";

        io::Context ctx;

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeSettlementType(jdoc.toJson()), utils::ValueError);
    }
}

TEST_CASE("TerrainType can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const QJsonObject jobj = jworld["terrainTypes"].toArray()[0].toObject();

    SECTION("unserializing TerrainType")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::TerrainType> tt(unserializer.unserializeTerrainType(jdoc.toJson()));

        REQUIRE(tt->objectName() == jobj["objectName"].toString());
        REQUIRE(tt->getDisplayName() == jobj["displayName"].toString());
    }

    const std::vector<core::TerrainType*> tts = world->getTerrainTypes();
    std::for_each(tts.cbegin(), tts.cend(), [&](core::TerrainType* o) { ctx.add(o); });

    SECTION("unserializing TerrainType - all properties inherited")
    {
        const QJsonObject parentJobj = jworld["terrainTypes"].toArray()[0].toObject();
        QJsonObject jobj;
        jobj["objectName"] = "childTt0";
        jobj["hierarchyParent"] = parentJobj["objectName"];

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::TerrainType> tt(unserializer.unserializeTerrainType(jdoc.toJson()));

        REQUIRE(!tt->isHierarchyRoot());
        REQUIRE(tt->getHierarchyParent()->objectName() == parentJobj["objectName"].toString());
        REQUIRE(tt->objectName() == jobj["objectName"].toString());
        REQUIRE(tt->getDisplayName() == parentJobj["displayName"].toString());
    }
}

TEST_CASE("TerrainType can't be unserialized from JSON", "[JsonUnserializer]")
{
    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(unserializer.unserializeTerrainType(invalidJson), utils::ValueError);
    }

    SECTION("unserializing TerrainType, no parent terrainTypes")
    {
        QJsonObject jobj;
        jobj["objectName"] = "terrainTypeChild0";
        jobj["hierarchyParent"] = "nonExistentTerrainType0";

        io::Context ctx;

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeTerrainType(jdoc.toJson()), utils::ValueError);
    }
}

TEST_CASE("Unit can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::CampaignMap*, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jmap{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const std::vector<core::UnitType*> uts = world->getUnitTypes();
    std::for_each(uts.cbegin(), uts.cend(), [&](core::UnitType* o) { ctx.add(o); });

    const std::vector<core::MapNode*> mns = map->getMapNodes();
    std::for_each(mns.cbegin(), mns.cend(), [&](core::MapNode* o) { ctx.add(o); });

    const std::vector<core::Faction*> fs = map->getFactions();
    std::for_each(fs.cbegin(), fs.cend(), [&](core::Faction* o) { ctx.add(o); });

    const QJsonObject jobj = jmap["units"].toArray()[0].toObject();

    SECTION("unserializing Unit")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Unit> u(unserializer.unserializeUnit(jdoc.toJson()));

        REQUIRE(u->objectName() == jobj["objectName"].toString());
        REQUIRE(u->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(u->getType()->objectName() == jobj["type"].toString());
        REQUIRE(u->getMapNode()->objectName() == jobj["mapNode"].toString());
        REQUIRE(u->getOwner()->objectName() == jobj["owner"].toString());
    }
}

TEST_CASE("Unit can't be unserialized from JSON", "[JsonUnserializer]")
{
    const std::pair<core::CampaignMap*, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jmap{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const QJsonObject jobj = jmap["units"].toArray()[0].toObject();

    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(unserializer.unserializeUnit(invalidJson), utils::ValueError);
    }

    SECTION("unserializing Unit, no unitTypes")
    {
        io::Context ctx;

        const std::vector<core::MapNode*> mns = map->getMapNodes();
        std::for_each(mns.cbegin(), mns.cend(), [&](core::MapNode* o) { ctx.add(o); });

        const std::vector<core::Faction*> fs = map->getFactions();
        std::for_each(fs.cbegin(), fs.cend(), [&](core::Faction* o) { ctx.add(o); });

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeUnit(jdoc.toJson()), utils::ValueError);
    }

    SECTION("unserializing Unit, no mapNodes")
    {
        io::Context ctx;

        const std::vector<core::UnitType*> uts = world->getUnitTypes();
        std::for_each(uts.cbegin(), uts.cend(), [&](core::UnitType* o) { ctx.add(o); });

        const std::vector<core::Faction*> fs = map->getFactions();
        std::for_each(fs.cbegin(), fs.cend(), [&](core::Faction* o) { ctx.add(o); });

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeUnit(jdoc.toJson()), utils::ValueError);
    }

    SECTION("unserializing Unit, no factions")
    {
        io::Context ctx;

        const std::vector<core::UnitType*> uts = world->getUnitTypes();
        std::for_each(uts.cbegin(), uts.cend(), [&](core::UnitType* o) { ctx.add(o); });

        const std::vector<core::MapNode*> mns = map->getMapNodes();
        std::for_each(mns.cbegin(), mns.cend(), [&](core::MapNode* o) { ctx.add(o); });

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeUnit(jdoc.toJson()), utils::ValueError);
    }
}

TEST_CASE("UnitType can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const std::vector<core::TerrainType*> tts = world->getTerrainTypes();
    std::for_each(tts.cbegin(), tts.cend(), [&](core::TerrainType* o) { ctx.add(o); });
    const std::vector<core::UnitType*> uts = world->getUnitTypes();
    std::for_each(uts.cbegin(), uts.cend(), [&](core::UnitType* o) { ctx.add(o); });

    SECTION("unserializing UnitType")
    {
        const QJsonObject jobj = jworld["unitTypes"].toArray()[0].toObject();

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::UnitType> ut(unserializer.unserializeUnitType(jdoc.toJson()));

        REQUIRE(ut->isHierarchyRoot());
        REQUIRE(ut->objectName() == jobj["objectName"].toString());
        REQUIRE(ut->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(ut->getHitPoints() == jobj["hitPoints"].toInt());
        REQUIRE(ut->getExperiencePoints() == jobj["experiencePoints"].toInt());
        REQUIRE(ut->getMovementPoints() == jobj["movementPoints"].toInt());
        REQUIRE(ut->getRecruitmentCost() == jobj["recruitmentCost"].toInt());
        REQUIRE(ut->getUpkeepCost() == jobj["upkeepCost"].toInt());
        objectEqualsMap(jobj["movementCosts"].toObject(), ut->getMovementCosts());
        arrayEqualsList(jobj["upgrades"].toArray(), ut->getUpgrades());
    }

    SECTION("unserializing UnitType which inherits all it's properties")
    {
        const QJsonObject parentJobj = jworld["unitTypes"].toArray()[0].toObject();
        QJsonObject jobj;
        jobj["objectName"] = "childUnitType0";
        jobj["hierarchyParent"] = parentJobj["objectName"];

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::UnitType> ut(unserializer.unserializeUnitType(jdoc.toJson()));

        REQUIRE(!ut->isHierarchyRoot());
        REQUIRE(ut->getHierarchyParent()->objectName() == jobj["hierarchyParent"].toString());

        core::UnitType* parent = ut->getHierarchyParent();

        REQUIRE(ut->objectName() == jobj["objectName"].toString());
        REQUIRE(ut->getDisplayName() == parent->getDisplayName());
        REQUIRE(ut->getHitPoints() == parent->getHitPoints());
        REQUIRE(ut->getExperiencePoints() == parent->getExperiencePoints());
        REQUIRE(ut->getMovementPoints() == parent->getMovementPoints());
        REQUIRE(ut->getRecruitmentCost() == parent->getRecruitmentCost());
        REQUIRE(ut->getUpkeepCost() == parent->getUpkeepCost());
        REQUIRE(ut->getMovementCosts() == parent->getMovementCosts());
        REQUIRE(ut->getUpgrades() == parent->getUpgrades());
    }
}

TEST_CASE("UnitType can't be unserialized from JSON", "[JsonUnserializer]")
{
    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const QJsonObject jobj = jworld["unitTypes"].toArray()[1].toObject();

    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(unserializer.unserializeUnitType(invalidJson), utils::ValueError);
    }

    SECTION("unserializing UnitType, no terrainTypes")
    {
        io::Context ctx;

        const std::vector<core::UnitType*> uts = world->getUnitTypes();
        std::for_each(uts.cbegin(), uts.cend(), [&](core::UnitType* o) { ctx.add(o); });

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeUnitType(jdoc.toJson()), utils::ValueError);
    }

    SECTION("unserializing UnitType, no unitTypes")
    {
        io::Context ctx;

        const std::vector<core::TerrainType*> tts = world->getTerrainTypes();
        std::for_each(tts.cbegin(), tts.cend(), [&](core::TerrainType* o) { ctx.add(o); });

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(unserializer.unserializeUnitType(jdoc.toJson()), utils::ValueError);
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
        REQUIRE(world->getArmyTypes().size() == jobj["armyTypes"].toArray().size());
        REQUIRE(world->getBanners().size() == jobj["banners"].toArray().size());
        REQUIRE(world->getTerrainTypes().size() == jobj["terrainTypes"].toArray().size());
        REQUIRE(world->getUnitTypes().size() == jobj["unitTypes"].toArray().size());
        REQUIRE(world->getSettlementTypes().size() == jobj["settlementTypes"].toArray().size());
        REQUIRE(world->getCivilizations().size() == jobj["civilizations"].toArray().size());
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
