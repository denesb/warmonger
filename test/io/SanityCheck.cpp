/**
 * Copyright (C) 2015-2017 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <QString>

#include "test/catch.hpp"

#include "core/World.h"
#include "io/File.h"
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

TEST_CASE("isMapSane", "[SanityCheck]")
{
    std::unique_ptr<core::World> world(io::readWorld("./world-packages/world.wwd"));

    SECTION("All is good")
    {
        REQUIRE(io::isMapSane("./map-packages/map.wmd", world.get()));
    }

    SECTION("Inexistent")
    {
        REQUIRE(!io::isMapSane("./map-packages/inexistent.wmd", world.get()));
    }

    SECTION("Invalid JSON")
    {
        REQUIRE(!io::isMapSane("./map-packages/invalidJson.wmd", world.get()));
    }

    SECTION("Unresolved reference")
    {
        REQUIRE(!io::isMapSane("./map-packages/unresolvedReference.wmd", world.get()));
    }

    SECTION("Invalid value")
    {
        REQUIRE(!io::isMapSane("./map-packages/invalidValue.wmd", world.get()));
    }
}
