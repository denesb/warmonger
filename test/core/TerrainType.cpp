#include "core/TerrainType.h"
#include "test/catch.hpp"
#include "utils/Exception.h"

using namespace warmonger;

TEST_CASE("TerrainType hierarchy and inheritance", "[TerrainType]")
{
    SECTION("root's getters throw")
    {
        core::TerrainType root;

        REQUIRE_THROWS_AS(root.getDisplayName(), utils::ValueError);
    }

    SECTION("getters return own values when they are set")
    {
        core::TerrainType root;

        root.setDisplayName("displayName");

        REQUIRE(root.getDisplayName() == "displayName");
    }

    SECTION("getters return parent's values when values are not set but has parent")
    {
        core::TerrainType parent;
        core::TerrainType child;

        child.setHierarchyParent(&parent);

        parent.setDisplayName("displayName");

        REQUIRE(child.getDisplayName() == "displayName");
    }
}
