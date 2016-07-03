#include <memory>

#include "test/catch.hpp"

#include "io/Exception.h"
#include "ui/WorldSurface.h"

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
            io::FileIOError
        );
    }

    SECTION("No metadata file in package")
    {
        REQUIRE_THROWS_AS(
            createWorldSurface("./dev_nometafile.wsp"),
            io::FileIOError
        );
    }

    SECTION("Metadata file is not a file")
    {
        REQUIRE_THROWS_AS(
            createWorldSurface("./dev_metadir.wsp"),
            io::FileIOError
        );
    }
}

TEST_CASE("Can load Surface metadata", "[WorldSurface]")
{
    ui::WorldSurface s("./dev.wsp");

    SECTION("Can read metadata")
    {
        REQUIRE(s.objectName() == "dev");
        REQUIRE(s.getDisplayName() == "Development surface");
        REQUIRE(s.getDescription() == "Surface used for development");
    }
}
