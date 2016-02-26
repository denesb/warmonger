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

TEST_CASE("Armor can be serialized to JSON", "[JsonSerializer]")
{
    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    core::Armor *a = world->getArmors()[0];

    SECTION("serializing Armor")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeArmor(a));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == a->objectName());
        REQUIRE(jobj["displayName"].toString() == a->getDisplayName());
        REQUIRE(jobj["defenses"].isObject() == true);
        objectEqualsMap(jobj["defenses"].toObject(), a->getDefenses());
    }
}

TEST_CASE("DamageType can be serialized to JSON", "[JsonSerializer]")
{
    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    core::DamageType *dt = world->getDamageTypes()[0];

    SECTION("serializing DamageType")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeDamageType(dt));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == dt->objectName());
        REQUIRE(jobj["displayName"].toString() == dt->getDisplayName());
    }
}

TEST_CASE("Faction can be serialized to JSON", "[JsonSerializer]")
{
    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    core::Faction *f = world->getFactions()[0];

    SECTION("serializing Faction")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeFaction(f));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == f->objectName());
        REQUIRE(jobj["displayName"].toString() == f->getDisplayName());
        REQUIRE(jobj["unitTypes"].isArray() == true);
        arrayEqualsList(jobj["unitTypes"].toArray(), f->getUnitTypes());

        REQUIRE(jobj["recruits"].isObject() == true);

        const QJsonObject jrecruits = jobj["recruits"].toObject();
        const QMap<core::SettlementType *, QList<core::UnitType *>> recruits =
            f->getRecruits();

        REQUIRE(jrecruits.size() == recruits.size());

        for (core::SettlementType *st : recruits.keys())
        {
            REQUIRE(jrecruits[st->objectName()].isArray() == true);
            arrayEqualsList(jrecruits[st->objectName()].toArray(), recruits[st]);
        }
    }
}

