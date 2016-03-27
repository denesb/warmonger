#include <algorithm>
#include <memory>

#include <QJsonDocument>
#include <QJsonObject>

#include "io/Context.h"
#include "io/Exception.h"
#include "io/JsonUnserializer.h"
#include "test/catch.hpp"
#include "test/Util.h"

using namespace warmonger;

CATCH_TRANSLATE_EXCEPTION(io::UnresolvedReferenceError& e) {
    return e.getMessage().toStdString();
}

static const QByteArray invalidJson{"{\"displayName\": \"displayName\" \"objectName\": \"objectName\"}"};

TEST_CASE("Armor can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const std::vector<core::DamageType *> dts = world->getDamageTypes();
    std::for_each(
        dts.cbegin(),
        dts.cend(),
        [&](core::DamageType *o){ctx.add(o);}
    );

    const QJsonObject jobj = jworld["armors"].toArray()[0].toObject();

    SECTION("unserializing Armor")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Armor> a(
            unserializer.unserializeArmor(jdoc.toJson())
        );

        REQUIRE(a->objectName() == jobj["objectName"].toString());
        REQUIRE(a->getDisplayName() == jobj["displayName"].toString());

        objectEqualsMap(jobj["defenses"].toObject(), a->getDefenses());
    }
}

TEST_CASE("Armor can't be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const QJsonObject jobj = jworld["armors"].toArray()[0].toObject();

    SECTION("invalid JSON")
    {
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(
            unserializer.unserializeArmor(invalidJson),
            io::JsonParseError
        );
    }

    SECTION("no damageTypes")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeArmor(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }
}

TEST_CASE("CampaignMap can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::CampaignMap *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> obj{maps.first};
    const QJsonObject jobj{maps.second};
    const std::unique_ptr<core::World> world{obj->getWorld()};

    ctx.add(world.get());

    const std::vector<core::TerrainType *> tts = world->getTerrainTypes();
    std::for_each(
        tts.cbegin(),
        tts.cend(),
        [&](core::TerrainType *o){ctx.add(o);}
    );

    const std::vector<core::Faction *> fs = world->getFactions();
    std::for_each(
        fs.cbegin(),
        fs.cend(),
        [&](core::Faction *o){ctx.add(o);}
    );

    const std::vector<core::SettlementType *> sts = world->getSettlementTypes();
    std::for_each(
        sts.cbegin(),
        sts.cend(),
        [&](core::SettlementType *o){ctx.add(o);}
    );

    const std::vector<core::UnitType *> uts = world->getUnitTypes();
    std::for_each(
        uts.cbegin(),
        uts.cend(),
        [&](core::UnitType *o){ctx.add(o);}
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
            const std::vector<core::MapNode *> mns{m->getMapNodes()};
            const QJsonArray jmns(jobj["mapNodes"].toArray());

            REQUIRE(jmns.size() == mns.size());

            for (size_t i = 0; i < mns.size(); i++)
            {
                const core::MapNode *mn{mns[i]};
                const QJsonObject jmn(jmns[i].toObject());
                REQUIRE(mn->objectName() == jmn["objectName"].toString());
                REQUIRE(mn->getDisplayName() == jmn["displayName"].toString());
                REQUIRE(mn->getTerrainType()->objectName() == jmn["terrainType"].toString());

                const QJsonObject jneighbours = jmn["neighbours"].toObject();
                const std::map<core::Direction, core::MapNode *> neighbours = mn->getNeighbours();

                REQUIRE(neighbours.size() == jneighbours.size());

                for (const auto& neighbour : neighbours)
                {
                    const QString dirName{core::direction2str(neighbour.first)};
                    REQUIRE(jneighbours.contains(dirName));

                    REQUIRE((neighbour.second == nullptr) == jneighbours[dirName].toString().isEmpty());

                    if(neighbour.second != nullptr)
                    {
                        REQUIRE(neighbour.second->objectName() == jneighbours[dirName].toString());
                    }
                }
            }
        }

        SECTION("unserializing players")
        {
            const std::vector<core::Player *> ps(m->getPlayers());
            const QJsonArray jps(jobj["players"].toArray());

            REQUIRE(jps.size() == ps.size());

            for (size_t i = 0; i < ps.size(); i++)
            {
                core::Player *p{ps[i]};
                const QJsonObject jp(jps[i].toObject());
                REQUIRE(p->objectName() == jp["objectName"].toString());
                REQUIRE(p->getDisplayName() == jp["displayName"].toString());
                REQUIRE(p->getColor().name() == jp["color"].toString());
                REQUIRE(p->getFaction()->objectName() == jp["faction"].toString());
            }
        }

        SECTION("unserializing settlements")
        {
            const std::vector<core::Settlement *> ss(m->getSettlements());
            const QJsonArray jss(jobj["settlements"].toArray());

            REQUIRE(jss.size() == ss.size());

            for (size_t i = 0; i < ss.size(); i++)
            {
                core::Settlement *s{ss[i]};
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
            const std::vector<core::Unit *> us(m->getUnits());
            const QJsonArray jus(jobj["units"].toArray());

            REQUIRE(jus.size() == us.size());

            for (size_t i = 0; i < us.size(); i++)
            {
                core::Unit *u{us[i]};
                const QJsonObject js(jus[i].toObject());
                REQUIRE(u->objectName() == js["objectName"].toString());
                REQUIRE(u->getDisplayName() == js["displayName"].toString());
                REQUIRE(u->getType()->objectName() == js["type"].toString());
                REQUIRE(u->getMapNode()->objectName() == js["mapNode"].toString());
                REQUIRE(u->getOwner()->objectName() == js["owner"].toString());
            }
        }
    }
}

