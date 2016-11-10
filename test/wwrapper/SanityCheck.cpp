#include <QString>

#include "test/catch.hpp"

#include "core/World.h"
#include "io/File.h"
#include "io/JsonUnserializer.h"
#include "wwrapper/SanityCheck.h"

using namespace warmonger;

TEST_CASE("isWorldSane", "[SanityCheck]")
{
    SECTION("All is good")
    {
        REQUIRE(wwrapper::isWorldSane("./world-packages/world.wwd"));
    }

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
}

TEST_CASE("isCampaignMapSane", "[SanityCheck]")
{
    io::JsonUnserializer unserializer;
    std::unique_ptr<core::World> world(io::readWorld("./world-packages/world.wwd", unserializer));

    SECTION("All is good")
    {
        REQUIRE(wwrapper::isCampaignMapSane("./campaignmap-packages/campaignmap.wmd", world.get()));
    }

    SECTION("Inexistent")
    {
        REQUIRE(!wwrapper::isCampaignMapSane("./campaignmap-packages/inexistent.wmd", world.get()));
    }

    SECTION("Invalid JSON")
    {
        REQUIRE(!wwrapper::isCampaignMapSane("./campaignmap-packages/invalidJson.wmd", world.get()));
    }

    SECTION("Unresolved reference")
    {
        REQUIRE(!wwrapper::isCampaignMapSane("./campaignmap-packages/unresolvedReference.wmd", world.get()));
    }

    SECTION("Invalid value")
    {
        REQUIRE(!wwrapper::isCampaignMapSane("./campaignmap-packages/invalidValue.wmd", world.get()));
    }
}