TEST_CASE("Map can be serialized to JSON", "[JsonSerializer]")
{
    const QPair<core::Map *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::Map> m{maps.first};
    const QJsonObject jobj{maps.second};
    const std::unique_ptr<core::World> world{m->getWorld()};

    SECTION("serializing Map")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeMap(m.get()));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == m->objectName());
        REQUIRE(jobj["displayName"].toString() == m->getDisplayName());
        REQUIRE(jobj["world"].toString() == m->getWorld()->objectName());
        REQUIRE(jobj["mapNodeIndex"].toInt() == m->getMapNodeIndex());
        REQUIRE(jobj["settlementIndex"].toInt() == m->getSettlementIndex());
        REQUIRE(jobj["unitIndex"].toInt() == m->getUnitIndex());

        REQUIRE(jobj["mapNodes"].isArray() == true);
        SECTION("serializing MapNodes")
        {
            const QJsonArray jmns(jobj["mapNodes"].toArray());
            const QList<core::MapNode *> mns(m->getMapNodes());

            REQUIRE(mns.size() == jmns.size());
            for (int i = 0; i < mns.size(); i++)
            {
                REQUIRE(jmns[i].isObject() == true);
                const QJsonObject jmn(jmns[i].toObject());
                const core::MapNode *mn = mns[i];
                REQUIRE(jmn["objectName"].toString() == mn->objectName());
                REQUIRE(jmn["displayName"].toString() == mn->getDisplayName());
                REQUIRE(jmn["terrainType"].toString() == mn->getTerrainType()->objectName());
            }
        }

        REQUIRE(jobj["players"].isArray() == true);
        SECTION("serializing Players")
        {
            const QJsonArray jps(jobj["players"].toArray());
            const QList<core::Player *> ps(m->getPlayers());

            REQUIRE(ps.size() == jps.size());
            for (int i = 0; i < ps.size(); i++)
            {
                REQUIRE(jps[i].isObject() == true);
                const QJsonObject jp(jps[i].toObject());
                const core::Player *p = ps[i];
                REQUIRE(jp["objectName"].toString() == p->objectName());
                REQUIRE(jp["displayName"].toString() == p->getDisplayName());
                REQUIRE(jp["color"].toString() == p->getColor().name());
                REQUIRE(jp["goldBalance"].toInt() == p->getGoldBalance());
                REQUIRE(jp["faction"].toString() == p->getFaction()->objectName());
            }
        }

        REQUIRE(jobj["settlements"].isArray() == true);
        SECTION("serializing Settlements")
        {
            const QJsonArray jss(jobj["settlements"].toArray());
            const QList<core::Settlement *> ss(m->getSettlements());

            REQUIRE(ss.size() == jss.size());
            for (int i = 0; i < ss.size(); i++)
            {
                REQUIRE(jss[i].isObject() == true);
                const QJsonObject js(jss[i].toObject());
                const core::Settlement *s = ss[i];
                REQUIRE(js["objectName"].toString() == s->objectName());
                REQUIRE(js["displayName"].toString() == s->getDisplayName());
                REQUIRE(js["type"].toString() == s->getType()->objectName());
                REQUIRE(js["mapNode"].toString() == s->getMapNode()->objectName());
                REQUIRE(js["owner"].toString() == s->getOwner()->objectName());
            }
        }

        REQUIRE(jobj["units"].isArray() == true);
        SECTION("serializing Units")
        {
            const QJsonArray jus(jobj["units"].toArray());
            const QList<core::Unit *> us(m->getUnits());

            REQUIRE(us.size() == jus.size());
            for (int i = 0; i < us.size(); i++)
            {
                REQUIRE(jus[i].isObject() == true);
                const QJsonObject ju(jus[i].toObject());
                const core::Unit *u = us[i];
                REQUIRE(ju["objectName"].toString() == u->objectName());
                REQUIRE(ju["displayName"].toString() == u->getDisplayName());
                REQUIRE(ju["type"].toString() == u->getType()->objectName());
                REQUIRE(ju["mapNode"].toString() == u->getMapNode()->objectName());
                REQUIRE(ju["owner"].toString() == u->getOwner()->objectName());
                REQUIRE(ju["experiencePoints"].toInt() == u->getExperiencePoints());
                REQUIRE(ju["hitPoints"].toDouble() == u->getHitPoints());
                REQUIRE(ju["movementPoints"].toDouble() == u->getMovementPoints());
            }
        }
    }
}

TEST_CASE("MapNode can be serialized to JSON", "[JsonSerializer]")
{
    const QPair<core::Map *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::Map> map{maps.first};
    const QJsonObject jobj{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const core::MapNode *mn = map->getMapNodes()[0];

    SECTION("serializing MapNode")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeMapNode(mn));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"] == mn->objectName());
        REQUIRE(jobj["displayName"] == mn->getDisplayName());
        REQUIRE(jobj["terrainType"] == mn->getTerrainType()->objectName());
    }
}

TEST_CASE("Player can be serialized to JSON", "[JsonSerializer]")
{
    const QPair<core::Map *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::Map> map{maps.first};
    const QJsonObject jobj{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const core::Player *p = map->getPlayers()[0];

    SECTION("serializing Player")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializePlayer(p));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == p->objectName());
        REQUIRE(jobj["displayName"].toString() == p->getDisplayName());
        REQUIRE(jobj["color"].toString() == p->getColor().name());
        REQUIRE(jobj["goldBalance"].toInt() == p->getGoldBalance());
        REQUIRE(jobj["faction"].toString() == p->getFaction()->objectName());
    }
}

TEST_CASE("Settlement can be serialized to JSON", "[JsonSerializer]")
{
    const QPair<core::Map *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::Map> map{maps.first};
    const QJsonObject jobj{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const core::Settlement *s = map->getSettlements()[0];

    SECTION("serializing Settlement")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeSettlement(s));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == s->objectName());
        REQUIRE(jobj["displayName"].toString() == s->getDisplayName());
        REQUIRE(jobj["type"].toString() == s->getType()->objectName());
        REQUIRE(jobj["mapNode"].toString() == s->getMapNode()->objectName());
        REQUIRE(jobj["owner"].toString() == s->getOwner()->objectName());
    }
}

