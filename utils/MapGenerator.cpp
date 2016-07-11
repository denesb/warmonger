#include <algorithm>
#include <iterator>
#include <utility>

#include "utils/MapGenerator.h"

namespace warmonger {
namespace utils {

namespace {

void addNodeRing(std::vector<core::MapNode *>& nodes);
core::MapNode * createNeighbour(core::MapNode *node, const utils::Direction direction);
void connectWithCommonNeighbour(
        core::MapNode *n1,
        core::MapNode *n2,
        const utils::Direction dn1n2,
        const utils::Direction dn1n3
    );

}

std::vector<core::MapNode *> generateNodes(unsigned int radius)
{
    std::vector<core::MapNode *> nodes;

    if (radius == 0)
    {
        return nodes;
    }

    core::MapNode *mapNode = new core::MapNode();
    nodes.push_back(mapNode);

    for (unsigned i = 1; i < radius; ++i)
    {
        addNodeRing(nodes);
    }

    return nodes;
}

namespace {

void addNodeRing(std::vector<core::MapNode *>& nodes)
{
    std::vector<core::MapNode *> newNodes;

    for (core::MapNode *node : nodes)
    {
        for (utils::Direction direction : utils::directions)
        {
            if (node->getNeighbour(direction) == nullptr)
                newNodes.push_back(createNeighbour(node, direction));
        }
    }

    std::copy(newNodes.begin(), newNodes.end(), std::back_inserter(nodes));
}

core::MapNode * createNeighbour(core::MapNode *node, const utils::Direction direction)
{
    core::MapNode *newNode = new core::MapNode();

    node->setNeighbour(direction, newNode);
    newNode->setNeighbour(utils::oppositeDirection(direction), node);

    const std::pair<utils::Direction, utils::Direction> axis = utils::neighbourDirections(direction);

    connectWithCommonNeighbour(node, newNode, direction, std::get<0>(axis));
    connectWithCommonNeighbour(node, newNode, direction, std::get<1>(axis));

    return newNode;
}

void connectWithCommonNeighbour(
        core::MapNode *n1,
        core::MapNode *n2,
        const utils::Direction dn1n2,
        const utils::Direction dn1n3
    )
{
    core::MapNode *n3 = n1->getNeighbour(dn1n3);

    if (n3 == nullptr)
        return;

    std::pair<utils::Direction, utils::Direction> connectionAxis = utils::connectingDirections(dn1n2, dn1n3);

    n2->setNeighbour(std::get<0>(connectionAxis), n3);
    n3->setNeighbour(std::get<1>(connectionAxis), n2);
}

}

} // namespace utils
} // namespace warmonger
