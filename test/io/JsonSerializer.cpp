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
            const QJsonArray jdts(jobj["damageTypes"].toArray());
            const QList<core::DamageType *> dts(world->getDamageTypes());

            REQUIRE(dts.size() == jdts.size());
            for (int i = 0; i < dts.size(); i++)
            {
                REQUIRE(jdts[i].isObject() == true);
                const QJsonObject jdt(jdts[i].toObject());
                const core::DamageType *dt = dts[i];
                REQUIRE(jdt["objectName"].toString() == dt->objectName());
                REQUIRE(jdt["displayName"].toString() == dt->getDisplayName());
            }
        }

        REQUIRE(jobj["armors"].isArray() == true);
        SECTION("serializing Armors")
        {
            const QJsonArray jas(jobj["armors"].toArray());
            const QList<core::Armor *> as(world->getArmors());

            REQUIRE(as.size() == jas.size());
            for (int i = 0; i < as.size(); i++)
            {
                REQUIRE(jas[i].isObject() == true);
                const QJsonObject ja(jas[i].toObject());
                const core::Armor *a = as[i];
                REQUIRE(ja["objectName"].toString() == a->objectName());
                REQUIRE(ja["displayName"].toString() == a->getDisplayName());
                REQUIRE(ja["defenses"].isObject() == true);
                objectEqualsMap(ja["defenses"].toObject(), a->getDefenses());
            }
        }

        REQUIRE(jobj["weapons"].isArray() == true);
        SECTION("serializing Weapons")
        {
            const QJsonArray jws(jobj["weapons"].toArray());
            const QList<core::Weapon *> ws(world->getWeapons());

            REQUIRE(ws.size() == jws.size());
            for (int i = 0; i < ws.size(); i++)
            {
                REQUIRE(jws[i].isObject() == true);
                const QJsonObject jw(jws[i].toObject());
                const core::Weapon *w = ws[i];
                REQUIRE(jw["objectName"].toString() == w->objectName());
                REQUIRE(jw["displayName"].toString() == w->getDisplayName());
                REQUIRE(jw["range"].toInt() == w->getRange());
                REQUIRE(jw["damages"].isObject() == true);
                objectEqualsMap(jw["damages"].toObject(), w->getDamages());
            }
        }

        REQUIRE(jobj["terrainTypes"].isArray() == true);
        SECTION("serializing TerrainTypes")
        {
            const QJsonArray jtts(jobj["terrainTypes"].toArray());
            const QList<core::TerrainType *> tts(world->getTerrainTypes());

            REQUIRE(tts.size() == jtts.size());
            for (int i = 0; i < tts.size(); i++)
            {
                REQUIRE(jtts[i].isObject() == true);
                const QJsonObject jtt(jtts[i].toObject());
                const core::TerrainType *tt = tts[i];
                REQUIRE(jtt["objectName"].toString() == tt->objectName());
                REQUIRE(jtt["displayName"].toString() == tt->getDisplayName());
            }
        }

        REQUIRE(jobj["unitClasses"].isArray() == true);
        SECTION("serializing UnitClasses")
        {
            const QJsonArray jucs(jobj["unitClasses"].toArray());
            const QList<core::UnitClass *> ucs(world->getUnitClasses());

            REQUIRE(ucs.size() == jucs.size());
            for (int i = 0; i < ucs.size(); i++)
            {
                REQUIRE(jucs[i].isObject() == true);
                const QJsonObject juc(jucs[i].toObject());
                const core::UnitClass *uc = ucs[i];
                REQUIRE(juc["objectName"].toString() == uc->objectName());
                REQUIRE(juc["displayName"].toString() == uc->getDisplayName());

                REQUIRE(juc["movementCosts"].isObject() == true);
                objectEqualsMap(juc["movementCosts"].toObject(), uc->getMovementCosts());

                REQUIRE(juc["attacks"].isObject() == true);
                objectEqualsMap(juc["attacks"].toObject(), uc->getAttacks());

                REQUIRE(juc["defenses"].isObject() == true);
                objectEqualsMap(juc["defenses"].toObject(), uc->getDefenses());
            }
        }

        REQUIRE(jobj["unitLevels"].isArray() == true);
        SECTION("serializing UnitLevels")
        {
            const QJsonArray juls(jobj["unitLevels"].toArray());
            const QList<core::UnitLevel *> uls(world->getUnitLevels());

            REQUIRE(uls.size() == juls.size());
            for (int i = 0; i < uls.size(); i++)
            {
                REQUIRE(juls[i].isObject() == true);
                const QJsonObject jul(juls[i].toObject());
                const core::UnitLevel *ul = uls[i];
                REQUIRE(jul["objectName"].toString() == ul->objectName());
                REQUIRE(jul["displayName"].toString() == ul->getDisplayName());
                REQUIRE(jul["experiencePoints"].toInt() == ul->getExperiencePoints());
                REQUIRE(jul["index"].toInt() == ul->getIndex());
            }
        }

        REQUIRE(jobj["unitTypes"].isArray() == true);
        SECTION("serializing UnitTypes")
        {
            const QJsonArray juts(jobj["unitTypes"].toArray());
            const QList<core::UnitType *> uts(world->getUnitTypes());

            REQUIRE(uts.size() == juts.size());
            for (int i = 0; i < uts.size(); i++)
            {
                REQUIRE(juts[i].isObject() == true);
                const QJsonObject jut(juts[i].toObject());
                const core::UnitType *ut = uts[i];
                REQUIRE(jut["objectName"].toString() == ut->objectName());
                REQUIRE(jut["displayName"].toString() == ut->getDisplayName());
                REQUIRE(jut["class"].toString() == ut->getClass()->objectName());
                REQUIRE(jut["level"].toString() == ut->getLevel()->objectName());
                REQUIRE(jut["hitPoints"].toInt() == ut->getHitPoints());
                REQUIRE(jut["recruitmentCost"].toInt() == ut->getRecruitmentCost());
                REQUIRE(jut["upkeepCost"].toInt() == ut->getUpkeepCost());
                REQUIRE(jut["armor"].toString() == ut->getArmor()->objectName());
                REQUIRE(jut["weapons"].isArray() == true);
                arrayEqualsList(jut["weapons"].toArray(), ut->getWeapons());
                REQUIRE(jut["upgrades"].isArray() == true);
                arrayEqualsList(jut["upgrades"].toArray(), ut->getUpgrades());
            }
        }

        REQUIRE(jobj["settlementTypes"].isArray() == true);
        SECTION("serializing SettlementTypes")
        {
            const QJsonArray jsts(jobj["settlementTypes"].toArray());
            const QList<core::SettlementType *> sts(world->getSettlementTypes());

            REQUIRE(sts.size() == jsts.size());
            for (int i = 0; i < sts.size(); i++)
            {
                REQUIRE(jsts[i].isObject() == true);
                const QJsonObject jst(jsts[i].toObject());
                const core::SettlementType *st = sts[i];
                REQUIRE(jst["objectName"].toString() == st->objectName());
                REQUIRE(jst["displayName"].toString() == st->getDisplayName());
                REQUIRE(jst["goldPerTurn"].toInt() == st->getGoldPerTurn());
                REQUIRE(jst["recruits"].isArray() == true);
                arrayEqualsList(jst["recruits"].toArray(), st->getRecruits());
            }
        }

        REQUIRE(jobj["factions"].isArray() == true);
        SECTION("serializing Factions")
        {
            const QJsonArray jfs(jobj["factions"].toArray());
            const QList<core::Faction *> fs(world->getFactions());

            REQUIRE(fs.size() == jfs.size());
            for (int i = 0; i < fs.size(); i++)
            {
                REQUIRE(jfs[i].isObject() == true);
                const QJsonObject jf(jfs[i].toObject());
                const core::Faction *f = fs[i];
                REQUIRE(jf["objectName"].toString() == f->objectName());
                REQUIRE(jf["displayName"].toString() == f->getDisplayName());
                REQUIRE(jf["unitTypes"].isArray() == true);
                arrayEqualsList(jf["unitTypes"].toArray(), f->getUnitTypes());

                REQUIRE(jf["recruits"].isObject() == true);
                const QJsonObject jrs(jf["recruits"].toObject());
                const QMap<core::SettlementType *, QList<core::UnitType *>> rs =
                    f->getRecruits();

                REQUIRE(rs.size() == jrs.size());
                for (core::SettlementType *st : rs.keys())
                {
                    REQUIRE(jrs[st->objectName()].isArray() == true);
                    arrayEqualsList(jrs[st->objectName()].toArray(), rs[st]);
                }
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
