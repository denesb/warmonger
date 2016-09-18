#include <map>
#include <memory>

#include "core/MapNode.h"
#include "core/MapNodeNeighbours.h"
#include "test/catch.hpp"

using namespace warmonger;

TEST_CASE("MapNode neighbours", "[MapNodeNeighbours]")
{
    core::MapNodeNeighbours neighbours;

    SECTION("MapNode has a full neighbour map after construction")
    {
        unsigned s{0};
        for (const auto& n : neighbours)
        {
            REQUIRE(n.second == nullptr);
            ++s;
        }
        REQUIRE(s == core::directions.size());
    }

    SECTION("Equal to other default constructed one")
    {
        core::MapNodeNeighbours neighbours1;

        REQUIRE(neighbours == neighbours1);
    }

    SECTION("Setter changes neighbour")
    {
        core::MapNode mn;

        neighbours[core::Direction::West] = &mn;

        REQUIRE(neighbours[core::Direction::West] == &mn);
        REQUIRE(neighbours.at(core::Direction::West) == &mn);
    }

    SECTION("Not equal to if a neighbour is changed")
    {
        core::MapNodeNeighbours neighbours1;
        core::MapNode mn;

        neighbours1[core::Direction::West] = &mn;

        REQUIRE(neighbours != neighbours1);
    }

    SECTION("Initializing to with incomplete number of neighbour yields complete neighbour map")
    {
        core::MapNode mn;
        core::MapNodeNeighbours neighbours1{{core::Direction::NorthWest, &mn}};

        REQUIRE(neighbours1[core::Direction::NorthWest] == &mn);
        REQUIRE(neighbours1.at(core::Direction::NorthWest) == &mn);

        unsigned s{0};
        for (const auto& n : neighbours1)
        {
            if (n.first == core::Direction::NorthWest)
            {
                REQUIRE(n.second == &mn);
            }
            else
            {
                REQUIRE(n.second == nullptr);
            }
            ++s;
        }
        REQUIRE(s == core::directions.size());
    }
}
