#include "core/CampaignMap.h"
#include "test/Util.h"
#include "test/catch.hpp"
#include "utils/ToString.h"

using namespace warmonger;

template <typename T>
struct MatchItemWithMember
{
    MatchItemWithMember(const T& member)
        : member(member)
    {
    }

    bool operator()(const core::CampaignMap::Content& content)
    {
        return std::get<T>(content) == this->member;
    }

    const T& member;
};

TEST_CASE("Create methods", "[CampaignMap]")
{
    core::CampaignMap map;

    SECTION("Settlement")
    {
        core::SettlementType type;

        core::Settlement* settlement = map.createSettlement(&type);

        REQUIRE(settlement->objectName() == "settlement-0");

        core::Settlement* settlement0 = new core::Settlement();
        core::Settlement* settlement2 = new core::Settlement();
        core::Settlement* settlementn = new core::Settlement();

        settlement0->setObjectName("settlement-0");
        settlement2->setObjectName("settlement-2");
        settlementn->setObjectName("settlement_3");

        DestroyWatcher w(settlement);

        map.setSettlements({settlement0, settlement2, settlementn});

        REQUIRE(w.isDestroyed());

        REQUIRE(settlement0->parent() == &map);
        REQUIRE(settlement2->parent() == &map);
        REQUIRE(settlementn->parent() == &map);

        core::Settlement* settlement3 = map.createSettlement(&type);

        REQUIRE(settlement3->objectName() == "settlement-3");
    }

    SECTION("MapNode")
    {
        core::TerrainType type;

        core::MapNode* mapNode = map.createMapNode(&type, core::MapNodeNeighbours());

        REQUIRE(mapNode->objectName() == "mapNode-0");

        core::MapNode* mapNode0 = new core::MapNode();
        core::MapNode* mapNode2 = new core::MapNode();
        core::MapNode* mapNoden = new core::MapNode();

        mapNode0->setObjectName("mapNode-0");
        mapNode2->setObjectName("mapNode-2");
        mapNoden->setObjectName("mapNode_3");

        DestroyWatcher w(mapNode);

        map.setMapNodes({mapNode0, mapNode2, mapNoden});

        REQUIRE(w.isDestroyed());

        REQUIRE(mapNode0->parent() == &map);
        REQUIRE(mapNode2->parent() == &map);
        REQUIRE(mapNoden->parent() == &map);

        core::MapNode* mapNode3 = map.createMapNode(
            &type, core::MapNodeNeighbours({{core::Direction::West, mapNode0}, {core::Direction::East, mapNode2}}));

        REQUIRE(mapNode3->objectName() == "mapNode-3");
        REQUIRE(mapNode3->getNeighbour(core::Direction::West) == mapNode0);
        REQUIRE(mapNode3->getNeighbour(core::Direction::East) == mapNode2);
        REQUIRE(mapNode0->getNeighbour(core::Direction::East) == mapNode3);
        REQUIRE(mapNode2->getNeighbour(core::Direction::West) == mapNode3);
    }

    SECTION("Unit")
    {
        core::UnitType type;
        type.setHitPoints(100);

        core::Unit* unit = map.createUnit(&type);

        REQUIRE(unit->objectName() == "unit-0");

        core::Unit* unit0 = new core::Unit();
        core::Unit* unit2 = new core::Unit();
        core::Unit* unitn = new core::Unit();

        unit0->setObjectName("unit-0");
        unit2->setObjectName("unit-2");
        unitn->setObjectName("unit_3");

        DestroyWatcher w(unit);

        map.setUnits({unit0, unit2, unitn});

        REQUIRE(w.isDestroyed());

        REQUIRE(unit0->parent() == &map);
        REQUIRE(unit2->parent() == &map);
        REQUIRE(unitn->parent() == &map);

        core::Unit* unit3 = map.createUnit(&type);

        REQUIRE(unit3->objectName() == "unit-3");
    }

    SECTION("Army")
    {
        core::ArmyType type;

        core::Army* army = map.createArmy(&type);

        REQUIRE(army->objectName() == "army-0");

        core::Army* army0 = new core::Army();
        core::Army* army2 = new core::Army();
        core::Army* armyn = new core::Army();

        army0->setObjectName("army-0");
        army2->setObjectName("army-2");
        armyn->setObjectName("army_3");

        DestroyWatcher w(army);

        map.setArmies({army0, army2, armyn});

        REQUIRE(w.isDestroyed());

        REQUIRE(army0->parent() == &map);
        REQUIRE(army2->parent() == &map);
        REQUIRE(armyn->parent() == &map);

        core::Army* army3 = map.createArmy(&type);

        REQUIRE(army3->objectName() == "army-3");
    }

    SECTION("Faction")
    {
        core::World world;

        core::Civilization* civ = new core::Civilization(&world);
        core::Banner* banner = new core::Banner(&world);

        world.setBanners({banner});
        world.setColors({QColor("black"), QColor("white"), QColor("red")});
        world.setCivilizations({civ});

        map.setWorld(&world);

        core::Faction* f0 = map.createFaction(civ);

        REQUIRE(f0->getCivilization() == civ);
        REQUIRE(f0->objectName() == "faction-0");
        REQUIRE(f0->getBanner() == banner);
        REQUIRE(f0->getPrimaryColor().isValid());
        REQUIRE(f0->getSecondaryColor().isValid());

        core::Faction* f1 = map.createFaction(civ);
        REQUIRE(f1->getCivilization() == civ);
        REQUIRE(f1->objectName() == "faction-1");
        REQUIRE(f1->getBanner() == banner);
        REQUIRE(f1->getPrimaryColor().isValid());
        REQUIRE(f1->getSecondaryColor().isValid());
        REQUIRE(!(f1->getPrimaryColor() == f0->getPrimaryColor() && (f1->getSecondaryColor() == f0->getSecondaryColor())));
    }
}

