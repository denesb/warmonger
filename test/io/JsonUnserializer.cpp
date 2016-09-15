#include <algorithm>
#include <memory>

#include <QJsonDocument>
#include <QJsonObject>

#include "io/Context.h"
#include "io/JsonUnserializer.h"
#include "test/catch.hpp"
#include "test/Util.h"
#include "utils/Exception.h"

using namespace warmonger;

CATCH_TRANSLATE_EXCEPTION(utils::ValueError& e) {
    return e.getMessage().toStdString();
}

static const QByteArray invalidJson{"{\"displayName\": \"displayName\" \"objectName\": \"objectName\"}"};

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
        const std::unique_ptr<core::ArmyType> a(
            unserializer.unserializeArmyType(jdoc.toJson())
        );

        REQUIRE(a->objectName() == jobj["objectName"].toString());
        REQUIRE(a->getDisplayName() == jobj["displayName"].toString());
    }
}

TEST_CASE("ArmyType can't be unserialized from JSON", "[JsonUnserializer]")
{
    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(
            unserializer.unserializeArmyType(invalidJson),
            utils::ValueError
        );
    }
}

TEST_CASE("Army can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::CampaignMap*, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jmap{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const std::vector<core::ArmyType*> ats = world->getArmyTypes();
    std::for_each(
        ats.cbegin(),
        ats.cend(),
        [&](core::ArmyType* o){ctx.add(o);}
    );

    const std::vector<core::Faction*> fs = map->getFactions();
    std::for_each(
        fs.cbegin(),
        fs.cend(),
        [&](core::Faction* o){ctx.add(o);}
    );

    const std::vector<core::MapNode*> mns = map->getMapNodes();
    std::for_each(
        mns.cbegin(),
        mns.cend(),
        [&](core::MapNode* o){ctx.add(o);}
    );

    const std::vector<core::Unit*> us = map->getUnits();
    std::for_each(
        us.cbegin(),
        us.cend(),
        [&](core::Unit* o){ctx.add(o);}
    );

    const QJsonObject jobj = jmap["armies"].toArray()[0].toObject();

    SECTION("unserializing Army")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Army> a(
            unserializer.unserializeArmy(jdoc.toJson())
        );

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

        REQUIRE_THROWS_AS(
            unserializer.unserializeArmy(invalidJson),
            utils::ValueError
        );
    }

    SECTION("unserializing Army, no armyTypes")
    {
        io::Context ctx;

        const std::vector<core::Unit*> us = map->getUnits();
        std::for_each(
            us.cbegin(),
            us.cend(),
            [&](core::Unit* o){ctx.add(o);}
        );

        const std::vector<core::MapNode*> mns = map->getMapNodes();
        std::for_each(
            mns.cbegin(),
            mns.cend(),
            [&](core::MapNode* o){ctx.add(o);}
        );

        const std::vector<core::Faction*> fs = map->getFactions();
        std::for_each(
            fs.cbegin(),
            fs.cend(),
            [&](core::Faction* o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeArmy(jdoc.toJson()),
            utils::ValueError
        );
    }

    SECTION("unserializing Army, no units")
    {
        io::Context ctx;

        const std::vector<core::ArmyType*> ats = world->getArmyTypes();
        std::for_each(
            ats.cbegin(),
            ats.cend(),
            [&](core::ArmyType* o){ctx.add(o);}
        );

        const std::vector<core::MapNode*> mns = map->getMapNodes();
        std::for_each(
            mns.cbegin(),
            mns.cend(),
            [&](core::MapNode* o){ctx.add(o);}
        );

        const std::vector<core::Faction*> fs = map->getFactions();
        std::for_each(
            fs.cbegin(),
            fs.cend(),
            [&](core::Faction* o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeArmy(jdoc.toJson()),
            utils::ValueError
        );
    }

    SECTION("unserializing Army, no mapNodes")
    {
        io::Context ctx;

        const std::vector<core::ArmyType*> ats = world->getArmyTypes();
        std::for_each(
            ats.cbegin(),
            ats.cend(),
            [&](core::ArmyType* o){ctx.add(o);}
        );

        const std::vector<core::Unit*> us = map->getUnits();
        std::for_each(
            us.cbegin(),
            us.cend(),
            [&](core::Unit* o){ctx.add(o);}
        );

        const std::vector<core::Faction*> fs = map->getFactions();
        std::for_each(
            fs.cbegin(),
            fs.cend(),
            [&](core::Faction* o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeArmy(jdoc.toJson()),
            utils::ValueError
        );
    }

    SECTION("unserializing Army, no factions")
    {
        io::Context ctx;

        const std::vector<core::ArmyType*> ats = world->getArmyTypes();
        std::for_each(
            ats.cbegin(),
            ats.cend(),
            [&](core::ArmyType* o){ctx.add(o);}
        );

        const std::vector<core::Unit*> us = map->getUnits();
        std::for_each(
            us.cbegin(),
            us.cend(),
            [&](core::Unit* o){ctx.add(o);}
        );

        const std::vector<core::MapNode*> mns = map->getMapNodes();
        std::for_each(
            mns.cbegin(),
            mns.cend(),
            [&](core::MapNode* o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeArmy(jdoc.toJson()),
            utils::ValueError
        );
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
    std::for_each(
        tts.cbegin(),
        tts.cend(),
        [&](core::TerrainType* o){ctx.add(o);}
    );

    const std::vector<core::Civilization*> fs = world->getCivilizations();
    std::for_each(
        fs.cbegin(),
        fs.cend(),
        [&](core::Civilization* o){ctx.add(o);}
    );

    const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
    std::for_each(
        sts.cbegin(),
        sts.cend(),
        [&](core::SettlementType* o){ctx.add(o);}
    );

    const std::vector<core::UnitType*> uts = world->getUnitTypes();
    std::for_each(
        uts.cbegin(),
        uts.cend(),
        [&](core::UnitType* o){ctx.add(o);}
    );

    const std::vector<core::ArmyType*> ats = world->getArmyTypes();
    std::for_each(
        ats.cbegin(),
        ats.cend(),
        [&](core::ArmyType* o){ctx.add(o);}
    );

    SECTION("unserializing CampaignMap")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::CampaignMap> m(
            unserializer.unserializeCampaignMap(jdoc.toJson())
        );

        REQUIRE(m->objectName() == jobj["objectName"].toString());
        REQUIRE(m->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(m->getWorld()->objectName() == jobj["world"].toString());
        REQUIRE(m->getMapNodeIndex() == jobj["mapNodeIndex"].toInt());
        REQUIRE(m->getSettlementIndex() == jobj["settlementIndex"].toInt());
        REQUIRE(m->getUnitIndex() == jobj["unitIndex"].toInt());

        SECTION("unserializing mapNodes")
        {
            const std::vector<core::MapNode*> mns{m->getMapNodes()};
            const QJsonArray jmns(jobj["mapNodes"].toArray());

            REQUIRE(jmns.size() == mns.size());

            for (size_t i = 0; i < mns.size(); i++)
            {
                const core::MapNode* mn{mns[i]};
                const QJsonObject jmn(jmns[i].toObject());
                REQUIRE(mn->objectName() == jmn["objectName"].toString());
                REQUIRE(mn->getDisplayName() == jmn["displayName"].toString());
                REQUIRE(mn->getTerrainType()->objectName() == jmn["terrainType"].toString());

                const QJsonObject jneighbours = jmn["neighbours"].toObject();
                const core::MapNodeNeighbours& neighbours = mn->getNeighbours();

                for (const auto& neighbour : neighbours)
                {
                    const QString dirName{utils::direction2str(neighbour.first)};
                    REQUIRE(jneighbours.contains(dirName));

                    REQUIRE((neighbour.second == nullptr) == jneighbours[dirName].toString().isEmpty());

                    if(neighbour.second != nullptr)
                    {
                        REQUIRE(neighbour.second->objectName() == jneighbours[dirName].toString());
                    }
                }
            }
        }

        SECTION("unserializing factions")
        {
            const std::vector<core::Faction*> ps(m->getFactions());
            const QJsonArray jps(jobj["factions"].toArray());

            REQUIRE(jps.size() == ps.size());

            for (size_t i = 0; i < ps.size(); i++)
            {
                core::Faction* p{ps[i]};
                const QJsonObject jp(jps[i].toObject());
                REQUIRE(p->objectName() == jp["objectName"].toString());
                REQUIRE(p->getDisplayName() == jp["displayName"].toString());
                REQUIRE(p->getColor().name() == jp["color"].toString());
                REQUIRE(p->getCivilization()->objectName() == jp["civilization"].toString());
            }
        }

        SECTION("unserializing settlements")
        {
            const std::vector<core::Settlement*> ss(m->getSettlements());
            const QJsonArray jss(jobj["settlements"].toArray());

            REQUIRE(jss.size() == ss.size());

            for (size_t i = 0; i < ss.size(); i++)
            {
                core::Settlement* s{ss[i]};
                const QJsonObject js(jss[i].toObject());
                REQUIRE(s->objectName() == js["objectName"].toString());
                REQUIRE(s->getDisplayName() == js["displayName"].toString());
                REQUIRE(s->getType()->objectName() == js["type"].toString());
                REQUIRE(s->getMapNode()->objectName() == js["mapNode"].toString());
                REQUIRE(s->getOwner()->objectName() == js["owner"].toString());
            }
        }

        SECTION("unserializing units")
        {
            const std::vector<core::Unit*> us(m->getUnits());
            const QJsonArray jus(jobj["units"].toArray());

            REQUIRE(jus.size() == us.size());

            for (size_t i = 0; i < us.size(); i++)
            {
                core::Unit* u{us[i]};
                const QJsonObject js(jus[i].toObject());
                REQUIRE(u->objectName() == js["objectName"].toString());
                REQUIRE(u->getDisplayName() == js["displayName"].toString());
                REQUIRE(u->getType()->objectName() == js["type"].toString());
                REQUIRE(u->getMapNode()->objectName() == js["mapNode"].toString());
                REQUIRE(u->getOwner()->objectName() == js["owner"].toString());
            }
        }

        SECTION("unserializing armies")
        {
            const std::vector<core::Army*> as(m->getArmies());
            const QJsonArray jas(jobj["armies"].toArray());

            REQUIRE(jas.size() == as.size());

            for (size_t i = 0; i < as.size(); i++)
            {
                core::Army* a{as[i]};
                const QJsonObject js(jas[i].toObject());
                REQUIRE(a->objectName() == js["objectName"].toString());
                REQUIRE(a->getDisplayName() == js["displayName"].toString());
                REQUIRE(a->getType()->objectName() == js["type"].toString());
                REQUIRE(a->getMapNode()->objectName() == js["mapNode"].toString());
                REQUIRE(a->getOwner()->objectName() == js["owner"].toString());
                arrayEqualsList(js["units"].toArray(), a->getUnits());
            }
        }
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

        REQUIRE_THROWS_AS(
            unserializer.unserializeCampaignMap(invalidJson),
            utils::ValueError
        );
    }

    SECTION("unserializing CampaignMap, no world")
    {
        io::Context ctx;

        const std::vector<core::TerrainType*> tts = world->getTerrainTypes();
        std::for_each(
            tts.cbegin(),
            tts.cend(),
            [&](core::TerrainType* o){ctx.add(o);}
        );
        const std::vector<core::Civilization*> fs = world->getCivilizations();
        std::for_each(
            fs.cbegin(),
            fs.cend(),
            [&](core::Civilization* o){ctx.add(o);}
        );
        const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
        std::for_each(
            sts.cbegin(),
            sts.cend(),
            [&](core::SettlementType* o){ctx.add(o);}
        );
        const std::vector<core::UnitType*> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType* o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeCampaignMap(jdoc.toJson()),
            utils::ValueError
        );
    }

    SECTION("unserializing Map, no terrainTypes")
    {
        io::Context ctx;

        ctx.add(world.get());

        const std::vector<core::Civilization*> fs = world->getCivilizations();
        std::for_each(
            fs.cbegin(),
            fs.cend(),
            [&](core::Civilization* o){ctx.add(o);}
        );
        const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
        std::for_each(
            sts.cbegin(),
            sts.cend(),
            [&](core::SettlementType* o){ctx.add(o);}
        );
        const std::vector<core::UnitType*> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType* o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeCampaignMap(jdoc.toJson()),
            utils::ValueError
        );
    }

    SECTION("unserializing Map, no civilizations")
    {
        io::Context ctx;

        ctx.add(world.get());

        const std::vector<core::TerrainType*> tts = world->getTerrainTypes();
        std::for_each(
            tts.cbegin(),
            tts.cend(),
            [&](core::TerrainType* o){ctx.add(o);}
        );
        const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
        std::for_each(
            sts.cbegin(),
            sts.cend(),
            [&](core::SettlementType* o){ctx.add(o);}
        );
        const std::vector<core::UnitType*> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType* o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeCampaignMap(jdoc.toJson()),
            utils::ValueError
        );
    }

    SECTION("unserializing Map, no settlementTypes")
    {
        io::Context ctx;

        ctx.add(world.get());

        const std::vector<core::TerrainType*> tts = world->getTerrainTypes();
        std::for_each(
            tts.cbegin(),
            tts.cend(),
            [&](core::TerrainType* o){ctx.add(o);}
        );
        const std::vector<core::Civilization*> fs = world->getCivilizations();
        std::for_each(
            fs.cbegin(),
            fs.cend(),
            [&](core::Civilization* o){ctx.add(o);}
        );
        const std::vector<core::UnitType*> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType* o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeCampaignMap(jdoc.toJson()),
            utils::ValueError
        );
    }

    SECTION("unserializing Map, no unitTypes")
    {
        io::Context ctx;

        ctx.add(world.get());

        const std::vector<core::TerrainType*> tts = world->getTerrainTypes();
        std::for_each(
            tts.cbegin(),
            tts.cend(),
            [&](core::TerrainType* o){ctx.add(o);}
        );
        const std::vector<core::Civilization*> fs = world->getCivilizations();
        std::for_each(
            fs.cbegin(),
            fs.cend(),
            [&](core::Civilization* o){ctx.add(o);}
        );
        const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
        std::for_each(
            sts.cbegin(),
            sts.cend(),
            [&](core::SettlementType* o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeCampaignMap(jdoc.toJson()),
            utils::ValueError
        );
    }
}

TEST_CASE("Civilization can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const std::vector<core::UnitType*> uts = world->getUnitTypes();
    std::for_each(
        uts.cbegin(),
        uts.cend(),
        [&](core::UnitType* o){ctx.add(o);}
    );
    const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
    std::for_each(
        sts.cbegin(),
        sts.cend(),
        [&](core::SettlementType* o){ctx.add(o);}
    );

    const QJsonObject jobj = jworld["civilizations"].toArray()[0].toObject();

    SECTION("unserializing Civilization")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Civilization> f(
            unserializer.unserializeCivilization(jdoc.toJson())
        );

        REQUIRE(f->objectName() == jobj["objectName"].toString());
        REQUIRE(f->getDisplayName() == jobj["displayName"].toString());
        arrayEqualsList(jobj["unitTypes"].toArray(), f->getUnitTypes());

        std::map<core::SettlementType*, std::vector<core::UnitType*>> recruits = f->getRecruits();
        const QJsonObject jrecruits = jobj["recruits"].toObject();

        REQUIRE(recruits.size() == jrecruits.size());

        for (const auto& e : recruits)
        {
            arrayEqualsList(jrecruits[e.first->objectName()].toArray(), e.second);
        }
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

        REQUIRE_THROWS_AS(
            unserializer.unserializeCivilization(invalidJson),
            utils::ValueError
        );
    }

    SECTION("unserializing Civilization, no settlementTypes")
    {
        io::Context ctx;

        const std::vector<core::UnitType*> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType* o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeCivilization(jdoc.toJson()),
            utils::ValueError
        );
    }

    SECTION("unserializing Civilization, no unitTypes")
    {
        io::Context ctx;

        const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
        std::for_each(
            sts.cbegin(),
            sts.cend(),
            [&](core::SettlementType* o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeCivilization(jdoc.toJson()),
            utils::ValueError
        );
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
    std::for_each(
        tts.cbegin(),
        tts.cend(),
        [&](core::TerrainType* o){ctx.add(o);}
    );

    const QJsonObject jobj = jmap["mapNodes"].toArray()[0].toObject();

    SECTION("unserializing MapNode")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::MapNode> mn(
            unserializer.unserializeMapNode(jdoc.toJson())
        );

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

        REQUIRE_THROWS_AS(
            unserializer.unserializeMapNode(invalidJson),
            utils::ValueError
        );
    }

    SECTION("unserializing MapNode, no terrainTypes")
    {
        io::Context ctx;

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeMapNode(jdoc.toJson()),
            utils::ValueError
        );
    }
}

