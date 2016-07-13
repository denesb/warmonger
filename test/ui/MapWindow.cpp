#include "test/catch.hpp"

#include "ui/MapWindow.h"
#include "utils/ToString.h"

using namespace warmonger;

TEST_CASE("Window size is smaller than map size", "[MapWindow]")
{
    ui::MapWindow mw(QRect(-100, -50, 100, 150), QSize(10, 10));

    REQUIRE(mw.getWindowPos() == QPoint(-100, -50));

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

TEST_CASE("Window width is equal to map width", "[MapWindow]")
{
    ui::MapWindow mw(QRect(-100, -100, 200, 200), QSize(200, 100));

    SECTION("Can't move window on the x axis at all")
    {
        REQUIRE(mw.getWindowPos() == QPoint(-100, -100));

        mw.centerWindow(QPoint(0, 0));
        REQUIRE(mw.getWindowPos() == QPoint(-100, -50));

        mw.moveWindowBy(QPoint(10, 10));
        REQUIRE(mw.getWindowPos() == QPoint(-100, -40));
    }
}

TEST_CASE("Window width is larger than map width", "[MapWindow]")
{
    ui::MapWindow mw(QRect(10, 10, 10, 40), QSize(20, 20));

    SECTION("Can't move window on the x axis at all")
    {
        REQUIRE(mw.getWindowPos() == QPoint(5, 10));

        mw.centerWindow(QPoint(15, 35));
        REQUIRE(mw.getWindowPos() == QPoint(5, 25));
    }
}

TEST_CASE("Window height is equal to map height", "[MapWindow]")
{
    ui::MapWindow mw(QRect(-10, -10, 40, 20), QSize(20, 20));

    SECTION("Can't move window on the y axis at all")
    {
        REQUIRE(mw.getWindowPos() == QPoint(-10, -10));

        mw.centerWindow(QPoint(10, 10));
        REQUIRE(mw.getWindowPos() == QPoint(0, -10));

        mw.moveWindowBy(QPoint(5, 5));
        REQUIRE(mw.getWindowPos() == QPoint(5, -10));
    }
}

TEST_CASE("Window height is larger than map height", "[MapWindow]")
{
    ui::MapWindow mw(QRect(10, 10, 40, 10), QSize(20, 20));

    SECTION("Can't move window on the y axis at all")
    {
        REQUIRE(mw.getWindowPos() == QPoint(10, 5));

        mw.centerWindow(QPoint(35, 15));
        REQUIRE(mw.getWindowPos() == QPoint(25, 5));

        mw.moveWindowBy(QPoint(-5, -5));
        REQUIRE(mw.getWindowPos() == QPoint(20, 5));
    }
}

TEST_CASE("Window size is equal to map size", "[MapWindow]")
{
    ui::MapWindow mw(QRect(-10, -10, 20, 20), QSize(20, 20));

    SECTION("Can't change window pos at all")
    {
        REQUIRE(mw.getWindowPos() == QPoint(-10, -10));

        mw.centerWindow(QPoint(0, 0));
        REQUIRE(mw.getWindowPos() == QPoint(-10, -10));

        mw.moveWindowBy(QPoint(5, 5));
        REQUIRE(mw.getWindowPos() == QPoint(-10, -10));
    }
}

TEST_CASE("Window size is larger than map size", "[MapWindow]")
{
    ui::MapWindow mw(QRect(10, 10, 10, 10), QSize(20, 20));

    SECTION("Can't change window pos at all")
    {
        REQUIRE(mw.getWindowPos() == QPoint(5, 5));

        mw.centerWindow(QPoint(35, 15));
        REQUIRE(mw.getWindowPos() == QPoint(5, 5));

        mw.moveWindowBy(QPoint(5, 5));
        REQUIRE(mw.getWindowPos() == QPoint(5, 5));
    }
}
