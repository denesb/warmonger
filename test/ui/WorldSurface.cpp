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

#include <QFile>

#include <catch.hpp>

#include "io/File.h"
#include "ui/WorldSurface.h"
#include "utils/Constants.h"
#include "utils/Exception.h"
#include "utils/ToString.h"
#include "utils/Utils.h"

using namespace warmonger;

CATCH_TRANSLATE_EXCEPTION(utils::Exception& e)
{
    return e.getMessage().toStdString();
}

TEST_CASE("Failed to load surface metadata", "[WorldSurface][!hide]")
{
    core::World world("uuid0");

    SECTION("No package file")
    {
        REQUIRE_THROWS_AS(ui::WorldSurface("./worldsurface-packages/test_nonExistent.wsp", &world), utils::IOError);
    }

    SECTION("No metadata file in package")
    {
        REQUIRE_THROWS_AS(ui::WorldSurface("./worldsurface-packages/test_noMeta.wsp", &world), utils::IOError);
    }

    SECTION("Metadata file is not a file")
    {
        REQUIRE_THROWS_AS(ui::WorldSurface("./worldsurface-packages/test_metaDir.wsp", &world), utils::IOError);
    }

    SECTION("Metadata file not valid JSON")
    {
        REQUIRE_THROWS_AS(
            ui::WorldSurface("./worldsurface-packages/test_metaInvalidJson.wsp", &world), utils::ValueError);
    }

    SECTION("No resource file")
    {
        ui::WorldSurface s("./worldsurface-packages/test_noRcc.wsp", &world);
        REQUIRE_THROWS_AS(s.activate(), utils::IOError);
    }

    SECTION("Resource file is not a file")
    {
        ui::WorldSurface s("./worldsurface-packages/test_rccDir.wsp", &world);
        REQUIRE_THROWS_AS(s.activate(), utils::IOError);
    }

    SECTION("Resource file is invalid")
    {
        ui::WorldSurface s("./worldsurface-packages/test_rccInvalid.wsp", &world);
        REQUIRE_THROWS_AS(s.activate(), utils::IOError);
    }

    SECTION("Resource file, missing definition file")
    {
        ui::WorldSurface s("./worldsurface-packages/test_noDefinition.wsp", &world);
        REQUIRE_THROWS_AS(s.activate(), utils::IOError);
    }

    SECTION("Resource file, definition file - invalid json")
    {
        ui::WorldSurface s("./worldsurface-packages/test_definitionInvalidJson.wsp", &world);
        REQUIRE_THROWS_AS(s.activate(), utils::ValueError);
    }

    SECTION("Resource file, no hexmask")
    {
        ui::WorldSurface s("./worldsurface-packages/test_noHexMask.wsp", &world);
        REQUIRE_THROWS_AS(s.activate(), utils::IOError);
    }
}

TEST_CASE("Missing required images", "[WorldSurface][!hide]")
{
    std::unique_ptr<core::World> world(io::readWorld("./world-packages/world.wwd"));

    SECTION("Missing army-type images")
    {
        ui::WorldSurface s("./worldsurface-packages/test_noArmyTypeImage.wsp", world.get());
        REQUIRE_THROWS_AS(s.activate(), utils::IOError);
    }

    SECTION("Missing settlement-type images")
    {
        ui::WorldSurface s("./worldsurface-packages/test_noSettlementTypeImage.wsp", world.get());
        REQUIRE_THROWS_AS(s.activate(), utils::IOError);
    }

    SECTION("Missing banner images")
    {
        ui::WorldSurface s("./worldsurface-packages/test_noBannerImage.wsp", world.get());
        REQUIRE_THROWS_AS(s.activate(), utils::IOError);
    }

    SECTION("Missing terrain-type images")
    {
        ui::WorldSurface s("./worldsurface-packages/test_noTerrainTypeImage.wsp", world.get());
        REQUIRE_THROWS_AS(s.activate(), utils::IOError);
    }

    SECTION("Missing unit-type images")
    {
        ui::WorldSurface s("./worldsurface-packages/test_noUnitTypeImage.wsp", world.get());
        REQUIRE_THROWS_AS(s.activate(), utils::IOError);
    }
}

TEST_CASE("Can use Surface", "[WorldSurface][!hide]")
{
    std::unique_ptr<core::World> world(io::readWorld("./world-packages/world.wwd"));

    ui::WorldSurface s("./worldsurface-packages/test.wsp", world.get());

    SECTION("Can read metadata")
    {
        REQUIRE(s.objectName() == "test");
        REQUIRE(s.getName() == "Test surface");
        REQUIRE(s.getDescription() == "Surface used for testing");
    }

    SECTION("Resources not yet loaded")
    {
        // TODO test with an entity with a graphic component
        // QFile f(s.getObjectImagePath(world->getUnitTypes().front()));
        // REQUIRE(f.open(QIODevice::ReadOnly) == false);
    }

    SECTION("Surface activated")
    {
        s.activate();

        // TODO test with an entity with a graphic component
        // QFile f(s.getObjectImagePath(world->getUnitTypes().front()));
        // REQUIRE(f.open(QIODevice::ReadOnly) == true);
        REQUIRE(s.getTileWidth() == 110);
        REQUIRE(s.getTileHeight() == 128);
        REQUIRE(s.getNormalGridColor().name() == "#000000");
        REQUIRE(s.getFocusGridColor().name() == "#d59037");
        REQUIRE(s.hexContains(QPoint(0, 0)) == false);
        REQUIRE(s.hexContains(QPoint(109, 127)) == false);
        REQUIRE(s.hexContains(QPoint(55, 64)) == true);
    }

    SECTION("Resources unloaded")
    {
        s.activate();
        s.deactivate();

        // TODO test with an entity with a graphic component
        // QFile f(s.getObjectImagePath(world->getUnitTypes().front()));
        // REQUIRE(f.open(QIODevice::ReadOnly) == false);
    }
}

TEST_CASE("isWorldSurfaceSane", "[WorldSurface][!hide]")
{
    std::unique_ptr<core::World> world(io::readWorld("./world-packages/world.wwd"));

    SECTION("All is good")
    {
        REQUIRE(ui::isWorldSurfaceSane("./worldsurface-packages/test.wsp", world.get()));
    }

    SECTION("Invalid Json in meta-informatuin")
    {
        REQUIRE(!ui::isWorldSurfaceSane("./worldsurface-packages/test_metaInvalidJson.wsp", world.get()));
    }

    SECTION("Resource file, no hexmask")
    {
        REQUIRE(!ui::isWorldSurfaceSane("./worldsurface-packages/test_noHexMask.wsp", world.get()));
    }
}