TEST_CASE("Faction can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::CampaignMap*, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jmap{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const std::vector<core::Civilization*> fs = world->getCivilizations();
    std::for_each(
        fs.cbegin(),
        fs.cend(),
        [&](core::Civilization* o){ctx.add(o);}
    );

    const QJsonObject jobj = jmap["factions"].toArray()[0].toObject();

    SECTION("unserializing Faction")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Faction> p(
            unserializer.unserializeFaction(jdoc.toJson())
        );

        REQUIRE(p->objectName() == jobj["objectName"].toString());
        REQUIRE(p->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(p->getColor().name() == jobj["color"].toString());
        REQUIRE(p->getGoldBalance() == jobj["goldBalance"].toInt());
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

        REQUIRE_THROWS_AS(
            unserializer.unserializeFaction(invalidJson),
            utils::ValueError
        );
    }

    SECTION("unserializing Faction, no Civilizations")
    {
        io::Context ctx;

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeFaction(jdoc.toJson()),
            utils::ValueError
        );
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
    std::for_each(
        sts.cbegin(),
        sts.cend(),
        [&](core::SettlementType* o){ctx.add(o);}
    );

    const std::vector<core::MapNode*> mns = map->getMapNodes();
    std::for_each(
        mns.cbegin(),
        mns.cend(),
        [&](core::MapNode* o){ctx.add(o);}
    );

    const std::vector<core::Faction*> fs = map->getFactions();
    std::for_each(
        fs.cbegin(),
        fs.cend(),
        [&](core::Faction* o){ctx.add(o);}
    );

    const QJsonObject jobj = jmap["settlements"].toArray()[0].toObject();

    SECTION("unserializing Settlement")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Settlement> s(
            unserializer.unserializeSettlement(jdoc.toJson())
        );

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

        REQUIRE_THROWS_AS(
            unserializer.unserializeSettlement(invalidJson),
            utils::ValueError
        );
    }

    SECTION("unserializing Settlement, no settlementTypes")
    {
        io::Context ctx;

        const std::vector<core::MapNode*> mns = map->getMapNodes();
        std::for_each(
            mns.cbegin(),
            mns.cend(),
            [&](core::MapNode* o){ctx.add(o);}
        );

        const std::vector<core::Faction*> fs = map->getFactions();
        std::for_each(
            fs.cbegin(),
            fs.cend(),
            [&](core::Faction* o){ctx.add(o);}
        );

        QJsonDocument jdoc(jobj);
        io::JsonUnserializer unserializer(ctx);
        REQUIRE_THROWS_AS(
            unserializer.unserializeSettlement(jdoc.toJson()),
            utils::ValueError
        );
    }

    SECTION("unserializing Settlement, no mapNodes")
    {
        io::Context ctx;

        const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
        std::for_each(
            sts.cbegin(),
            sts.cend(),
            [&](core::SettlementType* o){ctx.add(o);}
        );

        const std::vector<core::Faction*> fs = map->getFactions();
        std::for_each(
            fs.cbegin(),
            fs.cend(),
            [&](core::Faction* o){ctx.add(o);}
        );

        QJsonDocument jdoc(jobj);
        io::JsonUnserializer unserializer(ctx);
        REQUIRE_THROWS_AS(
            unserializer.unserializeSettlement(jdoc.toJson()),
            utils::ValueError
        );
    }

    SECTION("unserializing Settlement, no palyers")
    {
        io::Context ctx;

        const std::vector<core::SettlementType*> sts = world->getSettlementTypes();
        std::for_each(
            sts.cbegin(),
            sts.cend(),
            [&](core::SettlementType* o){ctx.add(o);}
        );

        const std::vector<core::MapNode*> mns = map->getMapNodes();
        std::for_each(
            mns.cbegin(),
            mns.cend(),
            [&](core::MapNode* o){ctx.add(o);}
        );

        QJsonDocument jdoc(jobj);
        io::JsonUnserializer unserializer(ctx);
        REQUIRE_THROWS_AS(
            unserializer.unserializeSettlement(jdoc.toJson()),
            utils::ValueError
        );
    }
}

