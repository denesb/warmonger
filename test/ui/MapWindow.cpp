#include "test/catch.hpp"
#include "ui/MapWindow.h"

using namespace warmonger;

TEST_CASE("Can move the window around", "[MapWindow]")
{
    ui::MapWindow mw(QSize(100, 100), QSize(10, 10));

    SECTION("Resize works")
    {
        REQUIRE(mw.getMapSize() == QSize(100, 100));
        REQUIRE(mw.getWindowSize() == QSize(10, 10));

        mw.setMapWidth(20);
        REQUIRE(mw.getWindowSize() == QSize(20, 10));
    }
}
