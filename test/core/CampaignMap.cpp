#include "core/CampaignMap.h"
#include "test/Util.h"
#include "test/catch.hpp"
#include "utils/ToString.h"

using namespace warmonger;

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
}
