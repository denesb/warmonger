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
        REQUIRE(m->getMapNodes().size() == jobj["mapNodes"].toArray().size());

        REQUIRE(jobj["factions"].isArray() == true);
        REQUIRE(m->getFactions().size() == jobj["factions"].toArray().size());

        REQUIRE(jobj["settlements"].isArray() == true);
        REQUIRE(m->getSettlements().size() == m->getSettlements().size());

        REQUIRE(jobj["units"].isArray() == true);
        REQUIRE(m->getUnits().size() == jobj["units"].toArray().size());

        REQUIRE(jobj["armies"].isArray() == true);
        REQUIRE(m->getArmies().size() == jobj["armies"].toArray().size());
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
        REQUIRE(jobj["neighbours"].isObject());

        const QJsonObject jneighbours = jobj["neighbours"].toObject();
        const core::MapNodeNeighbours& neighbours = mn->getNeighbours();

        for (auto it = jneighbours.constBegin(); it != jneighbours.constEnd(); it++)
        {
            REQUIRE_NOTHROW(core::str2direction(it.key()));
            REQUIRE(it.value().isString());

            const core::Direction dir{core::str2direction(it.key())};
            const QString neighbourName{it.value().toString()};

            REQUIRE(neighbourName.isEmpty() == (neighbours.at(dir) == nullptr));

            if(!neighbourName.isEmpty())
            {
                REQUIRE(neighbourName == neighbours.at(dir)->objectName());
            }
        }
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

        REQUIRE(!jobj.contains("hierarchyParent"));

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

    SECTION("serializing UnitType with all properties inherited")
    {
        core::UnitType childUt;
        childUt.setHierarchyParent(ut);

        childUt.setObjectName("childUnitType0");

        io::JsonSerializer serializer;
        QByteArray json(serializer.serializeUnitType(&childUt));
        const QJsonDocument jdoc(QJsonDocument::fromJson(json));
        const QJsonObject jobj(jdoc.object());

        REQUIRE(jobj["hierarchyParent"].toString() == childUt.getHierarchyParent()->objectName());
        REQUIRE(jobj["objectName"].toString() == childUt.objectName());
        REQUIRE(!jobj.contains("displayName"));
        REQUIRE(!jobj.contains("hitPoints"));
        REQUIRE(!jobj.contains("experiencePoints"));
        REQUIRE(!jobj.contains("movementPoints"));
        REQUIRE(!jobj.contains("recruitmentCost"));
        REQUIRE(!jobj.contains("upkeepCost"));
        REQUIRE(!jobj.contains("upgrades"));
        REQUIRE(!jobj.contains("movementCosts"));
        REQUIRE(!jobj.contains("upgrades"));
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
        REQUIRE(world->getArmyTypes().size() == jobj["armyTypes"].toArray().size());

        REQUIRE(jobj["terrainTypes"].isArray() == true);
        REQUIRE(world->getTerrainTypes().size() == jobj["terrainTypes"].toArray().size());

        REQUIRE(jobj["unitTypes"].isArray() == true);
        REQUIRE(world->getUnitTypes().size() == jobj["unitTypes"].toArray().size());

        REQUIRE(jobj["settlementTypes"].isArray() == true);
        REQUIRE(world->getSettlementTypes().size() == jobj["settlementTypes"].toArray().size());

        REQUIRE(jobj["civilizations"].isArray() == true);
        REQUIRE(world->getCivilizations().size() == jobj["civilizations"].toArray().size());
    }
}
