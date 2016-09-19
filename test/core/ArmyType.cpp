#include "core/ArmyType.h"
#include "test/catch.hpp"
#include "utils/Exception.h"

using namespace warmonger;

TEST_CASE("ArmyType hierarchy and inheritance", "[ArmyType]")
{
    SECTION("root's getters throw")
    {
        core::ArmyType root;

        REQUIRE_THROWS_AS(root.getDisplayName(), utils::ValueError);
    }

    SECTION("getters return own values when they are set")
    {
        core::ArmyType root;

        root.setDisplayName("displayName");

        REQUIRE(root.getDisplayName() == "displayName");
    }

    SECTION("getters return parent's values when values are not set but has parent")
    {
        core::ArmyType parent;
        core::ArmyType child;

        child.setHierarchyParent(&parent);

        parent.setDisplayName("displayName");

        REQUIRE(child.getDisplayName() == "displayName");
    }
}
