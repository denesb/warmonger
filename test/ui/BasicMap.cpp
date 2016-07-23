#include "test/catch.hpp"

#include "ui/BasicMap.h"
#include "utils/ToString.h"

using namespace warmonger;

class TestBasicMap : public ui::BasicMap
{
public:
    TestBasicMap(const QRect& mapRect, const QSize& windowSize)
        : BasicMap(nullptr)
    {
        this->setMapRect(mapRect);
        this->setWidth(windowSize.width());
        this->setHeight(windowSize.height());
    }

    QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*) override
    {
        return nullptr;
    }

    void setMapRect(const QRect& rect)
    {
        BasicMap::setMapRect(rect);
    }
};

TEST_CASE("Window size is smaller than map size", "[BasicMap]")
{
    TestBasicMap tm(QRect(-100, -50, 100, 150), QSize(10, 10));

    SECTION("Can't set window position outside of map")
    {
        tm.setWindowPos(QPoint(10, 100));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(-11, 89));

        tm.setWindowPos(QPoint(-110, 110));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(-100, 89));

        tm.setWindowPos(QPoint(0, -60));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(-11, -50));

        tm.setWindowPos(QPoint(-110, -110));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(-100, -50));
    }

    SECTION("Can center window")
    {
        tm.centerWindow(QPoint(-10, 10));
        REQUIRE(tm.getWindowRect() == QRect(-15, 5, 10, 10));
    }

    SECTION("Centering window in the corner doesn't move window out of map")
    {
        tm.centerWindow(QPoint(-120, -120));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(-100, -50));

        tm.centerWindow(QPoint(0, 100));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(-11, 89));

        tm.centerWindow(QPoint(-50, 75));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(-55, 70));

        tm.centerWindow(QPoint(-2, 90));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(-11, 85));
    }

    SECTION("Can move window")
    {
        tm.centerWindow(QPoint(-15, 15));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(-20, 10));

        tm.moveWindowBy(QPoint(-10, 10));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(-30, 20));
    }

    SECTION("Can't move window out of map")
    {
        tm.centerWindow(QPoint(-30, 0));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(-35, -5));

        tm.moveWindowBy(QPoint(-120, -120));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(-100, -50));

        tm.moveWindowBy(QPoint(200, 200));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(-11, 89));
    }
}

TEST_CASE("Window width is equal to map width", "[BasicMap]")
{
    TestBasicMap tm(QRect(-100, -100, 200, 200), QSize(200, 100));

    SECTION("Can't move window on the x axis at all")
    {
        REQUIRE(tm.getWindowRect().topLeft().x() == -100);

        tm.centerWindow(QPoint(0, 0));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(-100, -50));

        tm.moveWindowBy(QPoint(10, 10));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(-100, -40));
    }
}

TEST_CASE("Window width is larger than map width", "[BasicMap]")
{
    TestBasicMap tm(QRect(10, 10, 10, 40), QSize(20, 20));

    SECTION("Can't move window on the x axis at all")
    {
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(5, 10));

        tm.centerWindow(QPoint(15, 35));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(5, 25));
    }
}

TEST_CASE("Window height is equal to map height", "[BasicMap]")
{
    TestBasicMap tm(QRect(-10, -10, 40, 20), QSize(20, 20));

    SECTION("Can't move window on the y axis at all")
    {
        REQUIRE(tm.getWindowRect().topLeft().y() == -10);

        tm.centerWindow(QPoint(10, 10));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(0, -10));

        tm.moveWindowBy(QPoint(5, 5));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(5, -10));
    }
}

TEST_CASE("Window height is larger than map height", "[BasicMap]")
{
    TestBasicMap tm(QRect(10, 10, 40, 10), QSize(20, 20));

    SECTION("Can't move window on the y axis at all")
    {
        REQUIRE(tm.getWindowRect().topLeft().y() == 5);

        tm.centerWindow(QPoint(35, 15));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(25, 5));

        tm.moveWindowBy(QPoint(-5, -5));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(20, 5));
    }
}

TEST_CASE("Window size is equal to map size", "[BasicMap]")
{
    TestBasicMap tm(QRect(-10, -10, 20, 20), QSize(20, 20));

    SECTION("Can't change window pos at all")
    {
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(-10, -10));

        tm.centerWindow(QPoint(0, 0));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(-10, -10));

        tm.moveWindowBy(QPoint(5, 5));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(-10, -10));
    }
}

TEST_CASE("Window size is larger than map size", "[BasicMap]")
{
    TestBasicMap tm(QRect(10, 10, 10, 10), QSize(20, 20));

    SECTION("Can't change window pos at all")
    {
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(5, 5));

        tm.centerWindow(QPoint(35, 15));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(5, 5));

        tm.moveWindowBy(QPoint(5, 5));
        REQUIRE(tm.getWindowRect().topLeft() == QPoint(5, 5));
    }
}

TEST_CASE("Window pos is updated when map size changes", "[BasicMap]")
{
    TestBasicMap tm(QRect(10, 10, 40, 40), QSize(20, 20));

    tm.setWindowPos(QPoint(29, 29));

    REQUIRE(tm.getWindowRect().topLeft() == QPoint(29, 29));

    tm.setMapRect(QRect(10, 10, 30, 30));

    REQUIRE(tm.getWindowRect().topLeft() == QPoint(19, 19));
}
