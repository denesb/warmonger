#include <algorithm>

#include <QJsonDocument>
#include <QJsonObject>

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

    SECTION("unserializing DamageType")
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
