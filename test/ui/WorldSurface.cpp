#include <QFile>

#include "test/catch.hpp"

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

TEST_CASE("Failed to load surface metadata", "[WorldSurface]")
{
    core::World world;

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
        REQUIRE_THROWS_AS(ui::WorldSurface("./worldsurface-packages/test_metaInvalidJson.wsp", &world), utils::ValueError);
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

TEST_CASE("Missing some required images", "[WorldSurface]")
{
    core::World world;

    ui::WorldSurface s("./worldsurface-packages/test_noRequiredStaticImage.wsp", &world);

    REQUIRE_THROWS_AS(s.activate(), utils::IOError);
}

TEST_CASE("Can use Surface", "[WorldSurface]")
{
    core::World world;

    ui::WorldSurface s("./worldsurface-packages/test.wsp", &world);

    SECTION("Can read metadata")
    {
        REQUIRE(s.objectName() == "test");
        REQUIRE(s.getDisplayName() == "Test surface");
        REQUIRE(s.getDescription() == "Surface used for testing");
    }

    SECTION("Resources not yet loaded")
    {
        QFile f(s.getImagePath(ui::WorldSurface::Image::HoverValid));
        REQUIRE(f.open(QIODevice::ReadOnly) == false);
    }

    SECTION("Surface activated")
    {
        s.activate();

        QFile f(s.getImagePath(ui::WorldSurface::Image::HoverValid));
        REQUIRE(f.open(QIODevice::ReadOnly) == true);
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

        QFile f(s.getImagePath(ui::WorldSurface::Image::HoverValid));
        REQUIRE(f.open(QIODevice::ReadOnly) == false);
    }
}
