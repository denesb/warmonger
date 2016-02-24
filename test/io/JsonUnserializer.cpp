#include <algorithm>
#include <memory>

#include <QJsonDocument>
#include <QJsonObject>

#include "core/MapNode.h"
#include "core/Player.h"
#include "core/Settlement.h"
#include "core/Unit.h"
#include "io/Context.h"
#include "io/Exception.h"
#include "io/JsonUnserializer.h"
#include "test/catch.hpp"
#include "test/Util.h"

using namespace warmonger;

CATCH_TRANSLATE_EXCEPTION(io::UnresolvedReferenceError& e) {
    return e.getMessage().toStdString();
}

TEST_CASE("Armor can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    core::DamageType *dt = world->getDamageTypes()[0];
    ctx.add(dt);

    QJsonObject jobj;
    jobj["objectName"] = "armor1";
    jobj["displayName"] = "Armor 1";

    QJsonObject jdefenses;
    jdefenses[dt->objectName()] = 12;
    jobj["defenses"] = jdefenses;

    SECTION("unserializing Armor")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Armor> a(
            unserializer.unserializeArmor(jdoc.toJson())
        );

        REQUIRE(a->objectName() == jobj["objectName"].toString());
        REQUIRE(a->getDisplayName() == jobj["displayName"].toString());

        objectEqualsMap(jdefenses, a->getDefenses());
    }
}

TEST_CASE("DamageType can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    QJsonObject jobj;
    jobj["objectName"] = "damageType1";
    jobj["displayName"] = "DamageType 1";

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

TEST_CASE("Faction can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    QList<core::UnitType *> uts = world->getUnitTypes();
    std::for_each(
        uts.cbegin(),
        uts.cend(),
        [&](core::UnitType *ut){ctx.add<core::UnitType *>(ut);}
    );
    core::SettlementType *st = world->getSettlementTypes()[0];
    ctx.add(st);

    QJsonObject jobj;
    jobj["objectName"] = "faction1";
    jobj["displayName"] = "Faction 1";
    QJsonArray unitTypes;
    std::transform(
        uts.begin(),
        uts.end(),
        std::back_inserter(unitTypes),
        [](core::UnitType *o){return o->objectName();}
    );
    jobj["unitTypes"] = unitTypes;

    QJsonObject jrecruits;
    jrecruits[st->objectName()] = unitTypes;
    jobj["recruits"] = jrecruits;

    SECTION("unserializing Faction")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Faction> f(
            unserializer.unserializeFaction(jdoc.toJson())
        );

        REQUIRE(f->objectName() == jobj["objectName"].toString());
        REQUIRE(f->getDisplayName() == jobj["displayName"].toString());
        arrayEqualsList(unitTypes, f->getUnitTypes());

        QMap<core::SettlementType *, QList<core::UnitType *>> recruits =
            f->getRecruits();
        REQUIRE(recruits.size() == jrecruits.size());
        for (core::SettlementType *s : recruits.keys())
        {
            arrayEqualsList(
                jrecruits[s->objectName()].toArray(),
                recruits[s]
            );
        }
    }
}

TEST_CASE("MapNode can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;
    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    core::TerrainType *tt = world->getTerrainTypes()[0];
    ctx.add(tt);

    QJsonObject jobj;
    jobj["objectName"] = "mapNode1";
    jobj["displayName"] = "MapNode 1";
    jobj["terrainType"] = tt->objectName();

    SECTION("unserializing MapNode")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::MapNode> mn(
            unserializer.unserializeMapNode(jdoc.toJson())
        );

        REQUIRE(mn->objectName() == jobj["objectName"].toString());
        REQUIRE(mn->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(mn->getTerrainType() == tt);
    }
}

TEST_CASE("Player can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    core::Faction *f = world->getFactions()[0];
    ctx.add(f);

    QJsonObject jobj;
    jobj["objectName"] = "player1";
    jobj["displayName"] = "Player 1";

    QColor color("red");
    jobj["color"] = color.name();
    jobj["goldBalance"] = 300;
    jobj["faction"] = f->objectName();

    SECTION("unserializing Player")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Player> p(
            unserializer.unserializePlayer(jdoc.toJson())
        );

        REQUIRE(p->objectName() == jobj["objectName"].toString());
        REQUIRE(p->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(p->getColor() == color);
        REQUIRE(p->getGoldBalance() == jobj["goldBalance"].toInt());
        REQUIRE(p->getFaction() == f);
    }
}