TEST_CASE("SettlementType can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const std::vector<core::UnitType*> uts = world->getUnitTypes();
    std::for_each(
        uts.cbegin(),
        uts.cend(),
        [&](core::UnitType* o){ctx.add(o);}
    );

    const QJsonObject jobj = jworld["settlementTypes"].toArray()[0].toObject();

    SECTION("unserializing SettlementType")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::SettlementType> st(
            unserializer.unserializeSettlementType(jdoc.toJson())
        );

        REQUIRE(st->objectName() == jobj["objectName"].toString());
        REQUIRE(st->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(st->getGoldPerTurn() == jobj["goldPerTurn"].toInt());
        arrayEqualsList(jobj["recruits"].toArray(), st->getRecruits());
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

        REQUIRE_THROWS_AS(
            unserializer.unserializeSettlementType(invalidJson),
            utils::ValueError
        );
    }

    SECTION("unserializing SettlementType, no unitTypes")
    {
        io::Context ctx;

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        REQUIRE_THROWS_AS(
            unserializer.unserializeSettlementType(jdoc.toJson()),
            utils::ValueError
        );
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
        const std::unique_ptr<core::TerrainType> tt(
            unserializer.unserializeTerrainType(jdoc.toJson())
        );

        REQUIRE(tt->objectName() == jobj["objectName"].toString());
        REQUIRE(tt->getDisplayName() == jobj["displayName"].toString());
    }
}

