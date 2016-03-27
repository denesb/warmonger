#include <map>
#include <memory>

#include "core/MapNode.h"
#include "test/catch.hpp"

using namespace warmonger;

TEST_CASE("MapNode neighbours", "[MapNode]")
{
    SECTION("MapNode has a full neighbour map after construction")
    {
        std::unique_ptr<core::MapNode> mn(new core::MapNode);

        REQUIRE(mn->getNeighbours().size() == 6);

        for (const auto& n : mn->getNeighbours())
        {
            REQUIRE(n.second == nullptr);
        }
    }

    SECTION("Full neighbor map is unchanged")
    {
        std::unique_ptr<core::MapNode> mn(new core::MapNode);

        const std::map<core::Direction, core::MapNode *> ns{
            {core::Direction::West, new core::MapNode(mn.get())},
            {core::Direction::NorthWest, new core::MapNode(mn.get())},
            {core::Direction::NorthEast, new core::MapNode(mn.get())},
            {core::Direction::East, new core::MapNode(mn.get())},
            {core::Direction::SouthEast, new core::MapNode(mn.get())},
            {core::Direction::SouthWest, new core::MapNode(mn.get())}
        };
        mn->setNeighbours(ns);

        REQUIRE(ns == mn->getNeighbours());
    }

    SECTION("Incomplete neighbor map is filled")
    {
        std::unique_ptr<core::MapNode> mn(new core::MapNode);

        const std::map<core::Direction, core::MapNode *> ns{
            {core::Direction::West, new core::MapNode(mn.get())},
            {core::Direction::East, new core::MapNode(mn.get())},
            {core::Direction::SouthEast, new core::MapNode(mn.get())},
            {core::Direction::SouthWest, new core::MapNode(mn.get())}
        };
        mn->setNeighbours(ns);

        REQUIRE(mn->getNeighbours().size() == 6);

        for (const auto& n : mn->getNeighbours())
        {
            if (ns.find(n.first) == ns.end())
            {
                REQUIRE(n.second == nullptr);
            }
            else
            {
                REQUIRE(n.second == ns.at(n.first));
            }
        }
    }

    SECTION("Setting a neighbour changed only that neighbour")
    {
        std::unique_ptr<core::MapNode> mn(new core::MapNode);

        const std::map<core::Direction, core::MapNode *> ns{
            {core::Direction::West, new core::MapNode(mn.get())},
            {core::Direction::NorthWest, new core::MapNode(mn.get())},
            {core::Direction::NorthEast, new core::MapNode(mn.get())},
            {core::Direction::East, new core::MapNode(mn.get())},
            {core::Direction::SouthEast, new core::MapNode(mn.get())},
            {core::Direction::SouthWest, new core::MapNode(mn.get())}
        };
        mn->setNeighbours(ns);

        REQUIRE(mn->getNeighbours().size() == 6);
        REQUIRE(ns == mn->getNeighbours());

        core::MapNode *nn = new core::MapNode(mn.get());
        mn->setNeighbour(core::Direction::West, nn);

        REQUIRE(mn->getNeighbours().at(core::Direction::West) == nn);

        for (const auto& n : mn->getNeighbours())
        {
            if (n.first != core::Direction::West)
            {
                REQUIRE(n.second == ns.at(n.first));
            }
        }
    }
}
