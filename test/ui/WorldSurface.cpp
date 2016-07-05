#include <memory>

#include <QFile>

#include "test/catch.hpp"

#include "ui/WorldSurface.h"
#include "Exception.h"

using namespace warmonger;

std::unique_ptr<ui::WorldSurface> createWorldSurface(const QString& path)
{
    return std::unique_ptr<ui::WorldSurface>(new ui::WorldSurface(path));
}

TEST_CASE("Failed to load surface metadata", "[WorldSurface]")
{
    SECTION("No package file")
    {
        REQUIRE_THROWS_AS(
            createWorldSurface("./dev_nonexistent.wsp"),
            IOError
        );
    }

    SECTION("No metadata file in package")
    {
        REQUIRE_THROWS_AS(
            createWorldSurface("./dev_nometafile.wsp"),
            IOError
        );
    }

    SECTION("Metadata file is not a file")
    {
        REQUIRE_THROWS_AS(
            createWorldSurface("./dev_metadir.wsp"),
            IOError
        );
    }

    SECTION("Metadata file not valid JSON")
    {
        REQUIRE_THROWS_AS(
            createWorldSurface("./dev_metainvalidjson.wsp"),
            ValueError
        );
    }

    SECTION("No resource file")
    {
        ui::WorldSurface s("./dev_norcc.wsp");
        REQUIRE_THROWS_AS(
            s.activate(),
            IOError
        );
    }

    SECTION("Resource file is not a file")
    {
        ui::WorldSurface s("./dev_rccdir.wsp");
        REQUIRE_THROWS_AS(
            s.activate(),
            IOError
        );
    }

    SECTION("Resource file is invalid")
    {
        ui::WorldSurface s("./dev_rccinvalid.wsp");
        REQUIRE_THROWS_AS(
            s.activate(),
            IOError
        );
    }

    SECTION("Resource file, missing definition file")
    {
        ui::WorldSurface s("./dev_rccnodefinition.wsp");
        REQUIRE_THROWS_AS(
            s.activate(),
            IOError
        );
    }

    SECTION("Resource file, definition file - invalid json")
    {
        ui::WorldSurface s("./dev_rccdefinitioninvalidjson.wsp");
        REQUIRE_THROWS_AS(
            s.activate(),
            ValueError
        );
    }

    SECTION("Resource file, no hexmask")
    {
        ui::WorldSurface s("./dev_rccnohexmask.wsp");
        REQUIRE_THROWS_AS(
            s.activate(),
            IOError
        );
    }
}

TEST_CASE("Can use Surface", "[WorldSurface]")
{
    ui::WorldSurface s("./dev.wsp");

    SECTION("Can read metadata")
    {
        REQUIRE(s.objectName() == "dev");
        REQUIRE(s.getDisplayName() == "Development surface");
        REQUIRE(s.getDescription() == "Surface used for development");
    }

    SECTION("Resources not yet loaded")
    {
        QFile f(":/surface/dev.wsd");
        REQUIRE(f.open(QIODevice::ReadOnly) == false);
    }

    SECTION("Surface activated")
    {
        s.activate();

        QFile f(":/surface/dev.wsd");
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

        QFile f(":/surface/dev.wsd");
        REQUIRE(f.open(QIODevice::ReadOnly) == false);
    }
}