TEST_CASE("TerrainType can't be unserialized from JSON", "[JsonUnserializer]")
{
    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(
            unserializer.unserializeTerrainType(invalidJson),
            utils::ValueError
        );
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
    std::for_each(
        uts.cbegin(),
        uts.cend(),
        [&](core::UnitType* o){ctx.add(o);}
    );

    const std::vector<core::MapNode*> mns = map->getMapNodes();
    std::for_each(
        mns.cbegin(),
        mns.cend(),
        [&](core::MapNode* o){ctx.add(o);}
    );

    const std::vector<core::Faction*> fs = map->getFactions();
    std::for_each(
        fs.cbegin(),
        fs.cend(),
        [&](core::Faction* o){ctx.add(o);}
    );

    const QJsonObject jobj = jmap["units"].toArray()[0].toObject();

    SECTION("unserializing Unit")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Unit> u(
            unserializer.unserializeUnit(jdoc.toJson())
        );

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

        REQUIRE_THROWS_AS(
            unserializer.unserializeUnit(invalidJson),
            utils::ValueError
        );
    }

    SECTION("unserializing Unit, no unitTypes")
    {
        io::Context ctx;

        const std::vector<core::MapNode*> mns = map->getMapNodes();
        std::for_each(
            mns.cbegin(),
            mns.cend(),
            [&](core::MapNode* o){ctx.add(o);}
        );

        const std::vector<core::Faction*> fs = map->getFactions();
        std::for_each(
            fs.cbegin(),
            fs.cend(),
            [&](core::Faction* o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeUnit(jdoc.toJson()),
            utils::ValueError
        );
    }

    SECTION("unserializing Unit, no mapNodes")
    {
        io::Context ctx;

        const std::vector<core::UnitType*> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType* o){ctx.add(o);}
        );

        const std::vector<core::Faction*> fs = map->getFactions();
        std::for_each(
            fs.cbegin(),
            fs.cend(),
            [&](core::Faction* o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeUnit(jdoc.toJson()),
            utils::ValueError
        );
    }

    SECTION("unserializing Unit, no factions")
    {
        io::Context ctx;

        const std::vector<core::UnitType*> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType* o){ctx.add(o);}
        );

        const std::vector<core::MapNode*> mns = map->getMapNodes();
        std::for_each(
            mns.cbegin(),
            mns.cend(),
            [&](core::MapNode* o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeUnit(jdoc.toJson()),
            utils::ValueError
        );
    }
}