TEST_CASE("CampaignMap can't be unserialized from JSON", "[JsonUnserializer]")
{
    const std::pair<core::CampaignMap *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> obj{maps.first};
    const QJsonObject jobj{maps.second};
    const std::unique_ptr<core::World> world{obj->getWorld()};

    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(
            unserializer.unserializeCampaignMap(invalidJson),
            io::JsonParseError
        );
    }

    SECTION("unserializing CampaignMap, no world")
    {
        io::Context ctx;

        const std::vector<core::TerrainType *> tts = world->getTerrainTypes();
        std::for_each(
            tts.cbegin(),
            tts.cend(),
            [&](core::TerrainType *o){ctx.add(o);}
        );
        const std::vector<core::Faction *> fs = world->getFactions();
        std::for_each(
            fs.cbegin(),
            fs.cend(),
            [&](core::Faction *o){ctx.add(o);}
        );
        const std::vector<core::SettlementType *> sts = world->getSettlementTypes();
        std::for_each(
            sts.cbegin(),
            sts.cend(),
            [&](core::SettlementType *o){ctx.add(o);}
        );
        const std::vector<core::UnitType *> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType *o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeCampaignMap(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }

    SECTION("unserializing Map, no terrainTypes")
    {
        io::Context ctx;

        ctx.add(world.get());

        const std::vector<core::Faction *> fs = world->getFactions();
        std::for_each(
            fs.cbegin(),
            fs.cend(),
            [&](core::Faction *o){ctx.add(o);}
        );
        const std::vector<core::SettlementType *> sts = world->getSettlementTypes();
        std::for_each(
            sts.cbegin(),
            sts.cend(),
            [&](core::SettlementType *o){ctx.add(o);}
        );
        const std::vector<core::UnitType *> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType *o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeCampaignMap(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }

    SECTION("unserializing Map, no factions")
    {
        io::Context ctx;

        ctx.add(world.get());

        const std::vector<core::TerrainType *> tts = world->getTerrainTypes();
        std::for_each(
            tts.cbegin(),
            tts.cend(),
            [&](core::TerrainType *o){ctx.add(o);}
        );
        const std::vector<core::SettlementType *> sts = world->getSettlementTypes();
        std::for_each(
            sts.cbegin(),
            sts.cend(),
            [&](core::SettlementType *o){ctx.add(o);}
        );
        const std::vector<core::UnitType *> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType *o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeCampaignMap(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }

    SECTION("unserializing Map, no settlementTypes")
    {
        io::Context ctx;

        ctx.add(world.get());

        const std::vector<core::TerrainType *> tts = world->getTerrainTypes();
        std::for_each(
            tts.cbegin(),
            tts.cend(),
            [&](core::TerrainType *o){ctx.add(o);}
        );
        const std::vector<core::Faction *> fs = world->getFactions();
        std::for_each(
            fs.cbegin(),
            fs.cend(),
            [&](core::Faction *o){ctx.add(o);}
        );
        const std::vector<core::UnitType *> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType *o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeCampaignMap(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }

    SECTION("unserializing Map, no unitTypes")
    {
        io::Context ctx;

        ctx.add(world.get());

        const std::vector<core::TerrainType *> tts = world->getTerrainTypes();
        std::for_each(
            tts.cbegin(),
            tts.cend(),
            [&](core::TerrainType *o){ctx.add(o);}
        );
        const std::vector<core::Faction *> fs = world->getFactions();
        std::for_each(
            fs.cbegin(),
            fs.cend(),
            [&](core::Faction *o){ctx.add(o);}
        );
        const std::vector<core::SettlementType *> sts = world->getSettlementTypes();
        std::for_each(
            sts.cbegin(),
            sts.cend(),
            [&](core::SettlementType *o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeCampaignMap(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }
}


TEST_CASE("DamageType can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const QJsonObject jobj = jworld["damageTypes"].toArray()[0].toObject();

    SECTION("unserializing DamageType")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::DamageType> dt(
            unserializer.unserializeDamageType(jdoc.toJson())
        );

        REQUIRE(dt->objectName() == jobj["objectName"].toString());
        REQUIRE(dt->getDisplayName() == jobj["displayName"].toString());
    }
}

TEST_CASE("DamageType can't be unserialized from JSON", "[JsonUnserializer]")
{
    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(
            unserializer.unserializeDamageType(invalidJson),
            io::JsonParseError
        );
    }
}

TEST_CASE("Faction can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const std::vector<core::UnitType *> uts = world->getUnitTypes();
    std::for_each(
        uts.cbegin(),
        uts.cend(),
        [&](core::UnitType *o){ctx.add(o);}
    );
    const std::vector<core::SettlementType *> sts = world->getSettlementTypes();
    std::for_each(
        sts.cbegin(),
        sts.cend(),
        [&](core::SettlementType *o){ctx.add(o);}
    );

    const QJsonObject jobj = jworld["factions"].toArray()[0].toObject();

    SECTION("unserializing Faction")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Faction> f(
            unserializer.unserializeFaction(jdoc.toJson())
        );

        REQUIRE(f->objectName() == jobj["objectName"].toString());
        REQUIRE(f->getDisplayName() == jobj["displayName"].toString());
        arrayEqualsList(jobj["unitTypes"].toArray(), f->getUnitTypes());

        std::map<core::SettlementType *, std::vector<core::UnitType *>> recruits = f->getRecruits();
        const QJsonObject jrecruits = jobj["recruits"].toObject();

        REQUIRE(recruits.size() == jrecruits.size());

        for (const auto& e : recruits)
        {
            arrayEqualsList(jrecruits[e.first->objectName()].toArray(), e.second);
        }
    }
}

TEST_CASE("Faction can't be unserialized from JSON", "[JsonUnserializer]")
{
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const QJsonObject jobj = jworld["factions"].toArray()[0].toObject();

    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(
            unserializer.unserializeFaction(invalidJson),
            io::JsonParseError
        );
    }

    SECTION("unserializing Faction, no settlementTypes")
    {
        io::Context ctx;

        const std::vector<core::UnitType *> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType *o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeFaction(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }

    SECTION("unserializing Faction, no unitTypes")
    {
        io::Context ctx;

        const std::vector<core::SettlementType *> sts = world->getSettlementTypes();
        std::for_each(
            sts.cbegin(),
            sts.cend(),
            [&](core::SettlementType *o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeFaction(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }
}

TEST_CASE("MapNode can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::CampaignMap *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jmap{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const std::vector<core::TerrainType *> tts = world->getTerrainTypes();
    std::for_each(
        tts.cbegin(),
        tts.cend(),
        [&](core::TerrainType *o){ctx.add(o);}
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
    const std::pair<core::CampaignMap *, QJsonObject> maps = makeMap();
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
            io::JsonParseError
        );
    }

    SECTION("unserializing MapNode, no terrainTypes")
    {
        io::Context ctx;

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeMapNode(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }
}

TEST_CASE("Player can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::CampaignMap *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jmap{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const std::vector<core::Faction *> fs = world->getFactions();
    std::for_each(
        fs.cbegin(),
        fs.cend(),
        [&](core::Faction *o){ctx.add(o);}
    );

    const QJsonObject jobj = jmap["players"].toArray()[0].toObject();

    SECTION("unserializing Player")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Player> p(
            unserializer.unserializePlayer(jdoc.toJson())
        );

        REQUIRE(p->objectName() == jobj["objectName"].toString());
        REQUIRE(p->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(p->getColor().name() == jobj["color"].toString());
        REQUIRE(p->getGoldBalance() == jobj["goldBalance"].toInt());
        REQUIRE(p->getFaction()->objectName() == jobj["faction"].toString());
    }
}

TEST_CASE("Player can't be unserialized from JSON", "[JsonUnserializer]")
{
    const std::pair<core::CampaignMap *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jmap{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const QJsonObject jobj = jmap["players"].toArray()[0].toObject();

    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(
            unserializer.unserializePlayer(invalidJson),
            io::JsonParseError
        );
    }

    SECTION("unserializing Player, no Factions")
    {
        io::Context ctx;

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializePlayer(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }
}

TEST_CASE("Settlement can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::CampaignMap *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jmap{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const std::vector<core::SettlementType *> sts = world->getSettlementTypes();
    std::for_each(
        sts.cbegin(),
        sts.cend(),
        [&](core::SettlementType *o){ctx.add(o);}
    );

    const std::vector<core::MapNode *> mns = map->getMapNodes();
    std::for_each(
        mns.cbegin(),
        mns.cend(),
        [&](core::MapNode *o){ctx.add(o);}
    );

    const std::vector<core::Player *> ps = map->getPlayers();
    std::for_each(
        ps.cbegin(),
        ps.cend(),
        [&](core::Player *o){ctx.add(o);}
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
    const std::pair<core::CampaignMap *, QJsonObject> maps = makeMap();
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
            io::JsonParseError
        );
    }

    SECTION("unserializing Settlement, no settlementTypes")
    {
        io::Context ctx;

        const std::vector<core::MapNode *> mns = map->getMapNodes();
        std::for_each(
            mns.cbegin(),
            mns.cend(),
            [&](core::MapNode *o){ctx.add(o);}
        );

        const std::vector<core::Player *> ps = map->getPlayers();
        std::for_each(
            ps.cbegin(),
            ps.cend(),
            [&](core::Player *o){ctx.add(o);}
        );

        QJsonDocument jdoc(jobj);
        io::JsonUnserializer unserializer(ctx);
        REQUIRE_THROWS_AS(
            unserializer.unserializeSettlement(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }

    SECTION("unserializing Settlement, no mapNodes")
    {
        io::Context ctx;

        const std::vector<core::SettlementType *> sts = world->getSettlementTypes();
        std::for_each(
            sts.cbegin(),
            sts.cend(),
            [&](core::SettlementType *o){ctx.add(o);}
        );

        const std::vector<core::Player *> ps = map->getPlayers();
        std::for_each(
            ps.cbegin(),
            ps.cend(),
            [&](core::Player *o){ctx.add(o);}
        );

        QJsonDocument jdoc(jobj);
        io::JsonUnserializer unserializer(ctx);
        REQUIRE_THROWS_AS(
            unserializer.unserializeSettlement(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }

    SECTION("unserializing Settlement, no palyers")
    {
        io::Context ctx;

        const std::vector<core::SettlementType *> sts = world->getSettlementTypes();
        std::for_each(
            sts.cbegin(),
            sts.cend(),
            [&](core::SettlementType *o){ctx.add(o);}
        );

        const std::vector<core::MapNode *> mns = map->getMapNodes();
        std::for_each(
            mns.cbegin(),
            mns.cend(),
            [&](core::MapNode *o){ctx.add(o);}
        );

        QJsonDocument jdoc(jobj);
        io::JsonUnserializer unserializer(ctx);
        REQUIRE_THROWS_AS(
            unserializer.unserializeSettlement(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }
}

TEST_CASE("SettlementType can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const std::vector<core::UnitType *> uts = world->getUnitTypes();
    std::for_each(
        uts.cbegin(),
        uts.cend(),
        [&](core::UnitType *o){ctx.add(o);}
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
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const QJsonObject jobj = jworld["settlementTypes"].toArray()[0].toObject();

    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(
            unserializer.unserializeSettlementType(invalidJson),
            io::JsonParseError
        );
    }

    SECTION("unserializing SettlementType, no unitTypes")
    {
        io::Context ctx;

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        REQUIRE_THROWS_AS(
            unserializer.unserializeSettlementType(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }
}

TEST_CASE("TerrainType can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
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
            io::JsonParseError
        );
    }
}

TEST_CASE("Unit can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::CampaignMap *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jmap{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const std::vector<core::UnitType *> uts = world->getUnitTypes();
    std::for_each(
        uts.cbegin(),
        uts.cend(),
        [&](core::UnitType *o){ctx.add(o);}
    );

    const std::vector<core::MapNode *> mns = map->getMapNodes();
    std::for_each(
        mns.cbegin(),
        mns.cend(),
        [&](core::MapNode *o){ctx.add(o);}
    );

    const std::vector<core::Player *> ps = map->getPlayers();
    std::for_each(
        ps.cbegin(),
        ps.cend(),
        [&](core::Player *o){ctx.add(o);}
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
    const std::pair<core::CampaignMap *, QJsonObject> maps = makeMap();
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
            io::JsonParseError
        );
    }

    SECTION("unserializing Unit, no unitTypes")
    {
        io::Context ctx;

        const std::vector<core::MapNode *> mns = map->getMapNodes();
        std::for_each(
            mns.cbegin(),
            mns.cend(),
            [&](core::MapNode *o){ctx.add(o);}
        );

        const std::vector<core::Player *> ps = map->getPlayers();
        std::for_each(
            ps.cbegin(),
            ps.cend(),
            [&](core::Player *o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeUnit(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }

    SECTION("unserializing Unit, no mapNodes")
    {
        io::Context ctx;

        const std::vector<core::UnitType *> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType *o){ctx.add(o);}
        );

        const std::vector<core::Player *> ps = map->getPlayers();
        std::for_each(
            ps.cbegin(),
            ps.cend(),
            [&](core::Player *o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeUnit(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }

    SECTION("unserializing Unit, no players")
    {
        io::Context ctx;

        const std::vector<core::UnitType *> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType *o){ctx.add(o);}
        );

        const std::vector<core::MapNode *> mns = map->getMapNodes();
        std::for_each(
            mns.cbegin(),
            mns.cend(),
            [&](core::MapNode *o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeUnit(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }
}

TEST_CASE("UnitClass can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const std::vector<core::TerrainType *> tts = world->getTerrainTypes();
    std::for_each(
        tts.cbegin(),
        tts.cend(),
        [&](core::TerrainType *o){ctx.add(o);}
    );

    const QJsonObject jobj = jworld["unitClasses"].toArray()[0].toObject();

    SECTION("unserializing UnitClass")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::UnitClass> uc(
            unserializer.unserializeUnitClass(jdoc.toJson())
        );

        REQUIRE(uc->objectName() == jobj["objectName"].toString());
        REQUIRE(uc->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(uc->getMovementPoints() == jobj["movementPoints"].toInt());
        objectEqualsMap(
            jobj["movementCosts"].toObject(),
            uc->getMovementCosts()
        );
        objectEqualsMap(jobj["attacks"].toObject(), uc->getAttacks());
        objectEqualsMap(jobj["defenses"].toObject(), uc->getDefenses());
    }
}

TEST_CASE("UnitClass can't be unserialized from JSON", "[JsonUnserializer]")
{
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const QJsonObject jobj = jworld["unitClasses"].toArray()[0].toObject();

    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(
            unserializer.unserializeUnitClass(invalidJson),
            io::JsonParseError
        );
    }

    SECTION("unserializing UnitClass, no terrainTypes")
    {
        io::Context ctx;

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        REQUIRE_THROWS_AS(
            unserializer.unserializeUnitClass(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }
}

TEST_CASE("UnitLevel can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const QJsonObject jobj = jworld["unitLevels"].toArray()[0].toObject();

    SECTION("unserializing UnitLevel")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::UnitLevel> ul(
            unserializer.unserializeUnitLevel(jdoc.toJson())
        );

        REQUIRE(ul->objectName() == jobj["objectName"].toString());
        REQUIRE(ul->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(ul->getExperiencePoints() == jobj["experiencePoints"].toInt());
        REQUIRE(ul->getIndex() == jobj["index"].toInt());
    }
}

TEST_CASE("UnitLevel can't be unserialized from JSON", "[JsonUnserializer]")
{
    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(
            unserializer.unserializeUnitLevel(invalidJson),
            io::JsonParseError
        );
    }
}

TEST_CASE("UnitType can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const std::vector<core::UnitClass *> ucs = world->getUnitClasses();
    std::for_each(
        ucs.cbegin(),
        ucs.cend(),
        [&](core::UnitClass *o){ctx.add(o);}
    );
    const std::vector<core::UnitLevel *> uls = world->getUnitLevels();
    std::for_each(
        uls.cbegin(),
        uls.cend(),
        [&](core::UnitLevel *o){ctx.add(o);}
    );
    const std::vector<core::Armor *> as = world->getArmors();
    std::for_each(
        as.cbegin(),
        as.cend(),
        [&](core::Armor *o){ctx.add(o);}
    );
    const std::vector<core::Weapon *> ws = world->getWeapons();
    std::for_each(
        ws.cbegin(),
        ws.cend(),
        [&](core::Weapon *o){ctx.add(o);}
    );
    const std::vector<core::UnitType *> uts = world->getUnitTypes();
    std::for_each(
        uts.cbegin(),
        uts.cend(),
        [&](core::UnitType *o){ctx.add(o);}
    );

    const QJsonObject jobj = jworld["unitTypes"].toArray()[0].toObject();

    SECTION("unserializing UnitType")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::UnitType> ut(
            unserializer.unserializeUnitType(jdoc.toJson())
        );

        REQUIRE(ut->objectName() == jobj["objectName"].toString());
        REQUIRE(ut->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(ut->getClass()->objectName() == jobj["class"].toString());
        REQUIRE(ut->getHitPoints() == jobj["hitPoints"].toInt());
        REQUIRE(ut->getRecruitmentCost() == jobj["recruitmentCost"].toInt());
        REQUIRE(ut->getUpkeepCost() == jobj["upkeepCost"].toInt());
        REQUIRE(ut->getArmor()->objectName() == jobj["armor"].toString());
        arrayEqualsList(jobj["weapons"].toArray(), ut->getWeapons());
        arrayEqualsList(jobj["upgrades"].toArray(), ut->getUpgrades());
    }
}

TEST_CASE("UnitType can't be unserialized from JSON", "[JsonUnserializer]")
{
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const QJsonObject jobj = jworld["unitTypes"].toArray()[1].toObject();

    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(
            unserializer.unserializeUnitType(invalidJson),
            io::JsonParseError
        );
    }

    SECTION("unserializing UnitType, no unitClasses")
    {
        io::Context ctx;

        const std::vector<core::UnitLevel *> uls = world->getUnitLevels();
        std::for_each(
            uls.cbegin(),
            uls.cend(),
            [&](core::UnitLevel *o){ctx.add(o);}
        );
        const std::vector<core::Armor *> as = world->getArmors();
        std::for_each(
            as.cbegin(),
            as.cend(),
            [&](core::Armor *o){ctx.add(o);}
        );
        const std::vector<core::Weapon *> ws = world->getWeapons();
        std::for_each(
            ws.cbegin(),
            ws.cend(),
            [&](core::Weapon *o){ctx.add(o);}
        );
        const std::vector<core::UnitType *> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType *o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeUnitType(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }

    SECTION("unserializing UnitType, no unitLevels")
    {
        io::Context ctx;

        const std::vector<core::UnitClass *> ucs = world->getUnitClasses();
        std::for_each(
            ucs.cbegin(),
            ucs.cend(),
            [&](core::UnitClass *o){ctx.add(o);}
        );
        const std::vector<core::Armor *> as = world->getArmors();
        std::for_each(
            as.cbegin(),
            as.cend(),
            [&](core::Armor *o){ctx.add(o);}
        );
        const std::vector<core::Weapon *> ws = world->getWeapons();
        std::for_each(
            ws.cbegin(),
            ws.cend(),
            [&](core::Weapon *o){ctx.add(o);}
        );
        const std::vector<core::UnitType *> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType *o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeUnitType(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }

    SECTION("unserializing UnitType, no armors")
    {
        io::Context ctx;

        const std::vector<core::UnitClass *> ucs = world->getUnitClasses();
        std::for_each(
            ucs.cbegin(),
            ucs.cend(),
            [&](core::UnitClass *o){ctx.add(o);}
        );
        const std::vector<core::UnitLevel *> uls = world->getUnitLevels();
        std::for_each(
            uls.cbegin(),
            uls.cend(),
            [&](core::UnitLevel *o){ctx.add(o);}
        );
        const std::vector<core::Weapon *> ws = world->getWeapons();
        std::for_each(
            ws.cbegin(),
            ws.cend(),
            [&](core::Weapon *o){ctx.add(o);}
        );
        const std::vector<core::UnitType *> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType *o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeUnitType(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }

    SECTION("unserializing UnitType, no weapons")
    {
        io::Context ctx;

        const std::vector<core::UnitClass *> ucs = world->getUnitClasses();
        std::for_each(
            ucs.cbegin(),
            ucs.cend(),
            [&](core::UnitClass *o){ctx.add(o);}
        );
        const std::vector<core::UnitLevel *> uls = world->getUnitLevels();
        std::for_each(
            uls.cbegin(),
            uls.cend(),
            [&](core::UnitLevel *o){ctx.add(o);}
        );
        const std::vector<core::Armor *> as = world->getArmors();
        std::for_each(
            as.cbegin(),
            as.cend(),
            [&](core::Armor *o){ctx.add(o);}
        );
        const std::vector<core::UnitType *> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType *o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeUnitType(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }

    SECTION("unserializing UnitType, no unitTypes")
    {
        io::Context ctx;

        const std::vector<core::UnitClass *> ucs = world->getUnitClasses();
        std::for_each(
            ucs.cbegin(),
            ucs.cend(),
            [&](core::UnitClass *o){ctx.add(o);}
        );
        const std::vector<core::UnitLevel *> uls = world->getUnitLevels();
        std::for_each(
            uls.cbegin(),
            uls.cend(),
            [&](core::UnitLevel *o){ctx.add(o);}
        );
        const std::vector<core::Armor *> as = world->getArmors();
        std::for_each(
            as.cbegin(),
            as.cend(),
            [&](core::Armor *o){ctx.add(o);}
        );
        const std::vector<core::Weapon *> ws = world->getWeapons();
        std::for_each(
            ws.cbegin(),
            ws.cend(),
            [&](core::Weapon *o){ctx.add(o);}
        );

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);

        REQUIRE_THROWS_AS(
            unserializer.unserializeUnitType(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }
}

TEST_CASE("Weapon can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const std::vector<core::DamageType *> dts = world->getDamageTypes();
    std::for_each(
        dts.cbegin(),
        dts.cend(),
        [&](core::DamageType *o){ctx.add(o);}
    );

    const QJsonObject jobj = jworld["weapons"].toArray()[0].toObject();

    SECTION("unserializing Weapon")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Weapon> w(
            unserializer.unserializeWeapon(jdoc.toJson())
        );

        REQUIRE(w->objectName() == jobj["objectName"].toString());
        REQUIRE(w->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(w->getRange() == jobj["range"].toInt());
        objectEqualsMap(jobj["damages"].toObject(), w->getDamages());
    }
}

TEST_CASE("Weapon can't be unserialized from JSON", "[JsonUnserializer]")
{
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    const QJsonObject jobj = jworld["weapons"].toArray()[0].toObject();

    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(
            unserializer.unserializeWeapon(invalidJson),
            io::JsonParseError
        );
    }

    SECTION("unserializing Weapon")
    {
        io::Context ctx;

        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        REQUIRE_THROWS_AS(
            unserializer.unserializeWeapon(jdoc.toJson()),
            io::UnresolvedReferenceError
        );
    }
}

TEST_CASE("World can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
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

        SECTION("unserializing damageTypes")
        {
            const std::vector<core::DamageType *> dts(world->getDamageTypes());
            const QJsonArray jdts(jobj["damageTypes"].toArray());

            REQUIRE(jdts.size() == dts.size());

            for (size_t i = 0; i < dts.size(); i++)
            {
                core::DamageType *dt{dts[i]};
                const QJsonObject jdt(jdts[i].toObject());
                REQUIRE(dt->objectName() == jdt["objectName"].toString());
                REQUIRE(dt->getDisplayName() == jdt["displayName"].toString());
            }
        }

        SECTION("unserializing armors")
        {
            const std::vector<core::Armor *> as{world->getArmors()};
            const QJsonArray jas(jobj["armors"].toArray());

            REQUIRE(jas.size() == as.size());

            for (size_t i = 0; i < as.size(); i++)
            {
                core::Armor *a{as[i]};
                const QJsonObject ja(jas[i].toObject());
                REQUIRE(a->objectName() == ja["objectName"].toString());
                REQUIRE(a->getDisplayName() == ja["displayName"].toString());
                objectEqualsMap(ja["defenses"].toObject(), a->getDefenses());
            }
        }

        SECTION("unserializing weapons")
        {
            const std::vector<core::Weapon *> ws{world->getWeapons()};
            const QJsonArray jws(jobj["weapons"].toArray());

            REQUIRE(jws.size() == ws.size());

            for (size_t i = 0; i < ws.size(); i++)
            {
                core::Weapon *w{ws[i]};
                const QJsonObject jw(jws[i].toObject());
                REQUIRE(w->objectName() == jw["objectName"].toString());
                REQUIRE(w->getDisplayName() == jw["displayName"].toString());
                REQUIRE(w->getRange() == jw["range"].toInt());
                objectEqualsMap(jw["damages"].toObject(), w->getDamages());
            }
        }

        SECTION("unserializing terrainTypes")
        {
            const std::vector<core::TerrainType *> tts(world->getTerrainTypes());
            const QJsonArray jtts(jobj["terrainTypes"].toArray());

            REQUIRE(jtts.size() == tts.size());

            for (size_t i = 0; i < tts.size(); i++)
            {
                core::TerrainType *tt{tts[i]};
                const QJsonObject jtt(jtts[i].toObject());
                REQUIRE(tt->objectName() == jtt["objectName"].toString());
                REQUIRE(tt->getDisplayName() == jtt["displayName"].toString());
            }
        }

        SECTION("unserializing unitClasses")
        {
            const std::vector<core::UnitClass *> ucs(world->getUnitClasses());
            const QJsonArray jucs(jobj["unitClasses"].toArray());

            REQUIRE(jucs.size() == ucs.size());

            for (size_t i = 0; i < ucs.size(); i++)
            {
                core::UnitClass *uc{ucs[i]};
                const QJsonObject juc(jucs[i].toObject());
                REQUIRE(uc->objectName() == juc["objectName"].toString());
                REQUIRE(uc->getDisplayName() == juc["displayName"].toString());
                REQUIRE(uc->getMovementPoints() == juc["movementPoints"].toInt());
                objectEqualsMap(juc["movementCosts"].toObject(), uc->getMovementCosts());
                objectEqualsMap(juc["attacks"].toObject(), uc->getAttacks());
                objectEqualsMap(juc["defenses"].toObject(), uc->getDefenses());
            }
        }

        SECTION("unserializing unitLevels")
        {
            const std::vector<core::UnitLevel *> uls(world->getUnitLevels());
            const QJsonArray juls(jobj["unitLevels"].toArray());

            REQUIRE(juls.size() == uls.size());

            for (size_t i = 0; i < uls.size(); i++)
            {
                core::UnitLevel *ul{uls[i]};
                const QJsonObject jul(juls[i].toObject());
                REQUIRE(ul->objectName() == jul["objectName"].toString());
                REQUIRE(ul->getDisplayName() == jul["displayName"].toString());
                REQUIRE(ul->getExperiencePoints() == jul["experiencePoints"].toInt());
                REQUIRE(ul->getIndex() == jul["index"].toInt());
            }
        }

        SECTION("unserializing unitTypes")
        {
            const std::vector<core::UnitType *> uts(world->getUnitTypes());
            const QJsonArray juts(jobj["unitTypes"].toArray());

            REQUIRE(juts.size() == uts.size());

            for (size_t i = 0; i < uts.size(); i++)
            {
                core::UnitType *ut{uts[i]};
                const QJsonObject jut(juts[i].toObject());
                REQUIRE(ut->objectName() == jut["objectName"].toString());
                REQUIRE(ut->getDisplayName() == jut["displayName"].toString());
                REQUIRE(ut->getClass()->objectName() == jut["class"].toString());
                REQUIRE(ut->getLevel()->objectName() == jut["level"].toString());
                REQUIRE(ut->getHitPoints() == jut["hitPoints"].toInt());
                REQUIRE(ut->getRecruitmentCost() == jut["recruitmentCost"].toInt());
                REQUIRE(ut->getUpkeepCost() == jut["upkeepCost"].toInt());
                REQUIRE(ut->getArmor()->objectName() == jut["armor"].toString());
                arrayEqualsList(jut["weapons"].toArray(), ut->getWeapons());
                arrayEqualsList(jut["upgrades"].toArray(), ut->getUpgrades());
            }
        }

        SECTION("unserializing settlementTypes")
        {
            const std::vector<core::SettlementType *> sts(world->getSettlementTypes());
            const QJsonArray jsts(jobj["settlementTypes"].toArray());

            REQUIRE(jsts.size() == sts.size());

            for (size_t i = 0; i < sts.size(); i++)
            {
                core::SettlementType *st{sts[i]};
                const QJsonObject jst(jsts[i].toObject());
                REQUIRE(st->objectName() == jst["objectName"].toString());
                REQUIRE(st->getDisplayName() == jst["displayName"].toString());
                REQUIRE(st->getGoldPerTurn() == jst["goldPerTurn"].toInt());
                arrayEqualsList(jst["recruits"].toArray(), st->getRecruits());
            }
        }

        SECTION("unserializing factions")
        {
            const std::vector<core::Faction *> fs(world->getFactions());
            const QJsonArray jfs(jobj["factions"].toArray());

            REQUIRE(jfs.size() == fs.size());

            for (size_t i = 0; i < fs.size(); i++)
            {
                core::Faction *f{fs[i]};
                const QJsonObject jf(jfs[i].toObject());
                REQUIRE(f->objectName() == jf["objectName"].toString());
                REQUIRE(f->getDisplayName() == jf["displayName"].toString());
                arrayEqualsList(jf["unitTypes"].toArray(), f->getUnitTypes());

                std::map<core::SettlementType *, std::vector<core::UnitType *>> rs = f->getRecruits();
                const QJsonObject jrs = jf["recruits"].toObject();

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
            io::JsonParseError
        );
    }
}

TEST_CASE("WorldSurface can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const QJsonObject jobj;
    jobj["objectName"] = "weapon1";
    jobj["displayName"] = "Weapon 1";
    jobj["tileWidth"] = 100;
    jobj["tileHeight"] = 200;

    SECTION("unserializing WorldSurface")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::WorldSurface> ws(
            unserializer.unserializeWorldSurface(jdoc.toJson())
        );

        REQUIRE(ws->objectName() == jobj["objectName"].toString());
        REQUIRE(ws->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(ws->getTileWidth() == jobj["tileWidth"].toInt());
        REQUIRE(ws->getTileHeight() == jobj["tileHeight"].toInt());
    }
}

TEST_CASE("WorldSurface can't be unserialized from JSON", "[JsonUnserializer]")
{
    SECTION("invalid JSON")
    {
        io::Context ctx;
        io::JsonUnserializer unserializer(ctx);

        REQUIRE_THROWS_AS(
            unserializer.unserializeWorldSurface(invalidJson),
            io::JsonParseError
        );
    }
}
