#include "test/catch.hpp"

#include "ui/BasicMiniMap.h"
#include "utils/ToString.h"

using namespace warmonger;

class TestBasicMiniMap : public ui::BasicMiniMap
{
public:
    TestBasicMiniMap(const QRect& mapRect, const QSize& frame)
        : BasicMiniMap(nullptr)
    {
        this->setMapRect(mapRect);
        this->setWidth(frame.width());
        this->setHeight(frame.height());
    }

    QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*) override
    {
        return nullptr;
    }

    void setMapRect(const QRect& rect)
    {
        BasicMiniMap::setMapRect(rect);
    }
};

TEST_CASE("Window size is larger than frame size", "[BasicMiniMap]")
{
    TestBasicMiniMap tm(QRect(10, 10, 100, 100), QSize(10, 10));
    tm.setWindowRect(QRect(0, 0, 100, 100));

    const QRect expectedWindowRect = QRect(0, 0, 10, 10);
    REQUIRE(tm.getWindowRect() == expectedWindowRect);

    SECTION("Can't move window at all")
    {
        tm.setWindowRect(QRect(1, 1, 10, 10));
        REQUIRE(tm.getWindowRect() == expectedWindowRect);

        tm.centerWindow(QPoint(11, 11));
        REQUIRE(tm.getWindowRect() == expectedWindowRect);

        tm.moveWindowBy(QPoint(1, 1));
        REQUIRE(tm.getWindowRect() == expectedWindowRect);
    }
}