TEST_CASE("SettlementType can be serialized to JSON", "[JsonSerializer]")
{
    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    core::SettlementType *st = world->getSettlementTypes()[0];

    SECTION("serializing SettlementType")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeSettlementType(st));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == st->objectName());
        REQUIRE(jobj["displayName"].toString() == st->getDisplayName());
        REQUIRE(jobj["goldPerTurn"].toInt() == st->getGoldPerTurn());
        REQUIRE(jobj["recruits"].isArray() == true);

        const QJsonArray recruits = jobj["recruits"].toArray();
        arrayEqualsList(recruits, st->getRecruits());
    }
}

TEST_CASE("TerrainType can be serialized to JSON", "[JsonSerializer]")
{
    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    core::TerrainType *tt = world->getTerrainTypes()[0];

    SECTION("serializing TerrainType")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeTerrainType(tt));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == tt->objectName());
        REQUIRE(jobj["displayName"].toString() == tt->getDisplayName());
    }
}

TEST_CASE("Unit can be serialized to JSON", "[JsonSerializer]")
{
    const QPair<core::Map *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::Map> map{maps.first};
    const QJsonObject jobj{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const core::Unit *u = map->getUnits()[0];

    SECTION("serializing Unit")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeUnit(u));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == u->objectName());
        REQUIRE(jobj["displayName"].toString() == u->getDisplayName());
        REQUIRE(jobj["type"].toString() == u->getType()->objectName());
        REQUIRE(jobj["mapNode"].toString() == u->getMapNode()->objectName());
        REQUIRE(jobj["owner"].toString() == u->getOwner()->objectName());
        REQUIRE(jobj["experiencePoints"].toDouble() == u->getExperiencePoints());
        REQUIRE(jobj["hitPoints"].toDouble() == u->getHitPoints());
        REQUIRE(jobj["movementPoints"].toDouble() == u->getMovementPoints());
    }
}

TEST_CASE("UnitClass can be serialized to JSON", "[JsonSerializer]")
{
    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    core::UnitClass *uc = world->getUnitClasses()[0];

    SECTION("serializing UnitClass")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeUnitClass(uc));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == uc->objectName());
        REQUIRE(jobj["displayName"].toString() == uc->getDisplayName());
        REQUIRE(jobj["movementPoints"].toInt() == uc->getMovementPoints());
        REQUIRE(jobj["movementCosts"].isObject() == true);
        REQUIRE(jobj["attacks"].isObject() == true);
        REQUIRE(jobj["defenses"].isObject() == true);

        const QJsonObject movementCosts(jobj["movementCosts"].toObject());
        objectEqualsMap(movementCosts, uc->getMovementCosts());

        const QJsonObject attacks(jobj["attacks"].toObject());
        objectEqualsMap(attacks, uc->getAttacks());

        const QJsonObject defenses(jobj["defenses"].toObject());
        objectEqualsMap(defenses, uc->getDefenses());
    }
}

TEST_CASE("UnitLevel can be serialized to JSON", "[JsonSerializer]")
{
    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    core::UnitLevel *ul = world->getUnitLevels()[0];

    SECTION("serializing UnitLevel")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeUnitLevel(ul));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == ul->objectName());
        REQUIRE(jobj["displayName"].toString() == ul->getDisplayName());
        REQUIRE(jobj["index"].toInt() == ul->getIndex());
        REQUIRE(jobj["experiencePoints"].toInt() == ul->getExperiencePoints());
    }
}