TEST_CASE("UnitType can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const std::vector<core::TerrainType*> tts = world->getTerrainTypes();
    std::for_each(
        tts.cbegin(),
        tts.cend(),
        [&](core::TerrainType* o){ctx.add(o);}
    );
    const std::vector<core::UnitType*> uts = world->getUnitTypes();
    std::for_each(
        uts.cbegin(),
        uts.cend(),
        [&](core::UnitType* o){ctx.add(o);}
    );

    SECTION("unserializing UnitType")
    {
        const QJsonObject jobj = jworld["unitTypes"].toArray()[0].toObject();

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::UnitType> ut(
            unserializer.unserializeUnitType(jdoc.toJson())
        );

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
        const std::unique_ptr<core::UnitType> ut(
            unserializer.unserializeUnitType(jdoc.toJson())
        );

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

        REQUIRE_THROWS_AS(
            unserializer.unserializeUnitType(invalidJson),
            utils::ValueError
        );
    }

    SECTION("unserializing UnitType, no terrainTypes")
    {
        io::Context ctx;

        const std::vector<core::UnitType*> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType* o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeUnitType(jdoc.toJson()),
            utils::ValueError
        );
    }

    SECTION("unserializing UnitType, no unitTypes")
    {
        io::Context ctx;

        const std::vector<core::TerrainType*> tts = world->getTerrainTypes();
        std::for_each(
            tts.cbegin(),
            tts.cend(),
            [&](core::TerrainType* o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeUnitType(jdoc.toJson()),
            utils::ValueError
        );
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
        const std::unique_ptr<core::World> world(
            unserializer.unserializeWorld(jdoc.toJson())
        );

        REQUIRE(world->objectName() == jobj["objectName"].toString());
        REQUIRE(world->getDisplayName() == jobj["displayName"].toString());

        SECTION("unserializing terrainTypes")
        {
            const std::vector<core::TerrainType*> tts(world->getTerrainTypes());
            const QJsonArray jtts(jobj["terrainTypes"].toArray());

            REQUIRE(jtts.size() == tts.size());

            for (size_t i = 0; i < tts.size(); i++)
            {
                core::TerrainType* tt{tts[i]};
                const QJsonObject jtt(jtts[i].toObject());
                REQUIRE(tt->objectName() == jtt["objectName"].toString());
                REQUIRE(tt->getDisplayName() == jtt["displayName"].toString());
            }
        }

        SECTION("unserializing unitTypes")
        {
            const std::vector<core::UnitType*> uts(world->getUnitTypes());
            const QJsonArray juts(jobj["unitTypes"].toArray());

            REQUIRE(juts.size() == uts.size());

            for (size_t i = 0; i < uts.size(); i++)
            {
                core::UnitType* ut{uts[i]};
                const QJsonObject jut(juts[i].toObject());
                REQUIRE(ut->objectName() == jut["objectName"].toString());
                REQUIRE(ut->getDisplayName() == jut["displayName"].toString());
                REQUIRE(ut->getHitPoints() == jut["hitPoints"].toInt());
                REQUIRE(ut->getExperiencePoints() == jut["experiencePoints"].toInt());
                REQUIRE(ut->getMovementPoints() == jut["movementPoints"].toInt());
                REQUIRE(ut->getRecruitmentCost() == jut["recruitmentCost"].toInt());
                REQUIRE(ut->getUpkeepCost() == jut["upkeepCost"].toInt());
                arrayEqualsList(jut["upgrades"].toArray(), ut->getUpgrades());
                objectEqualsMap(jut["movementCosts"].toObject(), ut->getMovementCosts());
            }
        }

        SECTION("unserializing settlementTypes")
        {
            const std::vector<core::SettlementType*> sts(world->getSettlementTypes());
            const QJsonArray jsts(jobj["settlementTypes"].toArray());

            REQUIRE(jsts.size() == sts.size());

            for (size_t i = 0; i < sts.size(); i++)
            {
                core::SettlementType* st{sts[i]};
                const QJsonObject jst(jsts[i].toObject());
                REQUIRE(st->objectName() == jst["objectName"].toString());
                REQUIRE(st->getDisplayName() == jst["displayName"].toString());
                REQUIRE(st->getGoldPerTurn() == jst["goldPerTurn"].toInt());
                arrayEqualsList(jst["recruits"].toArray(), st->getRecruits());
            }
        }

        SECTION("unserializing civilizations")
        {
            const std::vector<core::Civilization*> cs(world->getCivilizations());
            const QJsonArray jcs(jobj["civilizations"].toArray());

            REQUIRE(jcs.size() == cs.size());

            for (size_t i = 0; i < cs.size(); i++)
            {
                core::Civilization* c{cs[i]};
                const QJsonObject jc(jcs[i].toObject());
                REQUIRE(c->objectName() == jc["objectName"].toString());
                REQUIRE(c->getDisplayName() == jc["displayName"].toString());
                arrayEqualsList(jc["unitTypes"].toArray(), c->getUnitTypes());

                std::map<core::SettlementType*, std::vector<core::UnitType*>> rs = c->getRecruits();
                const QJsonObject jrs = jc["recruits"].toObject();

                REQUIRE(rs.size() == jrs.size());

                for (const auto& e : rs)
                {
                    arrayEqualsList(jrs[e.first->objectName()].toArray(), e.second);
                }
            }
        }
    }
}

TEST_CASE("World can't be unserialized from JSON", "[JsonUnserializer]")
{
    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(
            unserializer.unserializeWorld(invalidJson),
            utils::ValueError
        );
    }
}
