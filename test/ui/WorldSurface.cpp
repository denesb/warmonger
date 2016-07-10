#include <memory>

#include <QFile>
#include <QGuiApplication>

#include "test/catch.hpp"

#include "ui/WorldSurface.h"
#include "utils/Exception.h"

using namespace warmonger;

CATCH_TRANSLATE_EXCEPTION(utils::Exception& e) {
    return e.getMessage().toStdString();
}

TEST_CASE("Failed to load surface metadata", "[WorldSurface]")
{
    core::World world;

    int argc = 0;
    char **argv = nullptr;
    QGuiApplication app(argc, argv);
    QQuickWindow window;

    SECTION("No package file")
    {
        REQUIRE_THROWS_AS(
            ui::WorldSurface("./dev_nonexistent.wsp", &world, &window),
            utils::IOError
        );
    }

    SECTION("No metadata file in package")
    {
        REQUIRE_THROWS_AS(
            ui::WorldSurface("./dev_nometafile.wsp", &world, &window),
            utils::IOError
        );
    }

    SECTION("Metadata file is not a file")
    {
        REQUIRE_THROWS_AS(
            ui::WorldSurface("./dev_metadir.wsp", &world, &window),
            utils::IOError
        );
    }

    SECTION("Metadata file not valid JSON")
    {
        REQUIRE_THROWS_AS(
            ui::WorldSurface("./dev_metainvalidjson.wsp", &world, &window),
            utils::ValueError
        );
    }

    SECTION("No resource file")
    {
        ui::WorldSurface s("./dev_norcc.wsp", &world, &window);
        REQUIRE_THROWS_AS(
            s.activate(),
            utils::IOError
        );
    }

    SECTION("Resource file is not a file")
    {
        ui::WorldSurface s("./dev_rccdir.wsp", &world, &window);
        REQUIRE_THROWS_AS(
            s.activate(),
            utils::IOError
        );
    }

    SECTION("Resource file is invalid")
    {
        ui::WorldSurface s("./dev_rccinvalid.wsp", &world, &window);
        REQUIRE_THROWS_AS(
            s.activate(),
            utils::IOError
        );
    }

    SECTION("Resource file, missing definition file")
    {
        ui::WorldSurface s("./dev_rccnodefinition.wsp", &world, &window);
        REQUIRE_THROWS_AS(
            s.activate(),
            utils::IOError
        );
    }

    SECTION("Resource file, definition file - invalid json")
    {
        ui::WorldSurface s("./dev_rccdefinitioninvalidjson.wsp", &world, &window);
        REQUIRE_THROWS_AS(
            s.activate(),
            utils::ValueError
        );
    }

    SECTION("Resource file, no hexmask")
    {
        ui::WorldSurface s("./dev_rccnohexmask.wsp", &world, &window);
        REQUIRE_THROWS_AS(
            s.activate(),
            utils::IOError
        );
    }
}

TEST_CASE("Can use Surface", "[WorldSurface]")
{
    core::World world;

    int argc = 0;
    char **argv = nullptr;
    QGuiApplication app(argc, argv);
    QQuickWindow window;

    ui::WorldSurface s("./dev.wsp", &world, &window);

    SECTION("Can read metadata")
    {
        REQUIRE(s.objectName() == "dev");
        REQUIRE(s.getDisplayName() == "Development surface");
        REQUIRE(s.getDescription() == "Surface used for development");
    }

    SECTION("Resources not yet loaded")
    {
        QFile f(s.getPrefix() + "dev.wsd");
        REQUIRE(f.open(QIODevice::ReadOnly) == false);
    }

    SECTION("Surface activated")
    {
        s.activate();

        QFile f(s.getPrefix() + "dev.wsd");
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

        QFile f(s.getPrefix() + "dev.wsd");
        REQUIRE(f.open(QIODevice::ReadOnly) == false);
    }
}