TEST_CASE("UnitType can be serialized to JSON", "[JsonSerializer]")
{
    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    core::UnitType *ut = world->getUnitTypes()[0];

    SECTION("serializing UnitType")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeUnitType(ut));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == ut->objectName());
        REQUIRE(jobj["displayName"].toString() == ut->getDisplayName());
        REQUIRE(jobj["class"].toString() == ut->getClass()->objectName());
        REQUIRE(jobj["level"].toString() == ut->getLevel()->objectName());
        REQUIRE(jobj["hitPoints"].toInt() == ut->getHitPoints());
        REQUIRE(jobj["recruitmentCost"].toInt() == ut->getRecruitmentCost());
        REQUIRE(jobj["upkeepCost"].toInt() == ut->getUpkeepCost());
        REQUIRE(jobj["armor"].toString() == ut->getArmor()->objectName());
        REQUIRE(jobj["weapons"].isArray() == true);
        REQUIRE(jobj["upgrades"].isArray() == true);

        const QJsonArray weapons(jobj["weapons"].toArray());
        arrayEqualsList(weapons, ut->getWeapons());

        const QJsonArray upgrades(jobj["upgrades"].toArray());
        arrayEqualsList(upgrades, ut->getUpgrades());
    }
}

TEST_CASE("Weapon can be serialized to JSON", "[JsonSerializer]")
{
    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    core::Weapon *w = world->getWeapons()[0];

    SECTION("serializing Weapon")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeWeapon(w));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == w->objectName());
        REQUIRE(jobj["displayName"].toString() == w->getDisplayName());
        REQUIRE(jobj["range"].toInt() == w->getRange());
        REQUIRE(jobj["damages"].isObject() == true);

        const QJsonObject damages(jobj["damages"].toObject());
        objectEqualsMap(damages, w->getDamages());
    }
}

