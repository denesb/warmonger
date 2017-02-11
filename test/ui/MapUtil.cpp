/**
 * Copyright (C) 2015-2017 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "ui/MapUtil.h"
#include "core/MapGenerator.h"
#include "test/catch.hpp"
#include "ui/WorldSurface.h"
#include "utils/ToString.h"

using namespace warmonger;

std::pair<float, float> getScaleFactor(const QMatrix4x4& matrix);
std::pair<float, float> getTranslation(const QMatrix4x4& matrix);
std::ostream& operator<<(std::ostream& s, const core::MapNodeNeighbours& n);

TEST_CASE("Scaling, equal sided frame, content is larger than frame", "[centerIn]")
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

TEST_CASE("Scaling, unequal sided frame, content is larger than frame", "[centerIn]")
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

TEST_CASE("No scaling if content is smaller than frame", "[centerIn]")
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

TEST_CASE("Translation", "[centerIn]")
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

TEST_CASE("Move tests", "[moveTo]")
{
    QPoint p1(0, 0), p2(0, 0);

    SECTION("Points are at the same position")
    {
        const std::pair<float, float> translation = getTranslation(ui::moveTo(p1, p2));

        REQUIRE(std::get<0>(translation) == std::get<1>(translation));
        REQUIRE(std::get<0>(translation) == 0);
    }

    SECTION("Points are not at the same position, reference point is (0, 0)")
    {
        p1.setX(10);

        std::pair<float, float> t = getTranslation(ui::moveTo(p1, p2));

        REQUIRE(std::get<0>(t) == -10);
        REQUIRE(std::get<1>(t) == 0);

        p1.setY(20);

        t = getTranslation(ui::moveTo(p1, p2));

        REQUIRE(std::get<0>(t) == -10);
        REQUIRE(std::get<1>(t) == -20);

        p1.setY(-31);

        t = getTranslation(ui::moveTo(p1, p2));

        REQUIRE(std::get<0>(t) == -10);
        REQUIRE(std::get<1>(t) == 31);

        p1.setX(-301);

        t = getTranslation(ui::moveTo(p1, p2));

        REQUIRE(std::get<0>(t) == 301);
        REQUIRE(std::get<1>(t) == 31);
    }

    SECTION("Points are not at the same position, reference point is not (0, 0)")
    {
        p2.setX(-11);
        p2.setY(-22);

        std::pair<float, float> t = getTranslation(ui::moveTo(p1, p2));

        REQUIRE(std::get<0>(t) == -11);
        REQUIRE(std::get<1>(t) == -22);

        p1.setX(11);
        p1.setY(40);

        t = getTranslation(ui::moveTo(p1, p2));

        REQUIRE(std::get<0>(t) == -22);
        REQUIRE(std::get<1>(t) == -62);
    }
}

TEST_CASE("Visibility test", "[visibleContents]")
{
    std::vector<core::MapNode*> mapNodes = core::generateMapNodes(2);
    core::generateMapNodeNames(mapNodes);

    std::vector<core::CampaignMap::Content> contents;

    for (core::MapNode* mapNode : mapNodes)
    {
        contents.push_back(std::tuple<core::MapNode*, core::Settlement*, core::Army*>(mapNode, nullptr, nullptr));
    }

    const QSize tileSize(10, 10);
    const std::map<core::MapNode*, QPoint> mapNodesPos = ui::positionMapNodes(mapNodes.front(), tileSize);

    SECTION("All nodes are visible")
    {
        QRect window(-100, -100, 200, 200);

        std::vector<core::CampaignMap::Content> visibleContents =
            ui::visibleContents(contents, mapNodesPos, tileSize, window);

        REQUIRE(visibleContents.size() == mapNodes.size());
    }

    SECTION("One node is visible")
    {
        QRect window(4, 4, 2, 2);

        std::vector<core::CampaignMap::Content> visibleContents =
            ui::visibleContents(contents, mapNodesPos, tileSize, window);

        REQUIRE(visibleContents.size() == 1);
        REQUIRE(std::get<core::MapNode*>(visibleContents.front())->objectName() ==
            std::get<core::MapNode*>(contents.front())->objectName());
    }

    SECTION("Half of the map is visible")
    {
        QRect window(6, -100, 200, 200);

        std::vector<core::CampaignMap::Content> visibleContents =
            ui::visibleContents(contents, mapNodesPos, tileSize, window);

        REQUIRE(visibleContents.size() == 4);
    }

    SECTION("Quarter of the map is visible")
    {
        QRect window(6, 6, 200, 200);

        std::vector<core::CampaignMap::Content> visibleContents =
            ui::visibleContents(contents, mapNodesPos, tileSize, window);

        REQUIRE(visibleContents.size() == 3);
    }

    SECTION("No nodes is visible")
    {
        QRect window(100, 100, 200, 200);

        std::vector<core::CampaignMap::Content> visibleContents =
            ui::visibleContents(contents, mapNodesPos, tileSize, window);

        REQUIRE(visibleContents.empty() == true);
    }
}

TEST_CASE("", "[mapNodeAtPos]")
{
    std::vector<core::MapNode*> mapNodes = core::generateMapNodes(2);
    core::generateMapNodeNames(mapNodes);

    core::World world;

    ui::WorldSurface surface("./worldsurface-packages/test.wsp", &world);
    surface.activate();

    const std::map<core::MapNode*, QPoint> nodesPos = ui::positionMapNodes(mapNodes.front(), surface.getTileSize());

    const core::MapNode* n;

    const int w = surface.getTileSize().width();
    const int h = surface.getTileSize().height();

    n = ui::mapNodeAtPos(QPoint(w / 2, h / 2), nodesPos, &surface);
    REQUIRE(n != nullptr);
    REQUIRE(n == mapNodes.front());

    n = ui::mapNodeAtPos(QPoint(0, 0), nodesPos, &surface);
    REQUIRE(n != nullptr);
    REQUIRE(n == mapNodes.front()->getNeighbour(core::Direction::NorthWest));

    n = ui::mapNodeAtPos(QPoint(w, h), nodesPos, &surface);
    REQUIRE(n != nullptr);
    REQUIRE(n == mapNodes.front()->getNeighbour(core::Direction::SouthEast));

    n = ui::mapNodeAtPos(QPoint(-w / 2, h / 2), nodesPos, &surface);
    REQUIRE(n != nullptr);
    REQUIRE(n == mapNodes.front()->getNeighbour(core::Direction::West));

    n = ui::mapNodeAtPos(QPoint(w * 100, h * 100), nodesPos, &surface);
    REQUIRE(n == nullptr);
}

TEST_CASE("neighboursByPos", "[MapUtil]")
{
    std::vector<core::MapNode*> mapNodes = core::generateMapNodes(2);
    core::generateMapNodeNames(mapNodes);

    core::World world;

    ui::WorldSurface worldSurface("./worldsurface-packages/test.wsp", &world);
    const QSize tileSize = worldSurface.getTileSize();

    worldSurface.activate();

    const std::map<core::MapNode*, QPoint> mapNodesPos = ui::positionMapNodes(mapNodes.front(), tileSize);

    for (const auto& mapNodePos : mapNodesPos)
    {
        const core::MapNode* mapNode = mapNodePos.first;
        const QPoint cornerPos = mapNodePos.second;

        const QPoint middlePos = cornerPos + QPoint(tileSize.width() / 2, tileSize.height() / 2);

        core::MapNodeNeighbours neighbours = ui::neighboursByPos(middlePos, &worldSurface, mapNodesPos);
        REQUIRE(neighbours == mapNode->getNeighbours());
    }
}

std::pair<float, float> getScaleFactor(const QMatrix4x4& matrix)
{
    const QVector4D firstRow = matrix.row(0);
    const QVector4D secondRow = matrix.row(1);

    return std::make_pair(firstRow.x(), secondRow.y());
}

std::pair<float, float> getTranslation(const QMatrix4x4& matrix)
{
    const QVector4D firstRow = matrix.row(0);
    const QVector4D secondRow = matrix.row(1);

    return std::make_pair(firstRow.w(), secondRow.w());
}

std::ostream& operator<<(std::ostream& s, const core::MapNodeNeighbours& n)
{
    s << "{";

    const auto end = --n.cend();
    auto it = n.cbegin();
    for (; it != end; ++it)
        s << '"' << core::direction2str(it->first) << "\": " << it->second << ", ";

    s << '"' << core::direction2str(it->first) << "\": " << it->second << "}";

    return s;
}