TEST_CASE("Content", "[CampaignMap]")
{
    core::World world;

    core::CampaignMap map;

    map.setWorld(&world);

    core::TerrainType* terrainType0 = new core::TerrainType(&world);
    core::SettlementType* settlementType0 = new core::SettlementType(&world);
    core::ArmyType* armyType0 = new core::ArmyType(&world);

    world.setTerrainTypes({terrainType0});
    world.setSettlementTypes({settlementType0});
    world.setArmyTypes({armyType0});

    REQUIRE(map.getMapNodes().empty());
    REQUIRE(map.getSettlements().empty());
    REQUIRE(map.getArmies().empty());
    REQUIRE(map.getContents().empty());

    SECTION("Newly created settlement")
    {
        core::MapNode* mapNode0 = map.createMapNode(terrainType0, core::MapNodeNeighbours());

        REQUIRE(map.getContents().size() == 1);
        REQUIRE(std::get<0>(map.getContents()[0]) == mapNode0);

        std::vector<core::CampaignMap::Content> previousContent = map.getContents();

        core::Settlement* settlement0 = map.createSettlement(settlementType0);

        REQUIRE(previousContent == map.getContents());

        settlement0->setMapNode(mapNode0);

        REQUIRE(std::get<1>(map.getContents()[0]) == settlement0);

        settlement0->setMapNode(nullptr);

        REQUIRE(std::get<1>(map.getContents()[0]) == nullptr);
    }

    SECTION("Newly created army")
    {
        core::MapNode* mapNode0 = map.createMapNode(terrainType0, core::MapNodeNeighbours());

        REQUIRE(map.getContents().size() == 1);
        REQUIRE(std::get<0>(map.getContents()[0]) == mapNode0);

        std::vector<core::CampaignMap::Content> previousContent = map.getContents();

        core::Army* army0 = map.createArmy(armyType0);

        REQUIRE(previousContent == map.getContents());

        army0->setMapNode(mapNode0);

        REQUIRE(std::get<2>(map.getContents()[0]) == army0);

        army0->setMapNode(nullptr);

        REQUIRE(std::get<2>(map.getContents()[0]) == nullptr);
    }

    SECTION("Content set with setters")
    {
        core::MapNode* mapNode0 = new core::MapNode();
        core::MapNode* mapNode1 = new core::MapNode();

        map.setMapNodes({mapNode0, mapNode1});

        const std::vector<core::CampaignMap::Content>& contents = map.getContents();

        REQUIRE(contents.size() == 2);
        REQUIRE(std::find_if(contents.begin(), contents.end(), MatchItemWithMember<core::MapNode*>(mapNode0)) !=
            contents.end());
        REQUIRE(std::find_if(contents.begin(), contents.end(), MatchItemWithMember<core::MapNode*>(mapNode1)) !=
            contents.end());

        core::Settlement* settlement0 = new core::Settlement();
        core::Settlement* settlement1 = new core::Settlement();

        settlement1->setMapNode(mapNode0);

        map.setSettlements({settlement0, settlement1});

        core::Army* army0 = new core::Army();
        core::Army* army1 = new core::Army();

        army1->setMapNode(mapNode1);

        map.setArmies({army0, army1});

        SECTION("Settlement changes are tracked")
        {
            REQUIRE(
                std::find_if(contents.begin(), contents.end(), MatchItemWithMember<core::Settlement*>(settlement0)) ==
                contents.end());

            const auto it1 =
                std::find_if(contents.begin(), contents.end(), MatchItemWithMember<core::Settlement*>(settlement1));

            REQUIRE(it1 != contents.end());
            REQUIRE(std::get<1>(*it1) == settlement1);
            REQUIRE(std::get<0>(*it1) == settlement1->getMapNode());

            settlement0->setMapNode(mapNode1);

            const auto it0 =
                std::find_if(contents.begin(), contents.end(), MatchItemWithMember<core::Settlement*>(settlement0));

            REQUIRE(it0 != contents.end());
            REQUIRE(std::get<1>(*it0) == settlement0);
            REQUIRE(std::get<0>(*it0) == settlement0->getMapNode());
        }

        SECTION("Army changes are tracked")
        {
            REQUIRE(std::find_if(contents.begin(), contents.end(), MatchItemWithMember<core::Army*>(army0)) ==
                contents.end());

            const auto it1 = std::find_if(contents.begin(), contents.end(), MatchItemWithMember<core::Army*>(army1));

            REQUIRE(it1 != contents.end());
            REQUIRE(std::get<2>(*it1) == army1);
            REQUIRE(std::get<0>(*it1) == army1->getMapNode());

            army0->setMapNode(mapNode0);

            const auto it0 = std::find_if(contents.begin(), contents.end(), MatchItemWithMember<core::Army*>(army0));

            REQUIRE(it0 != contents.end());
            REQUIRE(std::get<2>(*it0) == army0);
            REQUIRE(std::get<0>(*it0) == army0->getMapNode());
        }

        SECTION("Removed settlements are not tracked anymore")
        {
            std::unique_ptr<core::Settlement> removedSettlement = map.removeSettlement(settlement0);

            REQUIRE(removedSettlement);

            auto it0 = std::find_if(
                contents.begin(), contents.end(), MatchItemWithMember<core::Settlement*>(removedSettlement.get()));

            REQUIRE(it0 == contents.end());

            removedSettlement->setMapNode(mapNode1);

            it0 = std::find_if(
                contents.begin(), contents.end(), MatchItemWithMember<core::Settlement*>(removedSettlement.get()));

            REQUIRE(it0 == contents.end());

            core::Settlement* settlement3 = new core::Settlement();

            settlement1->setMapNode(mapNode1);
            map.setSettlements({settlement3});

            auto it1 =
                std::find_if(contents.begin(), contents.end(), MatchItemWithMember<core::Settlement*>(settlement1));

            REQUIRE(it1 == contents.end());
        }

        SECTION("Removed armies are not tracked anymore")
        {
            std::unique_ptr<core::Army> removedArmy = map.removeArmy(army0);

            REQUIRE(removedArmy);

            auto it0 =
                std::find_if(contents.begin(), contents.end(), MatchItemWithMember<core::Army*>(removedArmy.get()));

            REQUIRE(it0 == contents.end());

            removedArmy->setMapNode(mapNode1);

            it0 = std::find_if(contents.begin(), contents.end(), MatchItemWithMember<core::Army*>(removedArmy.get()));

            REQUIRE(it0 == contents.end());

            core::Army* army3 = new core::Army();

            army1->setMapNode(mapNode1);
            map.setArmies({army3});

            auto it1 = std::find_if(contents.begin(), contents.end(), MatchItemWithMember<core::Army*>(army1));

            REQUIRE(it1 == contents.end());
        }

        SECTION("Removed mapNode is removed from content")
        {
            std::unique_ptr<core::MapNode> removedMapNode = map.removeMapNode(mapNode1);

            REQUIRE(removedMapNode);

            REQUIRE(std::find_if(contents.begin(),
                        contents.end(),
                        MatchItemWithMember<core::MapNode*>(removedMapNode.get())) == contents.end());
        }
    }
}
