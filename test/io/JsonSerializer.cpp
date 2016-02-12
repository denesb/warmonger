#include <memory>
#include <string>
#include <ostream>

#include <QJsonDocument>

#include "test/catch.hpp"

#include "core/Armor.h"
#include "core/DamageType.h"
#include "core/Faction.h"
#include "core/MapNode.h"
#include "core/Player.h"
#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/Unit.h"
#include "core/UnitClass.h"
#include "core/UnitLevel.h"
#include "core/UnitType.h"
#include "core/Weapon.h"
#include "core/World.h"
#include "core/WorldSurface.h"
#include "io/JsonSerializer.h"

using namespace warmonger;

std::ostream &operator<<(std::ostream &os, const QString &s)
{
    os << s.toStdString();
    return os;
}

template <typename T>
QJsonObject serialize(T obj)
{
    std::unique_ptr<io::Serializer> serializer(new io::JsonSerializer());
    QByteArray json(serializer->serialize(obj));

    QJsonDocument jdoc(QJsonDocument::fromJson(json));
    return jdoc.object();
}

TEST_CASE("Armor can be serialized to JSON", "[JsonSerializer]")
{
    core::Armor a(nullptr);
    a.setObjectName("armor1");
    a.setDisplayName("Armor1");

    core::DamageType dt(nullptr);
    dt.setObjectName("damageType1");

    a.setDefense(&dt, 10);

    SECTION("serializing Armor")
    {
        QJsonObject jobj(serialize(&a));

        REQUIRE(jobj["objectName"].toString() == a.objectName());
        REQUIRE(jobj["displayName"].toString() == a.getDisplayName());
        REQUIRE(jobj["defenses"].isObject() == true);

        QJsonObject defenses(jobj["defenses"].toObject());
        REQUIRE(defenses.size() == 1);
        REQUIRE(defenses[dt.objectName()] == a.getDefense(&dt));
    }
}

TEST_CASE("DamageType can be serialized to JSON", "[JsonSerializer]")
{
    core::DamageType dt(nullptr);
    dt.setObjectName("damageType1");
    dt.setDisplayName("DamageType1");

    SECTION("serializing DamageType")
    {
        QJsonObject jobj(serialize(&dt));

        REQUIRE(jobj["objectName"].toString() == dt.objectName());
        REQUIRE(jobj["displayName"].toString() == dt.getDisplayName());
    }
}

TEST_CASE("Faction can be serialized to JSON", "[JsonSerializer]")
{
    core::Faction f{nullptr};
    f.setObjectName("faction1");
    f.setDisplayName("Faction 1");

    core::UnitType ut1{nullptr};
    core::UnitType ut2{nullptr};

    ut1.setObjectName("unitType1");
    ut2.setObjectName("unitType2");

    QList<core::UnitType *> unitTypes{&ut1, &ut2};
    f.setUnitTypes(unitTypes);

    core::SettlementType st{nullptr};
    st.setObjectName("settlementType1");

    QMap<core::SettlementType *, QList<core::UnitType *>> recruits;
    recruits[&st] = QList<core::UnitType *>{&ut1, &ut2};
    f.setRecruits(recruits);

    SECTION("serializing Faction")
    {
        QJsonObject jobj(serialize(&f));

        REQUIRE(jobj["objectName"].toString() == f.objectName());
        REQUIRE(jobj["displayName"].toString() == f.getDisplayName());
        REQUIRE(jobj["unitTypes"].isArray() == true);

        QJsonArray unitTypes = jobj["unitTypes"].toArray();
        REQUIRE(unitTypes[0].toString() == ut1.objectName());
        REQUIRE(unitTypes[1].toString() == ut2.objectName());

        REQUIRE(jobj["recruits"].isObject() == true);

        QJsonObject recruits = jobj["recruits"].toObject();
        REQUIRE(recruits[st.objectName()].isArray() == true);
    }
}

