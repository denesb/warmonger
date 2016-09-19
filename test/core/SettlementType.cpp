#include "core/SettlementType.h"
#include "test/catch.hpp"
#include "utils/Exception.h"

using namespace warmonger;

TEST_CASE("SettlementType hierarchy and inheritance", "[SettlementType]")
{
    SECTION("root's getters throw")
    {
        core::SettlementType root;

        REQUIRE_THROWS_AS(root.getDisplayName(), utils::ValueError);
    }

    SECTION("getters return own values when they are set")
    {
        core::SettlementType root;

        root.setDisplayName("displayName");

        REQUIRE(root.getDisplayName() == "displayName");
    }

    SECTION("getters return parent's values when values are not set but has parent")
    {
        core::SettlementType parent;
        core::SettlementType child;

        child.setHierarchyParent(&parent);

        parent.setDisplayName("displayName");

        REQUIRE(child.getDisplayName() == "displayName");
    }
}
