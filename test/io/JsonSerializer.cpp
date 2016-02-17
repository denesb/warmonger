#include <memory>

#include <QJsonDocument>

#include "core/Armor.h"
#include "core/DamageType.h"
#include "core/Faction.h"
#include "core/Map.h"
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
#include "test/catch.hpp"
#include "test/Util.h"

using namespace warmonger;

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
        const QJsonObject jobj(serialize(&a));

        REQUIRE(jobj["objectName"].toString() == a.objectName());
        REQUIRE(jobj["displayName"].toString() == a.getDisplayName());
        REQUIRE(jobj["defenses"].isObject() == true);

        const QJsonObject defenses(jobj["defenses"].toObject());
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
        const QJsonObject jobj(serialize(&dt));

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
        const QJsonObject jobj(serialize(&f));

        REQUIRE(jobj["objectName"].toString() == f.objectName());
        REQUIRE(jobj["displayName"].toString() == f.getDisplayName());
        REQUIRE(jobj["unitTypes"].isArray() == true);

        const QJsonArray unitTypes = jobj["unitTypes"].toArray();
        REQUIRE(unitTypes[0].toString() == ut1.objectName());
        REQUIRE(unitTypes[1].toString() == ut2.objectName());

        REQUIRE(jobj["recruits"].isObject() == true);

        const QJsonObject recruits = jobj["recruits"].toObject();
        REQUIRE(recruits[st.objectName()].isArray() == true);
    }
}