TEST_CASE("MapNode can be serialized to JSON", "[JsonSerializer]")
{
    core::MapNode n{nullptr};
    n.setObjectName("mapNode1");
    n.setDisplayName("MapNode 1");

    core::TerrainType tt{nullptr};
    tt.setObjectName("terrainType1");
    tt.setDisplayName("TerrainType 1");

    n.setTerrainType(&tt);

    SECTION("serializing MapNode")
    {
        QJsonObject jobj(serialize(&n));

        REQUIRE(jobj["objectName"] == n.objectName());
        REQUIRE(jobj["displayName"] == n.getDisplayName());
        REQUIRE(jobj["terrainType"] == tt.objectName());
    }
}

TEST_CASE("Player can be serialized to JSON", "[JsonSerializer]")
{
    core::Player p{nullptr};
    p.setObjectName("player1");
    p.setDisplayName("Player 1");
    p.setColor(QColor("red"));
    p.setGoldBalance(100);

    core::Faction f{nullptr};
    f.setObjectName("faction1");

    p.setFaction(&f);

    SECTION("serializing Player")
    {
        QJsonObject jobj(serialize(&p));

        REQUIRE(jobj["objectName"].toString() == p.objectName());
        REQUIRE(jobj["displayName"].toString() == p.getDisplayName());
        REQUIRE(jobj["color"].toString() == p.getColor().name());
        REQUIRE(jobj["goldBalance"].toInt() == p.getGoldBalance());
        REQUIRE(jobj["faction"].toString() == f.objectName());
    }
}

TEST_CASE("Settlement can be serialized to JSON", "[JsonSerializer]")
{
    core::Settlement s(nullptr);
    s.setObjectName("settlementType1");
    s.setDisplayName("Settlement 1");

    core::MapNode mn{nullptr};
    mn.setObjectName("mapNode1");

    s.setMapNode(&mn);

    core::Player p{nullptr};
    p.setObjectName("player1");

    s.setOwner(&p);

    SECTION("serializing Settlement")
    {
        QJsonObject jobj(serialize(&s));

        REQUIRE(jobj["objectName"].toString() == s.objectName());
        REQUIRE(jobj["displayName"].toString() == s.getDisplayName());
        REQUIRE(jobj["mapNode"].toString() == s.getMapNode()->objectName());
        REQUIRE(jobj["owner"].toString() == s.getOwner()->objectName());
    }
}

TEST_CASE("SettlementType can be serialized to JSON", "[JsonSerializer]")
{
    core::SettlementType st(nullptr);
    st.setObjectName("settlementType1");
    st.setDisplayName("SettlementType 1");
    st.setGoldPerTurn(9);

    core::UnitType ut1{nullptr};
    core::UnitType ut2{nullptr};
    core::UnitType ut3{nullptr};

    ut1.setObjectName("unitType1");
    ut2.setObjectName("unitType2");
    ut3.setObjectName("unitType3");

    QList<core::UnitType *> recruits{&ut1, &ut2, &ut3};
    st.setRecruits(recruits);

    SECTION("serializing SettlementType")
    {
        QJsonObject jobj(serialize(&st));

        REQUIRE(jobj["objectName"].toString() == st.objectName());
        REQUIRE(jobj["displayName"].toString() == st.getDisplayName());
        REQUIRE(jobj["goldPerTurn"].toInt() == st.getGoldPerTurn());
        REQUIRE(jobj["recruits"].isArray() == true);

        QJsonArray recruits = jobj["recruits"].toArray();
        REQUIRE(recruits[0].toString() == ut1.objectName());
        REQUIRE(recruits[1].toString() == ut2.objectName());
        REQUIRE(recruits[2].toString() == ut3.objectName());
    }
}

TEST_CASE("TerrainType can be serialized to JSON", "[JsonSerializer]")
{
    core::TerrainType tt(nullptr);
    tt.setObjectName("terrainType1");
    tt.setDisplayName("TerrainType 1");

    SECTION("serializing TerrainType")
    {
        QJsonObject jobj(serialize(&tt));

        REQUIRE(jobj["objectName"].toString() == tt.objectName());
        REQUIRE(jobj["displayName"].toString() == tt.getDisplayName());
    }
}

