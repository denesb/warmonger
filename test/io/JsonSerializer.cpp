#include <memory>
#include <tuple>

#include <QJsonDocument>

#include "io/JsonSerializer.h"
#include "test/catch.hpp"
#include "test/Util.h"

using namespace warmonger;

TEST_CASE("Armor can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    core::Armor *a = world->getArmors()[0];

    SECTION("serializing Armor")
    {
        io::JsonSerializer serializer;
        const QByteArray json(serializer.serializeArmor(a));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == a->objectName());
        REQUIRE(jobj["displayName"].toString() == a->getDisplayName());
        REQUIRE(jobj["defenses"].isObject() == true);
        objectEqualsMap(jobj["defenses"].toObject(), a->getDefenses());
    }
}

TEST_CASE("Army can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::CampaignMap *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jobj{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const core::Army *a = map->getArmies()[0];

    SECTION("serializing Army")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeArmy(a));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == a->objectName());
        REQUIRE(jobj["displayName"].toString() == a->getDisplayName());
        REQUIRE(jobj["mapNode"].toString() == a->getMapNode()->objectName());
        REQUIRE(jobj["owner"].toString() == a->getOwner()->objectName());
        REQUIRE(jobj["units"].isArray());
        arrayEqualsList(jobj["units"].toArray(), a->getUnits());
    }
}

TEST_CASE("CampaignMap can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::CampaignMap *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> m{maps.first};
    const QJsonObject jobj{maps.second};
    const std::unique_ptr<core::World> world{m->getWorld()};

    SECTION("serializing CampaignMap")
    {
        io::JsonSerializer serializer;
        const QByteArray json(serializer.serializeCampaignMap(m.get()));
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
            const std::vector<core::MapNode *> mns(m->getMapNodes());

            REQUIRE(mns.size() == jmns.size());
            for (size_t i = 0; i < mns.size(); i++)
            {
                REQUIRE(jmns[i].isObject() == true);
                const QJsonObject jmn(jmns[i].toObject());
                const core::MapNode *mn = mns[i];
                REQUIRE(jmn["objectName"].toString() == mn->objectName());
                REQUIRE(jmn["displayName"].toString() == mn->getDisplayName());
                REQUIRE(jmn["terrainType"].toString() == mn->getTerrainType()->objectName());
                REQUIRE(jmn["neighbours"].isObject());

                const QJsonObject jneighbours = jmn["neighbours"].toObject();
                const std::map<utils::Direction, core::MapNode *> neighbours = mn->getNeighbours();

                REQUIRE(jneighbours.size() == neighbours.size());

                for (auto it = jneighbours.constBegin(); it != jneighbours.constEnd(); it++)
                {
                    REQUIRE_NOTHROW(utils::str2direction(it.key()));
                    REQUIRE(it.value().isString());

                    const utils::Direction dir{utils::str2direction(it.key())};
                    const QString neighbourName{it.value().toString()};

                    REQUIRE(neighbourName.isEmpty() == (neighbours.at(dir) == nullptr));

                    if(!neighbourName.isEmpty())
                    {
                        REQUIRE(neighbourName == neighbours.at(dir)->objectName());
                    }
                }
            }
        }

        REQUIRE(jobj["factions"].isArray() == true);
        SECTION("serializing Factions")
        {
            const QJsonArray jps(jobj["factions"].toArray());
            const std::vector<core::Faction *> ps(m->getFactions());

            REQUIRE(ps.size() == jps.size());
            for (size_t i = 0; i < ps.size(); i++)
            {
                REQUIRE(jps[i].isObject() == true);
                const QJsonObject jp(jps[i].toObject());
                const core::Faction *p = ps[i];
                REQUIRE(jp["objectName"].toString() == p->objectName());
                REQUIRE(jp["displayName"].toString() == p->getDisplayName());
                REQUIRE(jp["color"].toString() == p->getColor().name());
                REQUIRE(jp["goldBalance"].toInt() == p->getGoldBalance());
                REQUIRE(jp["civilization"].toString() == p->getCivilization()->objectName());
            }
        }

        REQUIRE(jobj["settlements"].isArray() == true);
        SECTION("serializing Settlements")
        {
            const QJsonArray jss(jobj["settlements"].toArray());
            const std::vector<core::Settlement *> ss(m->getSettlements());

            REQUIRE(ss.size() == jss.size());
            for (size_t i = 0; i < ss.size(); i++)
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
            const std::vector<core::Unit *> us(m->getUnits());

            REQUIRE(us.size() == jus.size());
            for (size_t i = 0; i < us.size(); i++)
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

        REQUIRE(jobj["armies"].isArray() == true);
        SECTION("serializing Armies")
        {
            const QJsonArray jas(jobj["armies"].toArray());
            const std::vector<core::Army *> as(m->getArmies());

            REQUIRE(as.size() == jas.size());
            for (size_t i = 0; i < as.size(); i++)
            {
                REQUIRE(jas[i].isObject() == true);
                const QJsonObject ja(jas[i].toObject());
                const core::Army *a = as[i];
                REQUIRE(ja["objectName"].toString() == a->objectName());
                REQUIRE(ja["displayName"].toString() == a->getDisplayName());
                REQUIRE(ja["mapNode"].toString() == a->getMapNode()->objectName());
                REQUIRE(ja["owner"].toString() == a->getOwner()->objectName());
                REQUIRE(ja["units"].isArray());
                arrayEqualsList(ja["units"].toArray(), a->getUnits());
            }
        }
    }
}

