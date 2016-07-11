#include "test/catch.hpp"

#include "ui/MapWindow.h"
#include "utils/ToString.h"

using namespace warmonger;

TEST_CASE("Can move the window around", "[MapWindow]")
{
    ui::MapWindow mw(QSize(100, 100), QSize(10, 10));

    SECTION("Resize works")
    {
        REQUIRE(mw.getMapSize() == QSize(100, 100));
        REQUIRE(mw.getWindowSize() == QSize(10, 10));

        mw.setMapWidth(20);
        REQUIRE(mw.getMapSize() == QSize(20, 100));
    }

    SECTION("Can't set window position outside of map")
    {
        mw.setWindowPos(QPoint(110, 110));
        REQUIRE(mw.getWindowPos() == QPoint(89, 89));

        mw.setWindowPos(QPoint(-110, 110));
        REQUIRE(mw.getWindowPos() == QPoint(0, 89));

        mw.setWindowPos(QPoint(110, -110));
        REQUIRE(mw.getWindowPos() == QPoint(89, 0));

        mw.setWindowPos(QPoint(-110, -110));
        REQUIRE(mw.getWindowPos() == QPoint(0, 0));
    }

    SECTION("Can center window")
    {
        mw.centerWindow(QPoint(10, 10));
        REQUIRE(mw.getWindowRect() == QRect(5, 5, 10, 10));
    }

    SECTION("Centering window in the corner doesn't move window out of map")
    {
        mw.centerWindow(QPoint(0, 0));
        REQUIRE(mw.getWindowRect() == QRect(0, 0, 10, 10));

        mw.centerWindow(QPoint(98, 98));
        REQUIRE(mw.getWindowRect() == QRect(89, 89, 10, 10));

        mw.centerWindow(QPoint(50, 95));
        REQUIRE(mw.getWindowRect() == QRect(45, 89, 10, 10));

        mw.centerWindow(QPoint(95, 20));
        REQUIRE(mw.getWindowRect() == QRect(89, 15, 10, 10));
    }

    SECTION("Can move window")
    {
        mw.centerWindow(QPoint(15, 15));
        REQUIRE(mw.getWindowRect() == QRect(10, 10, 10, 10));

        mw.moveWindowBy(QPoint(10, 10));
        REQUIRE(mw.getWindowRect() == QRect(20, 20, 10, 10));
    }

    SECTION("Can't move window out of map")
    {
        mw.centerWindow(QPoint(15, 15));
        REQUIRE(mw.getWindowRect() == QRect(10, 10, 10, 10));

        mw.moveWindowBy(QPoint(-20, -20));
        REQUIRE(mw.getWindowRect() == QRect(0, 0, 10, 10));

        mw.moveWindowBy(QPoint(200, 200));
        REQUIRE(mw.getWindowRect() == QRect(89, 89, 10, 10));
    }
}