TEST_CASE("Unit can be serialized to JSON", "[JsonSerializer]")
{
    core::Unit u{nullptr};
    u.setObjectName("unit1");
    u.setDisplayName("Unit 1");
    u.setRank(core::Unit::Soldier);

    core::UnitType ut{nullptr};
    ut.setObjectName("unitType1");

    u.setType(&ut);

    core::MapNode mn{nullptr};
    mn.setObjectName("mapNode1");

    u.setMapNode(&mn);

    core::Player p{nullptr};
    p.setObjectName("player1");

    u.setOwner(&p);

    u.setExperiencePoints(15.0);
    u.setHitPoints(28.2);
    u.setMovementPoints(16.0);

    SECTION("serializing Unit")
    {
        QJsonObject jobj(serialize(&u));

        REQUIRE(jobj["objectName"].toString() == u.objectName());
        REQUIRE(jobj["displayName"].toString() == u.getDisplayName());
        REQUIRE(jobj["rank"].toString() == core::Unit::rank2str[u.getRank()]);
        REQUIRE(jobj["type"].toString() == u.getType()->objectName());
        REQUIRE(jobj["mapNode"].toString() == u.getMapNode()->objectName());
        REQUIRE(jobj["owner"].toString() == u.getOwner()->objectName());
        REQUIRE(jobj["experiencePoints"].toDouble() == u.getExperiencePoints());
        REQUIRE(jobj["hitPoints"].toDouble() == u.getHitPoints());
        REQUIRE(jobj["movementPoints"].toDouble() == u.getMovementPoints());
    }
}

TEST_CASE("UnitClass can be serialized to JSON", "[JsonSerializer]")
{
    core::UnitClass uc(nullptr);
    uc.setObjectName("unitClass1");
    uc.setDisplayName("UnitClass 1");
    uc.setMovementPoints(16);

    core::TerrainType tt(nullptr);
    tt.setObjectName("terrainType1");

    uc.setMovementCost(&tt, 4);
    uc.setAttack(&tt, 5);
    uc.setDefense(&tt, 6);

    SECTION("serializing UnitClass")
    {
        QJsonObject jobj(serialize(&uc));

        REQUIRE(jobj["objectName"].toString() == uc.objectName());
        REQUIRE(jobj["displayName"].toString() == uc.getDisplayName());
        REQUIRE(jobj["movementPoints"].toInt() == uc.getMovementPoints());
        REQUIRE(jobj["movementCosts"].isObject() == true);
        REQUIRE(jobj["attacks"].isObject() == true);
        REQUIRE(jobj["defenses"].isObject() == true);

        QJsonObject mc(jobj["movementCosts"].toObject());
        REQUIRE(mc.size() == 1);
        REQUIRE(mc[tt.objectName()].toInt() == uc.getMovementCost(&tt));

        QJsonObject attacks(jobj["attacks"].toObject());
        REQUIRE(attacks.size() == 1);
        REQUIRE(attacks[tt.objectName()].toInt() == uc.getAttack(&tt));

        QJsonObject defenses(jobj["defenses"].toObject());
        REQUIRE(defenses.size() == 1);
        REQUIRE(defenses[tt.objectName()].toInt() == uc.getDefense(&tt));
    }
}

TEST_CASE("UnitLevel can be serialized to JSON", "[JsonSerializer]")
{
    core::UnitLevel ul(nullptr);
    ul.setObjectName("unitLevel1");
    ul.setDisplayName("UnitLevel 1");
    ul.setIndex(1);
    ul.setExperiencePoints(100);

    SECTION("serializing UnitLevel")
    {
        QJsonObject jobj(serialize(&ul));

        REQUIRE(jobj["objectName"].toString() == ul.objectName());
        REQUIRE(jobj["displayName"].toString() == ul.getDisplayName());
        REQUIRE(jobj["index"].toInt() == ul.getIndex());
        REQUIRE(jobj["experiencePoints"].toInt() == ul.getExperiencePoints());
    }
}