TEST_CASE("Settlement can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    core::SettlementType *st = world->getSettlementTypes()[0];

    core::MapNode mn;
    mn.setObjectName("mapNode1");

    core::Player p;
    p.setObjectName("player1");

    ctx.add(st);
    ctx.add(&mn);
    ctx.add(&p);

    QJsonObject jobj;
    jobj["objectName"] = "settlement1";
    jobj["displayName"] = "Settlement 1";
    jobj["type"] = st->objectName();
    jobj["mapNode"] = mn.objectName();
    jobj["owner"] = p.objectName();

    SECTION("unserializing Settlement")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Settlement> s(
            unserializer.unserializeSettlement(jdoc.toJson())
        );

        REQUIRE(s->objectName() == jobj["objectName"].toString());
        REQUIRE(s->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(s->getType() == st);
        REQUIRE(s->getMapNode() == &mn);
        REQUIRE(s->getOwner() == &p);
    }
}

TEST_CASE("SettlementType can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    QList<core::UnitType *> uts = world->getUnitTypes();
    std::for_each(
        uts.cbegin(),
        uts.cend(),
        [&](core::UnitType *ut){ctx.add<core::UnitType *>(ut);}
    );

    QJsonObject jobj;
    jobj["objectName"] = "settlementType1";
    jobj["displayName"] = "SettlementType 1";
    jobj["goldPerTurn"] = 20;
    QJsonArray recruits;
    std::transform(
        uts.begin(),
        uts.end(),
        std::back_inserter(recruits),
        [](core::UnitType *o){return o->objectName();}
    );
    jobj["recruits"] = recruits;

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
        arrayEqualsList(recruits, st->getRecruits());
    }
}

TEST_CASE("TerrainType can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    QJsonObject jobj;
    jobj["objectName"] = "terrainType1";
    jobj["displayName"] = "TerrainType 1";

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

TEST_CASE("Unit can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;
    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    core::UnitType *ut = world->getUnitTypes()[0];

    core::MapNode mn;
    mn.setObjectName("mapNode1");

    core::Player p;
    p.setObjectName("player1");

    ctx.add(ut);
    ctx.add(&mn);
    ctx.add(&p);

    QJsonObject jobj;
    jobj["objectName"] = "unit1";
    jobj["displayName"] = "Unit 1";
    jobj["type"] = ut->objectName();
    jobj["mapNode"] = mn.objectName();
    jobj["owner"] = p.objectName();

    SECTION("unserializing Unit")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const std::unique_ptr<core::Unit> u(
            unserializer.unserializeUnit(jdoc.toJson())
        );

        REQUIRE(u->objectName() == jobj["objectName"].toString());
        REQUIRE(u->getDisplayName() == jobj["displayName"].toString());
        REQUIRE(u->getType() == ut);
        REQUIRE(u->getMapNode() == &mn);
        REQUIRE(u->getOwner() == &p);
    }
}

TEST_CASE("UnitClass can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    core::TerrainType *tt = world->getTerrainTypes()[0];
    ctx.add(tt);

    QJsonObject jobj;
    jobj["objectName"] = "unitClass1";
    jobj["displayName"] = "UnitClass 1";
    jobj["movementPoints"] = 40;

    QJsonObject movementCosts;
    movementCosts[tt->objectName()] = 16;
    jobj["movementCosts"] = movementCosts;

    QJsonObject attacks;
    attacks[tt->objectName()] = 20;
    jobj["attacks"] = attacks;

    QJsonObject defenses;
    defenses[tt->objectName()] = 20;
    jobj["defenses"] = defenses;

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
        objectEqualsMap(movementCosts, uc->getMovementCosts());
        objectEqualsMap(attacks, uc->getAttacks());
        objectEqualsMap(defenses, uc->getDefenses());
    }
}

