#include "core/UnitType.h"
#include "test/catch.hpp"
#include "utils/Exception.h"

using namespace warmonger;

TEST_CASE("Hierarchy and inheritance", "[UnitType]")
{
    SECTION("root's getters throw")
    {
        core::UnitType root;
        core::TerrainType tt;

        REQUIRE_THROWS_AS(root.getDisplayName(), utils::ValueError);
        REQUIRE_THROWS_AS(root.getHitPoints(), utils::ValueError);
        REQUIRE_THROWS_AS(root.getExperiencePoints(), utils::ValueError);
        REQUIRE_THROWS_AS(root.getMovementPoints(), utils::ValueError);
        REQUIRE_THROWS_AS(root.getRecruitmentCost(), utils::ValueError);
        REQUIRE_THROWS_AS(root.getUpkeepCost(), utils::ValueError);
        REQUIRE_THROWS_AS(root.getMovementCosts(), utils::ValueError);
        REQUIRE_THROWS_AS(root.getMovementCost(&tt), utils::ValueError);
        REQUIRE_THROWS_AS(root.getUpgrades(), utils::ValueError);
    }

    SECTION("getters return own values when they are set")
    {
        core::UnitType root;
        core::TerrainType tt;
        core::UnitType upgrade;

        root.setDisplayName("displayName");
        root.setHitPoints(100);
        root.setExperiencePoints(300);
        root.setMovementPoints(30);
        root.setRecruitmentCost(400);
        root.setUpkeepCost(40);
        root.setMovementCosts({{&tt, 2}});
        root.setUpgrades({&upgrade});

        REQUIRE(root.getDisplayName() == "displayName");
        REQUIRE(root.getHitPoints() == 100);
        REQUIRE(root.getExperiencePoints() == 300);
        REQUIRE(root.getMovementPoints() == 30);
        REQUIRE(root.getRecruitmentCost() == 400);
        REQUIRE(root.getUpkeepCost() == 40);

        REQUIRE(root.getMovementCosts().size() == 1);
        REQUIRE(root.getMovementCost(&tt) == 2);

        REQUIRE(root.getUpgrades().size() == 1);
        REQUIRE(root.getUpgrades().front() == &upgrade);
    }

    SECTION("getters return parent's values when values are not set but has parent")
    {
        core::UnitType parent;
        core::TerrainType tt;
        core::UnitType upgrade;

        core::UnitType child;

        child.setHierarchyParent(&parent);

        parent.setDisplayName("displayName");
        parent.setHitPoints(100);
        parent.setExperiencePoints(300);
        parent.setMovementPoints(30);
        parent.setRecruitmentCost(400);
        parent.setUpkeepCost(40);
        parent.setMovementCosts({{&tt, 2}});
        parent.setUpgrades({&upgrade});

        REQUIRE(child.getDisplayName() == "displayName");
        REQUIRE(child.getHitPoints() == 100);
        REQUIRE(child.getExperiencePoints() == 300);
        REQUIRE(child.getMovementPoints() == 30);
        REQUIRE(child.getRecruitmentCost() == 400);
        REQUIRE(child.getUpkeepCost() == 40);

        REQUIRE(child.getMovementCosts().size() == 1);
        REQUIRE(child.getMovementCost(&tt) == 2);

        REQUIRE(child.getUpgrades().size() == 1);
        REQUIRE(child.getUpgrades().front() == &upgrade);
    }
}