TEST_CASE("UnitType can be serialized to JSON", "[JsonSerializer]")
{
    core::UnitType ut(nullptr);
    ut.setObjectName("unitType1");
    ut.setDisplayName("UnitType 1");

    core::UnitClass uc(nullptr);
    uc.setObjectName("unitClass1");

    ut.setClass(&uc);

    core::UnitLevel ul(nullptr);
    ul.setObjectName("unitLevel1");

    ut.setLevel(&ul);

    ut.setHitPoints(30);
    ut.setRecruitmentCost(50);
    ut.setUpkeepCost(8);

    core::Armor a(nullptr);
    a.setObjectName("armor1");

    ut.setArmor(&a);

    core::Weapon w1(nullptr);
    w1.setObjectName("weapon1");

    core::Weapon w2(nullptr);
    w2.setObjectName("weapon2");

    const QList<core::Weapon *> weapons{&w1, &w2};
    ut.setWeapons(weapons);

    core::UnitType ut2(nullptr);
    ut2.setObjectName("unitType2");

    const QList<core::UnitType *> upgrades{&ut2};
    ut.setUpgrades(upgrades);

    SECTION("serializing UnitType")
    {
        QJsonObject jobj(serialize(&ut));

        REQUIRE(jobj["objectName"].toString() == ut.objectName());
        REQUIRE(jobj["displayName"].toString() == ut.getDisplayName());
        REQUIRE(jobj["class"].toString() == uc.objectName());
        REQUIRE(jobj["level"].toString() == ul.objectName());
        REQUIRE(jobj["hitPoints"].toInt() == ut.getHitPoints());
        REQUIRE(jobj["recruitmentCost"].toInt() == ut.getRecruitmentCost());
        REQUIRE(jobj["upkeepCost"].toInt() == ut.getUpkeepCost());
        REQUIRE(jobj["armor"].toString() == a.objectName());
        REQUIRE(jobj["weapons"].isArray() == true);
        REQUIRE(jobj["upgrades"].isArray() == true);

        QJsonArray weapons(jobj["weapons"].toArray());
        REQUIRE(weapons.size() == 2);
        REQUIRE(weapons[0].toString() == w1.objectName());
        REQUIRE(weapons[1].toString() == w2.objectName());

        QJsonArray upgrades(jobj["upgrades"].toArray());
        REQUIRE(upgrades.size() == 1);
        REQUIRE(upgrades[0].toString() == ut2.objectName());
    }
}

TEST_CASE("Weapon can be serialized to JSON", "[JsonSerializer]")
{
    core::Weapon w(nullptr);
    w.setObjectName("weapon1");
    w.setDisplayName("Weapon 1");
    w.setRange(2);

    core::DamageType dt(nullptr);
    dt.setObjectName("damageType1");

    w.setDamage(&dt, 20);

    SECTION("serializing WorldSurface")
    {
        QJsonObject jobj(serialize(&w));

        REQUIRE(jobj["objectName"].toString() == w.objectName());
        REQUIRE(jobj["displayName"].toString() == w.getDisplayName());
        REQUIRE(jobj["range"].toInt() == w.getRange());
        REQUIRE(jobj["damages"].isObject() == true);

        QJsonObject damages(jobj["damages"].toObject());
        REQUIRE(damages.size() == 1);
        REQUIRE(damages[dt.objectName()].toInt() == w.getDamage(&dt));
    }
}

