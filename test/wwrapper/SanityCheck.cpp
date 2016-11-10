#include <QString>

#include "test/catch.hpp"

#include "wwrapper/SanityCheck.h"

using namespace warmonger;

TEST_CASE("isWorldSane", "[SanityCheck]")
{
    SECTION("Inexistent")
    {
        REQUIRE(!wwrapper::isWorldSane("./world-packages/inexistent.wwd"));
    }

    SECTION("Invalid JSON")
    {
        REQUIRE(!wwrapper::isWorldSane("./world-packages/invalidJson.wwd"));
    }

    SECTION("Unresolved reference")
    {
        REQUIRE(!wwrapper::isWorldSane("./world-packages/unresolvedReference.wwd"));
    }

    SECTION("No Army-Types")
    {
        REQUIRE(!wwrapper::isWorldSane("./world-packages/noArmyTypes.wwd"));
    }

    SECTION("No Banners")
    {
        REQUIRE(!wwrapper::isWorldSane("./world-packages/noBanners.wwd"));
    }

    SECTION("No Civilizations")
    {
        REQUIRE(!wwrapper::isWorldSane("./world-packages/noCivilizations.wwd"));
    }

    SECTION("No Colors")
    {
        REQUIRE(!wwrapper::isWorldSane("./world-packages/noColors.wwd"));
    }

    SECTION("No Settlement-Types")
    {
        REQUIRE(!wwrapper::isWorldSane("./world-packages/noSettlementTypes.wwd"));
    }

    SECTION("No Terrain-Types")
    {
        REQUIRE(!wwrapper::isWorldSane("./world-packages/noTerrainTypes.wwd"));
    }

    SECTION("No Unit-Types")
    {
        REQUIRE(!wwrapper::isWorldSane("./world-packages/noUnitTypes.wwd"));
    }

    SECTION("All is good")
    {
        REQUIRE(wwrapper::isWorldSane("./world-packages/world.wwd"));
    }
}
