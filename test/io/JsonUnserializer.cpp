#include <algorithm>

#include <QJsonDocument>
#include <QJsonObject>

#include "core/MapNode.h"
#include "core/Player.h"
#include "core/Settlement.h"
#include "core/Unit.h"
#include "io/Context.h"
#include "io/JsonUnserializer.h"
#include "test/catch.hpp"
#include "test/Util.h"

using namespace warmonger;

TEST_CASE("Armor can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    const std::unique_ptr<core::World> w{makeWorld()};
    core::DamageType *dt = w->getDamageTypes()[0];
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

    const std::unique_ptr<core::World> world{makeWorld()};
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
    const std::unique_ptr<core::World> world{makeWorld()};
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
    const std::unique_ptr<core::World> world{makeWorld()};
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

    const std::unique_ptr<core::World> world{makeWorld()};
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

    const std::unique_ptr<core::World> world{makeWorld()};
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
    const std::unique_ptr<core::World> world{makeWorld()};
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

    const std::unique_ptr<core::World> world{makeWorld()};
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

    const std::unique_ptr<core::World> world{makeWorld()};
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

    const std::unique_ptr<core::World> world{makeWorld()};
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
