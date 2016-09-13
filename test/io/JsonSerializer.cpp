#include <memory>
#include <tuple>

#include <QJsonDocument>

#include "io/JsonSerializer.h"
#include "test/catch.hpp"
#include "test/Util.h"

using namespace warmonger;

TEST_CASE("ArmyType can be serialized to JSON", "[JsonSerializer]")
{
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    core::ArmyType *at = world->getArmyTypes()[0];

    SECTION("serializing ArmyType")
    {
        io::JsonSerializer serializer;
        const QByteArray json(serializer.serializeArmyType(at));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["objectName"].toString() == at->objectName());
        REQUIRE(jobj["displayName"].toString() == at->getDisplayName());
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
        REQUIRE(jobj["type"].toString() == a->getType()->objectName());
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
                const core::MapNodeNeighbours& neighbours = mn->getNeighbours();

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
                REQUIRE(ja["type"].toString() == a->getType()->objectName());
                REQUIRE(ja["mapNode"].toString() == a->getMapNode()->objectName());
                REQUIRE(ja["owner"].toString() == a->getOwner()->objectName());
                REQUIRE(ja["units"].isArray());
                arrayEqualsList(ja["units"].toArray(), a->getUnits());
            }
        }
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
        REQUIRE(jobj["hitPoints"].toInt() == ut->getHitPoints());
        REQUIRE(jobj["experiencePoints"].toInt() == ut->getExperiencePoints());
        REQUIRE(jobj["movementPoints"].toInt() == ut->getMovementPoints());
        REQUIRE(jobj["recruitmentCost"].toInt() == ut->getRecruitmentCost());
        REQUIRE(jobj["upkeepCost"].toInt() == ut->getUpkeepCost());
        REQUIRE(jobj["upgrades"].isArray());
        REQUIRE(jobj["movementCosts"].isObject() == true);

        const QJsonArray upgrades(jobj["upgrades"].toArray());
        arrayEqualsList(upgrades, ut->getUpgrades());

        const QJsonObject movementCosts(jobj["movementCosts"].toObject());
        objectEqualsMap(movementCosts, ut->getMovementCosts());
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

        REQUIRE(jobj["armyTypes"].isArray() == true);
        SECTION("serializing ArmyTypes")
        {
            const QJsonArray jats(jobj["armyTypes"].toArray());
            const std::vector<core::ArmyType *> ats(world->getArmyTypes());

            REQUIRE(ats.size() == jats.size());
            for (size_t i = 0; i < ats.size(); i++)
            {
                REQUIRE(jats[i].isObject() == true);
                const QJsonObject jat(jats[i].toObject());
                const core::ArmyType *at = ats[i];
                REQUIRE(jat["objectName"].toString() == at->objectName());
                REQUIRE(jat["displayName"].toString() == at->getDisplayName());
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
                REQUIRE(jut["hitPoints"].toInt() == ut->getHitPoints());
                REQUIRE(jut["experiencePoints"].toInt() == ut->getExperiencePoints());
                REQUIRE(jut["movementPoints"].toInt() == ut->getMovementPoints());
                REQUIRE(jut["recruitmentCost"].toInt() == ut->getRecruitmentCost());
                REQUIRE(jut["upkeepCost"].toInt() == ut->getUpkeepCost());
                REQUIRE(jut["upgrades"].isArray());
                arrayEqualsList(jut["upgrades"].toArray(), ut->getUpgrades());
                REQUIRE(jut["movementCosts"].isObject() == true);
                objectEqualsMap(jut["movementCosts"].toObject(), ut->getMovementCosts());
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
