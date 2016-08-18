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
        REQUIRE(s == utils::directions.size());
    }

    SECTION("Equal to other default constructed one")
    {
        core::MapNodeNeighbours neighbours1;

        REQUIRE(neighbours == neighbours1);
    }

    SECTION("Setter changes neighbour")
    {
        core::MapNode mn;

        neighbours[utils::Direction::West] = &mn;

        REQUIRE(neighbours[utils::Direction::West] == &mn);
        REQUIRE(neighbours.at(utils::Direction::West) == &mn);
    }

    SECTION("Not equal to if a neighbour is changed")
    {
        core::MapNodeNeighbours neighbours1;
        core::MapNode mn;

        neighbours1[utils::Direction::West] = &mn;

        REQUIRE(neighbours != neighbours1);
    }

    SECTION("Initializing to with incomplete number of neighbour yields complete neighbour map")
    {
        core::MapNode mn;
        core::MapNodeNeighbours neighbours1{{utils::Direction::NorthWest, &mn}};

        REQUIRE(neighbours1[utils::Direction::NorthWest] == &mn);
        REQUIRE(neighbours1.at(utils::Direction::NorthWest) == &mn);

        unsigned s{0};
        for (const auto& n : neighbours1)
        {
            if(n.first == utils::Direction::NorthWest)
            {
                REQUIRE(n.second == &mn);
            }
            else
            {
                REQUIRE(n.second == nullptr);
            }
            ++s;
        }
        REQUIRE(s == utils::directions.size());
    }
}
