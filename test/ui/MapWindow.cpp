#include "test/catch.hpp"

#include "ui/MapWindow.h"
#include "utils/ToString.h"

using namespace warmonger;

TEST_CASE("Window is smaller than map", "[MapWindow]")
{
    ui::MapWindow mw(QRect(-100, -50, 100, 150), QSize(10, 10));

    SECTION("Can't set window position outside of map")
    {
        mw.setWindowPos(QPoint(10, 100));
        REQUIRE(mw.getWindowPos() == QPoint(-11, 89));

        mw.setWindowPos(QPoint(-110, 110));
        REQUIRE(mw.getWindowPos() == QPoint(-100, 89));

        mw.setWindowPos(QPoint(0, -60));
        REQUIRE(mw.getWindowPos() == QPoint(-11, -50));

        mw.setWindowPos(QPoint(-110, -110));
        REQUIRE(mw.getWindowPos() == QPoint(-100, -50));
    }

    SECTION("Can center window")
    {
        mw.centerWindow(QPoint(-10, 10));
        REQUIRE(mw.getWindowRect() == QRect(-15, 5, 10, 10));
    }

    SECTION("Centering window in the corner doesn't move window out of map")
    {
        mw.centerWindow(QPoint(-120, -120));
        REQUIRE(mw.getWindowPos() == QPoint(-100, -50));

        mw.centerWindow(QPoint(0, 100));
        REQUIRE(mw.getWindowPos() == QPoint(-11, 89));

        mw.centerWindow(QPoint(-50, 75));
        REQUIRE(mw.getWindowPos() == QPoint(-55, 70));

        mw.centerWindow(QPoint(-2, 90));
        REQUIRE(mw.getWindowPos() == QPoint(-11, 85));
    }

    SECTION("Can move window")
    {
        mw.centerWindow(QPoint(-15, 15));
        REQUIRE(mw.getWindowPos() == QPoint(-20, 10));

        mw.moveWindowBy(QPoint(-10, 10));
        REQUIRE(mw.getWindowPos() == QPoint(-30, 20));
    }

    SECTION("Can't move window out of map")
    {
        mw.centerWindow(QPoint(-30, 0));
        REQUIRE(mw.getWindowPos() == QPoint(-35, -5));

        mw.moveWindowBy(QPoint(-120, -120));
        REQUIRE(mw.getWindowPos() == QPoint(-100, -50));

        mw.moveWindowBy(QPoint(200, 200));
        REQUIRE(mw.getWindowPos() == QPoint(-11, 89));
    }
}

TEST_CASE("Window size is equal to map size", "[MapWindow]")
{
    ui::MapWindow mw(QRect(0, 0, 100, 100), QSize(10, 10));

    SECTION("Can't center window")
    {
    }
}
