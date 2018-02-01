/**
 * Copyright (C) 2015-2018 Botond Dénes
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

#include <map>
#include <memory>

#include <catch.hpp>

#include "core/MapNode.h"
#include "core/World.h"

using namespace warmonger;

TEST_CASE("MapNode neighbours", "[MapNodeNeighbours]")
{
    core::World w("uuid0", core::WorldRules::Type::Lua);
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
        core::MapNode mn(&w);

        neighbours[core::Direction::West] = &mn;

        REQUIRE(neighbours[core::Direction::West] == &mn);
        REQUIRE(neighbours.at(core::Direction::West) == &mn);
    }

    SECTION("Not equal to if a neighbour is changed")
    {
        core::MapNodeNeighbours neighbours1;
        core::MapNode mn(&w);

        neighbours1[core::Direction::West] = &mn;

        REQUIRE(neighbours != neighbours1);
    }

    SECTION("Initializing to with incomplete number of neighbour yields complete neighbour map")
    {
        core::MapNode mn(&w);
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
