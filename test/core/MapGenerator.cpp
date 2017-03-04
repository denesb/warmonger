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

#include "test/catch.hpp"

#include "core/MapGenerator.h"

using namespace warmonger;

unsigned int numberOfConnections(const std::vector<core::MapNode*>& nodes);

TEST_CASE("generateMapNodes", "[MapGenerator]")
{
    SECTION("radius == 0")
    {
        const std::vector<core::MapNode*> nodes = core::generateMapNodes(0);

        REQUIRE(nodes.size() == 0);
    }

    SECTION("radius == 1")
    {
        const std::vector<core::MapNode*> nodes = core::generateMapNodes(1);

        REQUIRE(nodes.size() == 1);
    }

    SECTION("radius == 2")
    {
        const std::vector<core::MapNode*> nodes = core::generateMapNodes(2);

        REQUIRE(nodes.size() == 7);
        REQUIRE(numberOfConnections(nodes) == 24);
    }

    SECTION("radius == 3")
    {
        const std::vector<core::MapNode*> nodes = core::generateMapNodes(3);

        REQUIRE(nodes.size() == 19);
        REQUIRE(numberOfConnections(nodes) == 84);
    }

    SECTION("radius == 4")
    {
        const std::vector<core::MapNode*> nodes = core::generateMapNodes(4);

        REQUIRE(nodes.size() == 37);
        REQUIRE(numberOfConnections(nodes) == 180);
    }
}

unsigned int numberOfConnections(const std::vector<core::MapNode*>& nodes)
{
    unsigned int n{0};

    for (const core::MapNode* node : nodes)
    {
        for (core::Direction direction : core::directions)
        {
            if (node->getNeighbour(direction))
                ++n;
        }
    }

    return n;
}
