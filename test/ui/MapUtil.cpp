#include "test/catch.hpp"

#include "ui/MapUtil.h"
#include "utils/ToString.h"

using namespace warmonger;

std::pair<float, float> getScaleFactor(const QMatrix4x4 &matrix);
std::pair<float, float> getTranslation(const QMatrix4x4 &matrix);

TEST_CASE("Scaling, equal sided frame, content is larger than frame", "[MapWindow]")
{
    QRectF frame(0.0, 0.0, 100, 100);

    SECTION("Equal sided content")
    {
        QRectF content(0.0, 0.0, 200, 200);

        const std::pair<float, float> scaleFactor = getScaleFactor(ui::centerIn(content, frame));

        REQUIRE(std::get<0>(scaleFactor) == std::get<1>(scaleFactor));
        REQUIRE(std::get<0>(scaleFactor) == 0.5);
    }

    SECTION("Equal sided frame, unequal sided content")
    {
        QRectF content(0.0, 0.0, 200, 400);

        const std::pair<float, float> scaleFactor = getScaleFactor(ui::centerIn(content, frame));

        REQUIRE(std::get<0>(scaleFactor) == std::get<1>(scaleFactor));
        REQUIRE(std::get<0>(scaleFactor) == 0.25);
    }
}

TEST_CASE("Scaling, unequal sided frame, content is larger than frame", "[MapWindow]")
{
    QRectF frame(0.0, 0.0, 200, 100);

    SECTION("Equal sided content")
    {
        QRectF content(0.0, 0.0, 400, 400);

        const std::pair<float, float> scaleFactor = getScaleFactor(ui::centerIn(content, frame));

        REQUIRE(std::get<0>(scaleFactor) == std::get<1>(scaleFactor));
        REQUIRE(std::get<0>(scaleFactor) == 0.25);
    }

    SECTION("Unequal sided content")
    {
        QRectF content(0.0, 0.0, 400, 800);

        const std::pair<float, float> scaleFactor = getScaleFactor(ui::centerIn(content, frame));

        REQUIRE(std::get<0>(scaleFactor) == std::get<1>(scaleFactor));
        REQUIRE(std::get<0>(scaleFactor) == 0.125);
    }
}

TEST_CASE("No scaling if content is smaller than frame", "[MapWindow]")
{
    QRectF frame(0.0, 0.0, 200, 200);

    SECTION("Equal sided content")
    {
        QRectF content(0.0, 0.0, 100, 100);

        const std::pair<float, float> scaleFactor = getScaleFactor(ui::centerIn(content, frame));

        REQUIRE(std::get<0>(scaleFactor) == std::get<1>(scaleFactor));
        REQUIRE(std::get<0>(scaleFactor) == 1);
    }

    SECTION("Unequal sided content")
    {
        QRectF content(0.0, 0.0, 150, 100);

        const std::pair<float, float> scaleFactor = getScaleFactor(ui::centerIn(content, frame));

        REQUIRE(std::get<0>(scaleFactor) == std::get<1>(scaleFactor));
        REQUIRE(std::get<0>(scaleFactor) == 1);
    }
}

TEST_CASE("Translation", "[MapWindow]")
{
    QRectF frame(0.0, 0.0, 200, 200);

    SECTION("Content is inside and smaller")
    {
        QRectF content(0.0, 0.0, 100, 80);

        const QMatrix4x4 transformation = ui::centerIn(content, frame);
        const std::pair<float, float> scaleFactor = getScaleFactor(transformation);

        REQUIRE(std::get<0>(scaleFactor) == std::get<1>(scaleFactor));
        REQUIRE(std::get<0>(scaleFactor) == 1);

        const std::pair<float, float> translation = getTranslation(transformation);

        REQUIRE(std::get<0>(translation) == 50);
        REQUIRE(std::get<1>(translation) == 60);
    }

    SECTION("Content is outside and smaller")
    {
        QRectF content(-100.0, -70.0, 100, 80);

        const QMatrix4x4 transformation = ui::centerIn(content, frame);
        const std::pair<float, float> scaleFactor = getScaleFactor(transformation);

        REQUIRE(std::get<0>(scaleFactor) == std::get<1>(scaleFactor));
        REQUIRE(std::get<0>(scaleFactor) == 1);

        const std::pair<float, float> translation = getTranslation(transformation);

        REQUIRE(std::get<0>(translation) == 150);
        REQUIRE(std::get<1>(translation) == 130);
    }

    SECTION("Content is outside and larger")
    {
        QRectF content(-200.0, 100.0, 400, 380);

        const QMatrix4x4 transformation = ui::centerIn(content, frame);
        const std::pair<float, float> scaleFactor = getScaleFactor(transformation);

        REQUIRE(std::get<0>(scaleFactor) == std::get<1>(scaleFactor));
        REQUIRE(std::get<0>(scaleFactor) == 0.5);

        const std::pair<float, float> translation = getTranslation(transformation);

        REQUIRE(std::get<0>(translation) == 100);
        REQUIRE(std::get<1>(translation) == -45);
    }
}

std::pair<float, float> getScaleFactor(const QMatrix4x4 &matrix)
{
    const QVector4D firstRow = matrix.row(0);
    const QVector4D secondRow = matrix.row(1);

    return std::make_pair(firstRow.x(), secondRow.y());
}

std::pair<float, float> getTranslation(const QMatrix4x4 &matrix)
{
    const QVector4D firstRow = matrix.row(0);
    const QVector4D secondRow = matrix.row(1);

    return std::make_pair(firstRow.w(), secondRow.w());

}