TEST_CASE("World can be serialized to JSON", "[JsonSerializer]")
{
    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    SECTION("serializing World")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeWorld(world.get()));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == world->objectName());
        REQUIRE(jobj["displayName"].toString() == world->getDisplayName());

        REQUIRE(jobj["damageTypes"].isArray() == true);
        SECTION("serializing DamageTypes")
        {
            const QJsonArray dts(jobj["damageTypes"].toArray());

            REQUIRE(dts.size() == 2);
            REQUIRE(dts[0].isObject() == true);
            REQUIRE(dts[1].isObject() == true);

            const core::DamageType *dt1 = world->getDamageTypes()[0];
            const QJsonObject dt1j(dts[0].toObject());
            REQUIRE(dt1j["objectName"].toString() == dt1->objectName());
            REQUIRE(dt1j["displayName"].toString() == dt1->getDisplayName());

            const core::DamageType *dt2 = world->getDamageTypes()[1];
            const QJsonObject dt2j(dts[1].toObject());
            REQUIRE(dt2j["objectName"].toString() == dt2->objectName());
            REQUIRE(dt2j["displayName"].toString() == dt2->getDisplayName());
        }

        REQUIRE(jobj["armors"].isArray() == true);
        SECTION("serializing Armors")
        {
            const QJsonArray as(jobj["armors"].toArray());

            REQUIRE(as.size() == 1);
            REQUIRE(as[0].isObject() == true);

            const core::Armor *a1 = world->getArmors()[0];
            const QJsonObject a1j(as[0].toObject());
            REQUIRE(a1j["objectName"].toString() == a1->objectName());
            REQUIRE(a1j["displayName"].toString() == a1->getDisplayName());
            REQUIRE(a1j["defenses"].isObject() == true);

            const QJsonObject ds(a1j["defenses"].toObject());
            objectEqualsMap(ds, a1->getDefenses());
        }

        REQUIRE(jobj["weapons"].isArray() == true);
        SECTION("serializing Weapons")
        {
            const QJsonArray ws(jobj["weapons"].toArray());

            REQUIRE(ws.size() == 2);
            REQUIRE(ws[0].isObject() == true);
            REQUIRE(ws[1].isObject() == true);

            const core::Weapon *w1 = world->getWeapons()[0];
            const QJsonObject w1j(ws[0].toObject());
            REQUIRE(w1j["objectName"].toString() == w1->objectName());
            REQUIRE(w1j["displayName"].toString() == w1->getDisplayName());
            REQUIRE(w1j["range"].toInt() == w1->getRange());
            REQUIRE(w1j["damages"].isObject() == true);

            const QJsonObject ds1(w1j["damages"].toObject());
            objectEqualsMap(ds1, w1->getDamages());

            const core::Weapon *w2 = world->getWeapons()[1];
            const QJsonObject w2j(ws[1].toObject());
            REQUIRE(w2j["objectName"].toString() == w2->objectName());
            REQUIRE(w2j["displayName"].toString() == w2->getDisplayName());
            REQUIRE(w2j["range"].toInt() == w2->getRange());
            REQUIRE(w2j["damages"].isObject() == true);

            const QJsonObject ds2(w2j["damages"].toObject());
            objectEqualsMap(ds2, w2->getDamages());
        }

        REQUIRE(jobj["terrainTypes"].isArray() == true);
        SECTION("serializing TerrainTypes")
        {
            const QJsonArray tts(jobj["terrainTypes"].toArray());

            REQUIRE(tts.size() == 1);
            REQUIRE(tts[0].isObject() == true);

            const core::TerrainType *tt1 = world->getTerrainTypes()[0];
            const QJsonObject tt1j(tts[0].toObject());
            REQUIRE(tt1j["objectName"].toString() == tt1->objectName());
            REQUIRE(tt1j["displayName"].toString() == tt1->getDisplayName());
        }

        REQUIRE(jobj["unitClasses"].isArray() == true);
        SECTION("serializing UnitClasses")
        {
            const QJsonArray ucs(jobj["unitClasses"].toArray());

            REQUIRE(ucs.size() == 1);
            REQUIRE(ucs[0].isObject() == true);

            const core::UnitClass *uc1 = world->getUnitClasses()[0];
            const QJsonObject uc1j(ucs[0].toObject());
            REQUIRE(uc1j["objectName"].toString() == uc1->objectName());
            REQUIRE(uc1j["displayName"].toString() == uc1->getDisplayName());

            REQUIRE(uc1j["movementCosts"].isObject() == true);
            const QJsonObject mcs1(uc1j["movementCosts"].toObject());
            objectEqualsMap(mcs1, uc1->getMovementCosts());

            REQUIRE(uc1j["attacks"].isObject() == true);
            const QJsonObject as1(uc1j["attacks"].toObject());
            objectEqualsMap(as1, uc1->getAttacks());

            REQUIRE(uc1j["defenses"].isObject() == true);
            const QJsonObject ds1(uc1j["defenses"].toObject());
            objectEqualsMap(ds1, uc1->getDefenses());
        }

        REQUIRE(jobj["unitLevels"].isArray() == true);
        SECTION("serializing UnitLevels")
        {
            const QJsonArray uls(jobj["unitLevels"].toArray());

            REQUIRE(uls.size() == 1);
            REQUIRE(uls[0].isObject() == true);

            const core::UnitLevel *ul1 = world->getUnitLevels()[0];
            const QJsonObject ul1j(uls[0].toObject());
            REQUIRE(ul1j["objectName"].toString() == ul1->objectName());
            REQUIRE(ul1j["displayName"].toString() == ul1->getDisplayName());
            REQUIRE(ul1j["experiencePoints"].toInt() == ul1->getExperiencePoints());
            REQUIRE(ul1j["index"].toInt() == ul1->getIndex());
        }

        REQUIRE(jobj["unitTypes"].isArray() == true);
        SECTION("serializing UnitTypes")
        {
            const QJsonArray uts(jobj["unitTypes"].toArray());

            REQUIRE(uts.size() == world->getUnitTypes().size());
            REQUIRE(uts[0].isObject() == true);

            const core::UnitType *ut1 = world->getUnitTypes()[0];
            const QJsonObject ut1j(uts[0].toObject());
            REQUIRE(ut1j["objectName"].toString() == ut1->objectName());
            REQUIRE(ut1j["displayName"].toString() == ut1->getDisplayName());
            REQUIRE(ut1j["class"].toString() == ut1->getClass()->objectName());
            REQUIRE(ut1j["level"].toString() == ut1->getLevel()->objectName());
            REQUIRE(ut1j["hitPoints"].toInt() == ut1->getHitPoints());
            REQUIRE(ut1j["recruitmentCost"].toInt() == ut1->getRecruitmentCost());
            REQUIRE(ut1j["upkeepCost"].toInt() == ut1->getUpkeepCost());
            REQUIRE(ut1j["armor"].toString() == ut1->getArmor()->objectName());
            REQUIRE(ut1j["weapons"].isArray() == true);
            REQUIRE(ut1j["upgrades"].isArray() == true);

            const QJsonArray ws(ut1j["weapons"].toArray());
            arrayEqualsList(ws, ut1->getWeapons());

            const QJsonArray us(ut1j["upgrades"].toArray());
            arrayEqualsList(us, ut1->getUpgrades());
        }

        REQUIRE(jobj["settlementTypes"].isArray() == true);
        SECTION("serializing SettlementTypes")
        {
            const QJsonArray sts(jobj["settlementTypes"].toArray());

            REQUIRE(sts.size() == 1);
            REQUIRE(sts[0].isObject() == true);

            const core::SettlementType *st1 = world->getSettlementTypes()[0];
            const QJsonObject st1j(sts[0].toObject());
            REQUIRE(st1j["objectName"].toString() == st1->objectName());
            REQUIRE(st1j["displayName"].toString() == st1->getDisplayName());
            REQUIRE(st1j["goldPerTurn"].toInt() == st1->getGoldPerTurn());
            REQUIRE(st1j["recruits"].isArray() == true);

            const QJsonArray rs(st1j["recruits"].toArray());
            arrayEqualsList(rs, st1->getRecruits());
        }

        REQUIRE(jobj["factions"].isArray() == true);
        SECTION("serializing Factions")
        {
            const QJsonArray fs(jobj["factions"].toArray());

            REQUIRE(fs.size() == 1);
            REQUIRE(fs[0].isObject() == true);

            const core::Faction *f1 = world->getFactions()[0];
            const QJsonObject f1j(fs[0].toObject());
            REQUIRE(f1j["objectName"].toString() == f1->objectName());
            REQUIRE(f1j["displayName"].toString() == f1->getDisplayName());
            REQUIRE(f1j["unitTypes"].isArray() == true);
            REQUIRE(f1j["recruits"].isObject() == true);

            const QJsonArray uts(f1j["unitTypes"].toArray());
            arrayEqualsList(uts, f1->getUnitTypes());

            const QJsonObject rs(f1j["recruits"].toObject());
            const QMap<core::SettlementType *, QList<core::UnitType *>> ors =
                f1->getRecruits();

            REQUIRE(rs.size() == ors.size());

            for (core::SettlementType *st : ors.keys())
            {
                REQUIRE(rs[st->objectName()].isArray() == true);
                arrayEqualsList(rs[st->objectName()].toArray(), ors[st]);
            }
        }
    }
}

TEST_CASE("WorldSurface can be serialized to JSON", "[JsonSerializer]")
{
    core::WorldSurface ws(nullptr);
    ws.setObjectName("worldSurface1");
    ws.setDisplayName("WorldSurface 1");
    ws.setTileSize(QSize(118, 128));

    SECTION("serializing WorldSurface")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeWorldSurface(&ws));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == ws.objectName());
        REQUIRE(jobj["displayName"].toString() == ws.getDisplayName());
        REQUIRE(jobj["tileWidth"].toInt() == ws.getTileWidth());
        REQUIRE(jobj["tileHeight"].toInt() == ws.getTileHeight());
    }
}