TEST_CASE("DamageType can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
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

TEST_CASE("Civilization can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    core::Civilization *f = world->getCivilizations()[0];

    SECTION("serializing Civilization")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeCivilization(f));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == f->objectName());
        REQUIRE(jobj["displayName"].toString() == f->getDisplayName());
        REQUIRE(jobj["unitTypes"].isArray() == true);
        arrayEqualsList(jobj["unitTypes"].toArray(), f->getUnitTypes());

        REQUIRE(jobj["recruits"].isObject() == true);

        const QJsonObject jrecruits = jobj["recruits"].toObject();
        const std::map<core::SettlementType *, std::vector<core::UnitType *>> recruits =
            f->getRecruits();

        REQUIRE(jrecruits.size() == recruits.size());

        for (const auto& e : recruits)
        {
            REQUIRE(jrecruits[e.first->objectName()].isArray() == true);
            arrayEqualsList(jrecruits[e.first->objectName()].toArray(), e.second);
        }
    }
}

TEST_CASE("MapNode can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::CampaignMap *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
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

TEST_CASE("Faction can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::CampaignMap *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
    const QJsonObject jobj{maps.second};
    const std::unique_ptr<core::World> world{map->getWorld()};

    const core::Faction *f = map->getFactions()[0];

    SECTION("serializing Faction")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeFaction(f));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == f->objectName());
        REQUIRE(jobj["displayName"].toString() == f->getDisplayName());
        REQUIRE(jobj["color"].toString() == f->getColor().name());
        REQUIRE(jobj["goldBalance"].toInt() == f->getGoldBalance());
        REQUIRE(jobj["civilization"].toString() == f->getCivilization()->objectName());
    }
}

TEST_CASE("Settlement can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::CampaignMap *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
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
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
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
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
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
    const std::pair<core::CampaignMap *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::CampaignMap> map{maps.first};
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
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
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
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
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
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
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
        REQUIRE(jobj["weapons"].isArray());
        REQUIRE(jobj["attackSkills"].isObject());
        REQUIRE(jobj["defenseSkills"].isObject());
        REQUIRE(jobj["upgrades"].isArray());

        const QJsonArray weapons(jobj["weapons"].toArray());
        arrayEqualsList(weapons, ut->getWeapons());

        const QJsonObject attackSkills(jobj["attackSkills"].toObject());
        objectEqualsMap(attackSkills, ut->getAttackSkills());

        const QJsonObject defenseSkills(jobj["defenseSkills"].toObject());
        objectEqualsMap(defenseSkills, ut->getDefenseSkills());

        const QJsonArray upgrades(jobj["upgrades"].toArray());
        arrayEqualsList(upgrades, ut->getUpgrades());
    }
}

TEST_CASE("WeaponClass can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    core::WeaponClass *wc = world->getWeaponClasses()[0];

    SECTION("serializing WeaponClass")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeWeaponClass(wc));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == wc->objectName());
        REQUIRE(jobj["displayName"].toString() == wc->getDisplayName());
    }
}

TEST_CASE("WeaponType can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    core::WeaponType *wt = world->getWeaponTypes()[0];

    SECTION("serializing WeaponType")
    {
        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeWeaponType(wt));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == wt->objectName());
        REQUIRE(jobj["displayName"].toString() == wt->getDisplayName());
        REQUIRE(jobj["class"].toString() == wt->getClass()->objectName());
    }
}

TEST_CASE("Weapon can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
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
        REQUIRE(jobj["type"].toString() == w->getType()->objectName());

        const QJsonObject damages(jobj["damages"].toObject());
        objectEqualsMap(damages, w->getDamages());
    }
}