TEST_CASE("World can be serialized to JSON", "[JsonSerializer]")
{
    core::World w{nullptr};
    w.setObjectName("world1");
    w.setDisplayName("World 1");

    core::DamageType dt1{nullptr};
    dt1.setObjectName("damageType1");
    dt1.setDisplayName("DamageType 1");

    core::DamageType dt2{nullptr};
    dt2.setObjectName("damageType2");
    dt2.setDisplayName("DamageType 2");

    w.setDamageTypes({&dt1, &dt2});

    core::Armor a1{nullptr};
    a1.setObjectName("armor1");
    a1.setDisplayName("Armor 1");
    a1.setDefense(&dt1, 22);

    w.setArmors({&a1});

    core::Weapon w1{nullptr};
    w1.setObjectName("weapon1");
    w1.setDisplayName("Weapon 1");
    w1.setRange(0);
    w1.setDamage(&dt1, 10);
    w1.setDamage(&dt2, 5);

    core::Weapon w2{nullptr};
    w2.setObjectName("weapon2");
    w2.setDisplayName("Weapon 2");
    w2.setRange(2);
    w2.setDamage(&dt2, 12);

    w.setWeapons({&w1, &w2});

    SECTION("serializing World")
    {
        const QJsonObject jobj(serialize(&w));

        REQUIRE(jobj["objectName"].toString() == w.objectName());
        REQUIRE(jobj["displayName"].toString() == w.getDisplayName());

        REQUIRE(jobj["damageTypes"].isArray() == true);
        SECTION("serializing DamageTypes")
        {
            const QJsonArray dts(jobj["damageTypes"].toArray());

            REQUIRE(dts.size() == 2);
            REQUIRE(dts[0].isObject() == true);
            REQUIRE(dts[1].isObject() == true);

            const QJsonObject dt1j(dts[0].toObject());
            REQUIRE(dt1j["objectName"].toString() == dt1.objectName());
            REQUIRE(dt1j["displayName"].toString() == dt1.getDisplayName());

            const QJsonObject dt2j(dts[1].toObject());
            REQUIRE(dt2j["objectName"].toString() == dt2.objectName());
            REQUIRE(dt2j["displayName"].toString() == dt2.getDisplayName());
        }

        REQUIRE(jobj["armors"].isArray() == true);
        SECTION("serializing Armors")
        {
            const QJsonArray as(jobj["armors"].toArray());

            REQUIRE(as.size() == 1);
            REQUIRE(as[0].isObject() == true);

            const QJsonObject a1j(as[0].toObject());
            REQUIRE(a1j["objectName"].toString() == a1.objectName());
            REQUIRE(a1j["displayName"].toString() == a1.getDisplayName());
            REQUIRE(a1j["defenses"].isObject() == true);

            const QJsonObject ds(a1j["defenses"].toObject());

            REQUIRE(ds[dt1.objectName()] == a1.getDefense(&dt1));
        }

        REQUIRE(jobj["weapons"].isArray() == true);
        SECTION("serializing Weapons")
        {
            const QJsonArray ws(jobj["weapons"].toArray());

            REQUIRE(ws.size() == 2);
            REQUIRE(ws[0].isObject() == true);
            REQUIRE(ws[1].isObject() == true);

            const QJsonObject w1j(ws[0].toObject());
            REQUIRE(w1j["objectName"].toString() == w1.objectName());
            REQUIRE(w1j["displayName"].toString() == w1.getDisplayName());
            REQUIRE(w1j["range"].toInt() == w1.getRange());
            REQUIRE(w1j["damages"].isObject() == true);

            const QJsonObject ds1(w1j["damages"].toObject());
            REQUIRE(ds1[dt1.objectName()].toInt() == w1.getDamage(&dt1));
            REQUIRE(ds1[dt2.objectName()].toInt() == w1.getDamage(&dt2));

            const QJsonObject w2j(ws[1].toObject());
            REQUIRE(w2j["objectName"].toString() == w2.objectName());
            REQUIRE(w2j["displayName"].toString() == w2.getDisplayName());
            REQUIRE(w2j["range"].toInt() == w2.getRange());
            REQUIRE(w2j["damages"].isObject() == true);

            const QJsonObject ds2(w2j["damages"].toObject());
            REQUIRE(ds2[dt1.objectName()].toInt() == w2.getDamage(&dt1));
        }
    }
}

TEST_CASE("WorldSurface can be serialized to JSON", "[JsonSerializer]")
{
    core::WorldSurface ws(nullptr);
    ws.setObjectName("worldSurface1");
    ws.setDisplayName("WorldSurface 1");
    ws.setTileSize(QSize(118, 128));

    SECTION("serializing Weapon")
    {
        QJsonObject jobj(serialize(&ws));

        REQUIRE(jobj["objectName"].toString() == ws.objectName());
        REQUIRE(jobj["displayName"].toString() == ws.getDisplayName());
        REQUIRE(jobj["tileWidth"].toInt() == ws.getTileWidth());
        REQUIRE(jobj["tileHeight"].toInt() == ws.getTileHeight());
    }
}