TEST_CASE("UnitLevel can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    QJsonObject jobj;
    jobj["objectName"] = "unitLevel1";
    jobj["displayName"] = "UnitLevel 1";
    jobj["experiencePoints"] = 100;
    jobj["index"] = 4;

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

TEST_CASE("UnitType can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    core::UnitClass *uc = world->getUnitClasses()[0];
    core::UnitLevel *ul = world->getUnitLevels()[0];
    core::Armor *a = world->getArmors()[0];
    QList<core::Weapon *> ws = world->getWeapons();
    QList<core::UnitType *> uts = world->getUnitTypes();
    ctx.add(uc);
    ctx.add(ul);
    ctx.add(a);
    std::for_each(
        ws.cbegin(),
        ws.cend(),
        [&](core::Weapon *w){ctx.add<core::Weapon *>(w);}
    );
    std::for_each(
        uts.cbegin(),
        uts.cend(),
        [&](core::UnitType *ut){ctx.add<core::UnitType *>(ut);}
    );

    QJsonObject jobj;
    jobj["objectName"] = "__unitType2";
    jobj["displayName"] = "__UnitType 2";
    jobj["class"] = uc->objectName();
    jobj["level"] = ul->objectName();
    jobj["hitPoints"] = 43;
    jobj["recruitmentCost"] = 25;
    jobj["upkeepCost"] = 8;
    jobj["armor"] = a->objectName();

    QJsonArray weapons;
    for (core::Weapon * w : ws)
    {
        weapons << w->objectName();
    }
    jobj["weapons"] = weapons;

    QJsonArray upgrades;
    for (core::UnitType * ut : uts)
    {
        upgrades << ut->objectName();
    }
    jobj["upgrades"] = upgrades;

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
        arrayEqualsList(weapons, ut->getWeapons());
        arrayEqualsList(upgrades, ut->getUpgrades());
    }
}

TEST_CASE("Weapon can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};
    const QJsonObject jworld{worlds.second};

    core::DamageType *dt = world->getDamageTypes()[0];
    ctx.add(dt);

    QJsonObject jobj;
    jobj["objectName"] = "weapon1";
    jobj["displayName"] = "Weapon 1";
    jobj["range"] = 3;

    QJsonObject damages;
    damages[dt->objectName()] = 40;
    jobj["damages"] = damages;

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

        objectEqualsMap(damages, w->getDamages());
    }
}