TEST_CASE("World can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
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
            const std::vector<core::DamageType *> dts(world->getDamageTypes());

            REQUIRE(dts.size() == jdts.size());
            for (size_t i = 0; i < dts.size(); i++)
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
            const std::vector<core::Armor *> as(world->getArmors());

            REQUIRE(as.size() == jas.size());
            for (size_t i = 0; i < as.size(); i++)
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

        REQUIRE(jobj["weaponClasses"].isArray() == true);
        SECTION("serializing WeaponClasses")
        {
            const QJsonArray jwcs(jobj["weaponClasses"].toArray());
            const std::vector<core::WeaponClass *> wcs(world->getWeaponClasses());

            REQUIRE(wcs.size() == jwcs.size());
            for (size_t i = 0; i < wcs.size(); i++)
            {
                REQUIRE(jwcs[i].isObject() == true);
                const QJsonObject jwc(jwcs[i].toObject());
                const core::WeaponClass *wc = wcs[i];
                REQUIRE(jwc["objectName"].toString() == wc->objectName());
                REQUIRE(jwc["displayName"].toString() == wc->getDisplayName());
            }
        }

        REQUIRE(jobj["weaponTypes"].isArray() == true);
        SECTION("serializing WeaponTypes")
        {
            const QJsonArray jwts(jobj["weaponTypes"].toArray());
            const std::vector<core::WeaponType *> wts(world->getWeaponTypes());

            REQUIRE(wts.size() == jwts.size());
            for (size_t i = 0; i < wts.size(); i++)
            {
                REQUIRE(jwts[i].isObject() == true);
                const QJsonObject jwt(jwts[i].toObject());
                const core::WeaponType *wt = wts[i];
                REQUIRE(jwt["objectName"].toString() == wt->objectName());
                REQUIRE(jwt["displayName"].toString() == wt->getDisplayName());
                REQUIRE(jwt["class"].toString() == wt->getClass()->objectName());
            }
        }

        REQUIRE(jobj["weapons"].isArray() == true);
        SECTION("serializing Weapons")
        {
            const QJsonArray jws(jobj["weapons"].toArray());
            const std::vector<core::Weapon *> ws(world->getWeapons());

            REQUIRE(ws.size() == jws.size());
            for (size_t i = 0; i < ws.size(); i++)
            {
                REQUIRE(jws[i].isObject() == true);
                const QJsonObject jw(jws[i].toObject());
                const core::Weapon *w = ws[i];
                REQUIRE(jw["objectName"].toString() == w->objectName());
                REQUIRE(jw["displayName"].toString() == w->getDisplayName());
                REQUIRE(jw["range"].toInt() == w->getRange());
                REQUIRE(jw["damages"].isObject() == true);
                objectEqualsMap(jw["damages"].toObject(), w->getDamages());
                REQUIRE(jw["type"].toString() == w->getType()->objectName());
            }
        }

        REQUIRE(jobj["terrainTypes"].isArray() == true);
        SECTION("serializing TerrainTypes")
        {
            const QJsonArray jtts(jobj["terrainTypes"].toArray());
            const std::vector<core::TerrainType *> tts(world->getTerrainTypes());

            REQUIRE(tts.size() == jtts.size());
            for (size_t i = 0; i < tts.size(); i++)
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
            const std::vector<core::UnitClass *> ucs(world->getUnitClasses());

            REQUIRE(ucs.size() == jucs.size());
            for (size_t i = 0; i < ucs.size(); i++)
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
            const std::vector<core::UnitLevel *> uls(world->getUnitLevels());

            REQUIRE(uls.size() == juls.size());
            for (size_t i = 0; i < uls.size(); i++)
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
            const std::vector<core::UnitType *> uts(world->getUnitTypes());

            REQUIRE(uts.size() == juts.size());
            for (size_t i = 0; i < uts.size(); i++)
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
                REQUIRE(jut["weapons"].isArray());
                arrayEqualsList(jut["weapons"].toArray(), ut->getWeapons());
                REQUIRE(jut["attackSkills"].isObject());
                objectEqualsMap(jut["attackSkills"].toObject(), ut->getAttackSkills());
                REQUIRE(jut["defenseSkills"].isObject());
                objectEqualsMap(jut["defenseSkills"].toObject(), ut->getDefenseSkills());
                REQUIRE(jut["upgrades"].isArray());
                arrayEqualsList(jut["upgrades"].toArray(), ut->getUpgrades());
            }
        }

        REQUIRE(jobj["settlementTypes"].isArray() == true);
        SECTION("serializing SettlementTypes")
        {
            const QJsonArray jsts(jobj["settlementTypes"].toArray());
            const std::vector<core::SettlementType *> sts(world->getSettlementTypes());

            REQUIRE(sts.size() == jsts.size());
            for (size_t i = 0; i < sts.size(); i++)
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

        REQUIRE(jobj["civilizations"].isArray() == true);
        SECTION("serializing Civilizations")
        {
            const QJsonArray jcs(jobj["civilizations"].toArray());
            const std::vector<core::Civilization *> cs(world->getCivilizations());

            REQUIRE(cs.size() == jcs.size());
            for (size_t i = 0; i < cs.size(); i++)
            {
                REQUIRE(jcs[i].isObject() == true);
                const QJsonObject jc(jcs[i].toObject());
                const core::Civilization *c = cs[i];
                REQUIRE(jc["objectName"].toString() == c->objectName());
                REQUIRE(jc["displayName"].toString() == c->getDisplayName());
                REQUIRE(jc["unitTypes"].isArray());
                arrayEqualsList(jc["unitTypes"].toArray(), c->getUnitTypes());

                REQUIRE(jc["recruits"].isObject());
                const QJsonObject jrs(jc["recruits"].toObject());
                const std::map<core::SettlementType *, std::vector<core::UnitType *>> rs = c->getRecruits();

                REQUIRE(rs.size() == jrs.size());
                for (const auto& e : rs)
                {
                    REQUIRE(jrs[e.first->objectName()].isArray() == true);
                    arrayEqualsList(jrs[e.first->objectName()].toArray(), e.second);
                }
            }
        }
    }
}