TEST_CASE("Map can be serialized to JSON", "[JsonSerializer]")
{
    core::Map m{nullptr};
    m.setObjectName("Map1");
    m.setDisplayName("Map 1");

    core::World w1{nullptr};
    w1.setObjectName("world1");

    m.setWorld(&w1);

    m.setMapNodeIndex(999);
    m.setSettlementIndex(99);
    m.setUnitIndex(9);

    core::TerrainType tt1{nullptr};
    tt1.setObjectName("terrainType1");

    core::MapNode mn1{nullptr};
    mn1.setObjectName("mapNode1");
    mn1.setDisplayName("mapNode1");
    mn1.setTerrainType(&tt1);

    m.setMapNodes({&mn1});

    core::Faction f1{nullptr};
    f1.setObjectName("faction1");

    core::Player p1{nullptr};
    p1.setObjectName("player1");
    p1.setDisplayName("Player 1");
    p1.setColor(QColor("red"));
    p1.setGoldBalance(3563465);
    p1.setFaction(&f1);

    m.setPlayers({&p1});

    core::SettlementType st1{nullptr};
    st1.setObjectName("settlementType1");

    core::Settlement s1{nullptr};
    s1.setObjectName("settlement1");
    s1.setDisplayName("Settlement 1");
    s1.setType(&st1);
    s1.setMapNode(&mn1);
    s1.setOwner(&p1);

    m.setSettlements({&s1});

    core::UnitType ut1{nullptr};
    ut1.setObjectName("unitType1");

    core::Unit u1{nullptr};
    u1.setObjectName("unit1");
    u1.setDisplayName("Unit 1");
    u1.setRank(core::Unit::Leader);
    u1.setType(&ut1);
    u1.setMapNode(&mn1);
    u1.setOwner(&p1);
    u1.setExperiencePoints(10.0);
    u1.setHitPoints(20.0);
    u1.setMovementPoints(30.0);

    m.setUnits({&u1});

    SECTION("serializing Map")
    {
        QJsonObject jobj(serialize(&m));

        REQUIRE(jobj["objectName"].toString() == m.objectName());
        REQUIRE(jobj["displayName"].toString() == m.getDisplayName());
        REQUIRE(jobj["world"].toString() == w1.objectName());
        REQUIRE(jobj["mapNodeIndex"].toInt() == m.getMapNodeIndex());
        REQUIRE(jobj["settlementIndex"].toInt() == m.getSettlementIndex());
        REQUIRE(jobj["unitIndex"].toInt() == m.getUnitIndex());

        REQUIRE(jobj["mapNodes"].isArray() == true);
        SECTION("serializing MapNodes")
        {
            const QJsonArray mns(jobj["mapNodes"].toArray());

            REQUIRE(mns.size() == 1);
            REQUIRE(mns[0].isObject() == true);

            const QJsonObject mn1j(mns[0].toObject());
            REQUIRE(mn1j["objectName"].toString() == mn1.objectName());
            REQUIRE(mn1j["displayName"].toString() == mn1.getDisplayName());
            REQUIRE(mn1j["terrainType"].toString() == tt1.objectName());
        }

        REQUIRE(jobj["players"].isArray() == true);
        SECTION("serializing Players")
        {
            const QJsonArray ps(jobj["players"].toArray());

            REQUIRE(ps.size() == 1);
            REQUIRE(ps[0].isObject() == true);

            const QJsonObject p1j(ps[0].toObject());
            REQUIRE(p1j["objectName"].toString() == p1.objectName());
            REQUIRE(p1j["displayName"].toString() == p1.getDisplayName());
            REQUIRE(p1j["color"].toString() == p1.getColor().name());
            REQUIRE(p1j["goldBalance"].toInt() == p1.getGoldBalance());
            REQUIRE(p1j["faction"].toString() == p1.getFaction()->objectName());
        }

        REQUIRE(jobj["settlements"].isArray() == true);
        SECTION("serializing Settlements")
        {
            const QJsonArray ss(jobj["settlements"].toArray());

            REQUIRE(ss.size() == 1);
            REQUIRE(ss[0].isObject() == true);

            const QJsonObject s1j(ss[0].toObject());
            REQUIRE(s1j["objectName"].toString() == s1.objectName());
            REQUIRE(s1j["displayName"].toString() == s1.getDisplayName());
            REQUIRE(s1j["type"].toString() == s1.getType()->objectName());
            REQUIRE(s1j["mapNode"].toString() == s1.getMapNode()->objectName());
            REQUIRE(s1j["owner"].toString() == s1.getOwner()->objectName());
        }

        REQUIRE(jobj["units"].isArray() == true);
        SECTION("serializing Units")
        {
            const QJsonArray us(jobj["units"].toArray());

            REQUIRE(us.size() == 1);
            REQUIRE(us[0].isObject() == true);

            const QJsonObject u1j(us[0].toObject());
            REQUIRE(u1j["objectName"].toString() == u1.objectName());
            REQUIRE(u1j["displayName"].toString() == u1.getDisplayName());
            REQUIRE(u1j["rank"].toString() == core::Unit::rank2str[u1.getRank()]);
            REQUIRE(u1j["type"].toString() == u1.getType()->objectName());
            REQUIRE(u1j["mapNode"].toString() == u1.getMapNode()->objectName());
            REQUIRE(u1j["owner"].toString() == u1.getOwner()->objectName());
            REQUIRE(u1j["experiencePoints"].toInt() == u1.getExperiencePoints());
            REQUIRE(u1j["hitPoints"].toDouble() == u1.getHitPoints());
            REQUIRE(u1j["movementPoints"].toDouble() == u1.getMovementPoints());
        }
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
        const QJsonObject jobj(serialize(&n));

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
        const QJsonObject jobj(serialize(&p));

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
    s.setObjectName("settlement1");
    s.setDisplayName("Settlement 1");

    core::SettlementType st1{nullptr};
    st1.setObjectName("settlementType1");

    s.setType(&st1);

    core::MapNode mn{nullptr};
    mn.setObjectName("mapNode1");

    s.setMapNode(&mn);

    core::Player p{nullptr};
    p.setObjectName("player1");

    s.setOwner(&p);

    SECTION("serializing Settlement")
    {
        const QJsonObject jobj(serialize(&s));

        REQUIRE(jobj["objectName"].toString() == s.objectName());
        REQUIRE(jobj["displayName"].toString() == s.getDisplayName());
        REQUIRE(jobj["type"].toString() == s.getType()->objectName());
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
        const QJsonObject jobj(serialize(&st));

        REQUIRE(jobj["objectName"].toString() == st.objectName());
        REQUIRE(jobj["displayName"].toString() == st.getDisplayName());
        REQUIRE(jobj["goldPerTurn"].toInt() == st.getGoldPerTurn());
        REQUIRE(jobj["recruits"].isArray() == true);

        const QJsonArray recruits = jobj["recruits"].toArray();
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
        const QJsonObject jobj(serialize(&tt));

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
        const QJsonObject jobj(serialize(&u));

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
        const QJsonObject jobj(serialize(&uc));

        REQUIRE(jobj["objectName"].toString() == uc.objectName());
        REQUIRE(jobj["displayName"].toString() == uc.getDisplayName());
        REQUIRE(jobj["movementPoints"].toInt() == uc.getMovementPoints());
        REQUIRE(jobj["movementCosts"].isObject() == true);
        REQUIRE(jobj["attacks"].isObject() == true);
        REQUIRE(jobj["defenses"].isObject() == true);

        const QJsonObject mc(jobj["movementCosts"].toObject());
        REQUIRE(mc.size() == 1);
        REQUIRE(mc[tt.objectName()].toInt() == uc.getMovementCost(&tt));

        const QJsonObject attacks(jobj["attacks"].toObject());
        REQUIRE(attacks.size() == 1);
        REQUIRE(attacks[tt.objectName()].toInt() == uc.getAttack(&tt));

        const QJsonObject defenses(jobj["defenses"].toObject());
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
        const QJsonObject jobj(serialize(&ul));

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

    ut.setWeapons({&w1, &w2});

    core::UnitType ut2(nullptr);
    ut2.setObjectName("unitType2");

    ut.setUpgrades({&ut2});

    SECTION("serializing UnitType")
    {
        const QJsonObject jobj(serialize(&ut));

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

        const QJsonArray weapons(jobj["weapons"].toArray());
        REQUIRE(weapons.size() == 2);
        REQUIRE(weapons[0].toString() == w1.objectName());
        REQUIRE(weapons[1].toString() == w2.objectName());

        const QJsonArray upgrades(jobj["upgrades"].toArray());
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
        const QJsonObject jobj(serialize(&w));

        REQUIRE(jobj["objectName"].toString() == w.objectName());
        REQUIRE(jobj["displayName"].toString() == w.getDisplayName());
        REQUIRE(jobj["range"].toInt() == w.getRange());
        REQUIRE(jobj["damages"].isObject() == true);

        const QJsonObject damages(jobj["damages"].toObject());
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

    core::TerrainType tt1{nullptr};
    tt1.setObjectName("terrainType1");
    tt1.setDisplayName("TerrainType 1");

    w.setTerrainTypes({&tt1});

    core::UnitClass uc1{nullptr};
    uc1.setObjectName("unitClass1");
    uc1.setDisplayName("UnitClass 1");
    uc1.setMovementCost(&tt1, 8);
    uc1.setAttack(&tt1, 10);
    uc1.setDefense(&tt1, 30);

    w.setUnitClasses({&uc1});

    core::UnitLevel ul1{nullptr};
    ul1.setObjectName("unitLevel1");
    ul1.setDisplayName("UnitLevel 1");
    ul1.setExperiencePoints(800);
    ul1.setIndex(1);

    w.setUnitLevels({&ul1});

    core::UnitType ut1{nullptr};
    ut1.setObjectName("unitType1");
    ut1.setDisplayName("UnitType 1");
    ut1.setClass(&uc1);
    ut1.setLevel(&ul1);
    ut1.setHitPoints(32);
    ut1.setRecruitmentCost(200);
    ut1.setUpkeepCost(20);
    ut1.setArmor(&a1);
    ut1.setWeapons({&w1});

    w.setUnitTypes({&ut1});

    core::SettlementType st1{nullptr};
    st1.setObjectName("settlementType1");
    st1.setDisplayName("SettlementType 1");
    st1.setGoldPerTurn(7);
    st1.setRecruits({&ut1});

    w.setSettlementTypes({&st1});

    core::Faction f1{nullptr};
    f1.setObjectName("faction1");
    f1.setDisplayName("Faction 1");
    f1.setUnitTypes({&ut1});

    w.setFactions({&f1});

    QMap<core::SettlementType *, QList<core::UnitType *>> recruits;
    recruits[&st1] = QList<core::UnitType *>({&ut1});
    f1.setRecruits(recruits);

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

        REQUIRE(jobj["terrainTypes"].isArray() == true);
        SECTION("serializing TerrainTypes")
        {
            const QJsonArray tts(jobj["terrainTypes"].toArray());

            REQUIRE(tts.size() == 1);
            REQUIRE(tts[0].isObject() == true);

            const QJsonObject tt1j(tts[0].toObject());
            REQUIRE(tt1j["objectName"].toString() == tt1.objectName());
            REQUIRE(tt1j["displayName"].toString() == tt1.getDisplayName());
        }

        REQUIRE(jobj["unitClasses"].isArray() == true);
        SECTION("serializing UnitClasses")
        {
            const QJsonArray ucs(jobj["unitClasses"].toArray());

            REQUIRE(ucs.size() == 1);
            REQUIRE(ucs[0].isObject() == true);

            const QJsonObject uc1j(ucs[0].toObject());
            REQUIRE(uc1j["objectName"].toString() == uc1.objectName());
            REQUIRE(uc1j["displayName"].toString() == uc1.getDisplayName());

            REQUIRE(uc1j["movementCosts"].isObject() == true);
            const QJsonObject mcs1(uc1j["movementCosts"].toObject());
            REQUIRE(mcs1.size() == 1);
            REQUIRE(mcs1[tt1.objectName()].toInt() == uc1.getMovementCost(&tt1));

            REQUIRE(uc1j["attacks"].isObject() == true);
            const QJsonObject as1(uc1j["attacks"].toObject());
            REQUIRE(as1.size() == 1);
            REQUIRE(as1[tt1.objectName()].toInt() == uc1.getAttack(&tt1));

            REQUIRE(uc1j["defenses"].isObject() == true);
            const QJsonObject ds1(uc1j["defenses"].toObject());
            REQUIRE(ds1.size() == 1);
            REQUIRE(ds1[tt1.objectName()].toInt() == uc1.getDefense(&tt1));
        }

        REQUIRE(jobj["unitLevels"].isArray() == true);
        SECTION("serializing UnitLevels")
        {
            const QJsonArray uls(jobj["unitLevels"].toArray());

            REQUIRE(uls.size() == 1);
            REQUIRE(uls[0].isObject() == true);

            const QJsonObject ul1j(uls[0].toObject());
            REQUIRE(ul1j["objectName"].toString() == ul1.objectName());
            REQUIRE(ul1j["displayName"].toString() == ul1.getDisplayName());
            REQUIRE(ul1j["experiencePoints"].toInt() == ul1.getExperiencePoints());
            REQUIRE(ul1j["index"].toInt() == ul1.getIndex());
        }

        REQUIRE(jobj["unitTypes"].isArray() == true);
        SECTION("serializing UnitTypes")
        {
            const QJsonArray uts(jobj["unitTypes"].toArray());

            REQUIRE(uts.size() == 1);
            REQUIRE(uts[0].isObject() == true);

            const QJsonObject ut1j(uts[0].toObject());
            REQUIRE(ut1j["objectName"].toString() == ut1.objectName());
            REQUIRE(ut1j["displayName"].toString() == ut1.getDisplayName());
            REQUIRE(ut1j["class"].toString() == ut1.getClass()->objectName());
            REQUIRE(ut1j["level"].toString() == ut1.getLevel()->objectName());
            REQUIRE(ut1j["hitPoints"].toInt() == ut1.getHitPoints());
            REQUIRE(ut1j["recruitmentCost"].toInt() == ut1.getRecruitmentCost());
            REQUIRE(ut1j["upkeepCost"].toInt() == ut1.getUpkeepCost());
            REQUIRE(ut1j["armor"].toString() == ut1.getArmor()->objectName());
            REQUIRE(ut1j["weapons"].isArray() == true);

            const QJsonArray ws(ut1j["weapons"].toArray());
            REQUIRE(ws.size() == 1);
            REQUIRE(ws[0].toString() == w1.objectName());

            REQUIRE(ut1j["upgrades"].isArray() == true);

            const QJsonArray us(ut1j["upgrades"].toArray());
            REQUIRE(us.size() == 0);
        }

        REQUIRE(jobj["settlementTypes"].isArray() == true);
        SECTION("serializing SettlementTypes")
        {
            const QJsonArray sts(jobj["settlementTypes"].toArray());

            REQUIRE(sts.size() == 1);
            REQUIRE(sts[0].isObject() == true);

            const QJsonObject st1j(sts[0].toObject());
            REQUIRE(st1j["objectName"].toString() == st1.objectName());
            REQUIRE(st1j["displayName"].toString() == st1.getDisplayName());
            REQUIRE(st1j["goldPerTurn"].toInt() == st1.getGoldPerTurn());
            REQUIRE(st1j["recruits"].isArray() == true);

            const QJsonArray rs(st1j["recruits"].toArray());
            REQUIRE(rs.size() == 1);
            REQUIRE(rs[0].toString() == ut1.objectName());
        }

        REQUIRE(jobj["factions"].isArray() == true);
        SECTION("serializing Factions")
        {
            const QJsonArray fs(jobj["factions"].toArray());

            REQUIRE(fs.size() == 1);
            REQUIRE(fs[0].isObject() == true);

            const QJsonObject f1j(fs[0].toObject());
            REQUIRE(f1j["objectName"].toString() == f1.objectName());
            REQUIRE(f1j["displayName"].toString() == f1.getDisplayName());
            REQUIRE(f1j["unitTypes"].isArray() == true);

            const QJsonArray uts(f1j["unitTypes"].toArray());
            REQUIRE(uts.size() == 1);
            REQUIRE(uts[0].toString() == ut1.objectName());

            REQUIRE(f1j["recruits"].isObject() == true);

            const QJsonObject rs(f1j["recruits"].toObject());
            REQUIRE(rs.size() == 1);
            REQUIRE(rs[st1.objectName()].isArray() == true);

            const QJsonArray recruits(rs[st1.objectName()].toArray());
            REQUIRE(recruits.size() == 1);
            REQUIRE(recruits[0].toString() == ut1.objectName());
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