TEST_CASE("World can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const QPair<core::World *, QJsonObject> worlds = makeWorld();
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
            QList<core::DamageType *> dts(world->getDamageTypes());
            QJsonArray jdts(jobj["damageTypes"].toArray());

            REQUIRE(jdts.size() == dts.size());

            for (int i = 0; i < dts.size(); i++)
            {
                core::DamageType *dt{dts[i]};
                QJsonObject jdt(jdts[i].toObject());
                REQUIRE(dt->objectName() == jdt["objectName"].toString());
                REQUIRE(dt->getDisplayName() == jdt["displayName"].toString());
            }
        }

        SECTION("unserializing armors")
        {
            QList<core::Armor *> as{world->getArmors()};
            QJsonArray jas(jobj["armors"].toArray());

            REQUIRE(jas.size() == as.size());

            for (int i = 0; i < as.size(); i++)
            {
                core::Armor *a{as[i]};
                QJsonObject ja(jas[i].toObject());
                REQUIRE(a->objectName() == ja["objectName"].toString());
                REQUIRE(a->getDisplayName() == ja["displayName"].toString());
                objectEqualsMap(ja["defenses"].toObject(), a->getDefenses());
            }
        }

        SECTION("unserializing weapons")
        {
            QList<core::Weapon *> ws{world->getWeapons()};
            QJsonArray jws(jobj["weapons"].toArray());

            REQUIRE(jws.size() == ws.size());

            for (int i = 0; i < ws.size(); i++)
            {
                core::Weapon *w{ws[i]};
                QJsonObject jw(jws[i].toObject());
                REQUIRE(w->objectName() == jw["objectName"].toString());
                REQUIRE(w->getDisplayName() == jw["displayName"].toString());
                REQUIRE(w->getRange() == jw["range"].toInt());
                objectEqualsMap(jw["damages"].toObject(), w->getDamages());
            }
        }

        SECTION("unserializing terrainTypes")
        {
            QList<core::TerrainType *> tts(world->getTerrainTypes());
            QJsonArray jtts(jobj["terrainTypes"].toArray());

            REQUIRE(jtts.size() == tts.size());

            for (int i = 0; i < tts.size(); i++)
            {
                core::TerrainType *tt{tts[i]};
                QJsonObject jtt(jtts[i].toObject());
                REQUIRE(tt->objectName() == jtt["objectName"].toString());
                REQUIRE(tt->getDisplayName() == jtt["displayName"].toString());
            }
        }

        SECTION("unserializing unitClasses")
        {
            QList<core::UnitClass *> ucs(world->getUnitClasses());
            QJsonArray jucs(jobj["unitClasses"].toArray());

            REQUIRE(jucs.size() == ucs.size());

            for (int i = 0; i < ucs.size(); i++)
            {
                core::UnitClass *uc{ucs[i]};
                QJsonObject juc(jucs[i].toObject());
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
            QList<core::UnitLevel *> uls(world->getUnitLevels());
            QJsonArray juls(jobj["unitLevels"].toArray());

            REQUIRE(juls.size() == uls.size());

            for (int i = 0; i < uls.size(); i++)
            {
                core::UnitLevel *ul{uls[i]};
                QJsonObject jul(juls[i].toObject());
                REQUIRE(ul->objectName() == jul["objectName"].toString());
                REQUIRE(ul->getDisplayName() == jul["displayName"].toString());
                REQUIRE(ul->getExperiencePoints() == jul["experiencePoints"].toInt());
                REQUIRE(ul->getIndex() == jul["index"].toInt());
            }
        }

        SECTION("unserializing unitTypes")
        {
            QList<core::UnitType *> uts(world->getUnitTypes());
            QJsonArray juts(jobj["unitTypes"].toArray());

            REQUIRE(juts.size() == uts.size());

            for (int i = 0; i < uts.size(); i++)
            {
                core::UnitType *ut{uts[i]};
                QJsonObject jut(juts[i].toObject());
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
            QList<core::SettlementType *> sts(world->getSettlementTypes());
            QJsonArray jsts(jobj["settlementTypes"].toArray());

            REQUIRE(jsts.size() == sts.size());

            for (int i = 0; i < sts.size(); i++)
            {
                core::SettlementType *st{sts[i]};
                QJsonObject jst(jsts[i].toObject());
                REQUIRE(st->objectName() == jst["objectName"].toString());
                REQUIRE(st->getDisplayName() == jst["displayName"].toString());
                REQUIRE(st->getGoldPerTurn() == jst["goldPerTurn"].toInt());
                arrayEqualsList(jst["recruits"].toArray(), st->getRecruits());
            }
        }

        SECTION("unserializing factions")
        {
            QList<core::Faction *> fs(world->getFactions());
            QJsonArray jfs(jobj["factions"].toArray());

            REQUIRE(jfs.size() == fs.size());

            for (int i = 0; i < fs.size(); i++)
            {
                core::Faction *f{fs[i]};
                QJsonObject jf(jfs[i].toObject());
                REQUIRE(f->objectName() == jf["objectName"].toString());
                REQUIRE(f->getDisplayName() == jf["displayName"].toString());
                arrayEqualsList(jf["unitTypes"].toArray(), f->getUnitTypes());

                QMap<core::SettlementType *, QList<core::UnitType *>> rs =
                    f->getRecruits();
                QJsonObject jrs = jf["recruits"].toObject();

                REQUIRE(rs.size() == jrs.size());

                for (core::SettlementType *st : rs.keys())
                {
                    arrayEqualsList(jrs[st->objectName()].toArray(), rs[st]);
                }
            }
        }
    }
}

TEST_CASE("WorldSurface can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    QJsonObject jobj;
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
