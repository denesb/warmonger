#include <QString>

#include "test/catch.hpp"

#include "core/World.h"
#include "io/File.h"
#include "io/JsonUnserializer.h"
#include "io/SanityCheck.h"

using namespace warmonger;

TEST_CASE("isWorldSane", "[SanityCheck]")
{
    SECTION("All is good")
    {
        REQUIRE(io::isWorldSane("./world-packages/world.wwd"));
    }

    SECTION("Inexistent")
    {
        REQUIRE(!io::isWorldSane("./world-packages/inexistent.wwd"));
    }

    SECTION("Invalid JSON")
    {
        REQUIRE(!io::isWorldSane("./world-packages/invalidJson.wwd"));
    }

    SECTION("Unresolved reference")
    {
        REQUIRE(!io::isWorldSane("./world-packages/unresolvedReference.wwd"));
    }

    SECTION("No Army-Types")
    {
        REQUIRE(!io::isWorldSane("./world-packages/noArmyTypes.wwd"));
    }

    SECTION("No Banners")
    {
        REQUIRE(!io::isWorldSane("./world-packages/noBanners.wwd"));
    }

    SECTION("No Civilizations")
    {
        REQUIRE(!io::isWorldSane("./world-packages/noCivilizations.wwd"));
    }

    SECTION("No Colors")
    {
        REQUIRE(!io::isWorldSane("./world-packages/noColors.wwd"));
    }

    SECTION("No Settlement-Types")
    {
        REQUIRE(!io::isWorldSane("./world-packages/noSettlementTypes.wwd"));
    }

    SECTION("No Terrain-Types")
    {
        REQUIRE(!io::isWorldSane("./world-packages/noTerrainTypes.wwd"));
    }

    SECTION("No Unit-Types")
    {
        REQUIRE(!io::isWorldSane("./world-packages/noUnitTypes.wwd"));
    }
}

TEST_CASE("isCampaignMapSane", "[SanityCheck]")
{
    io::JsonUnserializer unserializer;
    std::unique_ptr<core::World> world(io::readWorld("./world-packages/world.wwd", unserializer));

    SECTION("All is good")
    {
        REQUIRE(io::isCampaignMapSane("./campaignmap-packages/campaignmap.wmd", world.get()));
    }

    SECTION("Inexistent")
    {
        REQUIRE(!io::isCampaignMapSane("./campaignmap-packages/inexistent.wmd", world.get()));
    }

    SECTION("Invalid JSON")
    {
        REQUIRE(!io::isCampaignMapSane("./campaignmap-packages/invalidJson.wmd", world.get()));
    }

    SECTION("Unresolved reference")
    {
        REQUIRE(!io::isCampaignMapSane("./campaignmap-packages/unresolvedReference.wmd", world.get()));
    }

    SECTION("Invalid value")
    {
        REQUIRE(!io::isCampaignMapSane("./campaignmap-packages/invalidValue.wmd", world.get()));
    }
}
