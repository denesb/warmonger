#include "test/catch.hpp"

#include <iostream>

#include "utils/MapGenerator.h"

using namespace warmonger;

unsigned int numberOfConnections(const std::vector<core::MapNode *> &nodes);

TEST_CASE("Generate nodes with different radiuses", "[MapGenerator]")
{
    SECTION("radius == 0")
    {
        const std::vector<core::MapNode *> nodes = utils::generateNodes(0);

        REQUIRE(nodes.size() == 0);
    }

    SECTION("radius == 1")
    {
        const std::vector<core::MapNode *> nodes = utils::generateNodes(1);

        REQUIRE(nodes.size() == 1);
    }

    SECTION("radius == 2")
    {
        const std::vector<core::MapNode *> nodes = utils::generateNodes(2);

        REQUIRE(nodes.size() == 7);
        REQUIRE(numberOfConnections(nodes) == 24);
    }

    SECTION("radius == 3")
    {
        const std::vector<core::MapNode *> nodes = utils::generateNodes(3);

        REQUIRE(nodes.size() == 19);
        REQUIRE(numberOfConnections(nodes) == 84);
    }

    SECTION("radius == 4")
    {
        const std::vector<core::MapNode *> nodes = utils::generateNodes(4);

        REQUIRE(nodes.size() == 37);
        REQUIRE(numberOfConnections(nodes) == 180);
    }
}

unsigned int numberOfConnections(const std::vector<core::MapNode *> &nodes)
{
    unsigned int n{0};

    for (const core::MapNode *node : nodes)
    {
        for (utils::Direction direction : utils::directions)
        {
            if (node->getNeighbour(direction))
                ++n;
        }
    }

    return n;
}
