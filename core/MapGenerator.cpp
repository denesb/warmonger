#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iterator>
#include <utility>

#include "core/MapGenerator.h"

namespace warmonger {
namespace core {

namespace {

void addMapNodeRing(std::vector<MapNode*> &nodes);
MapNode* createNeighbour(MapNode* node, const Direction direction);
void connectWithCommonNeighbour(
        MapNode* n1,
        MapNode* n2,
        const Direction dn1n2,
        const Direction dn1n3
    );

}

std::vector<MapNode*> generateMapNodes(unsigned int radius)
{
    std::vector<MapNode*> nodes;

    if (radius == 0)
    {
        return nodes;
    }

    MapNode* mapNode = new MapNode();
    nodes.push_back(mapNode);

    for (unsigned i = 1; i < radius; ++i)
    {
        addMapNodeRing(nodes);
    }

    return nodes;
}

void generateMapNodeNames(const std::vector<MapNode*>& nodes)
{
    for (std::size_t i = 0; i < nodes.size(); ++i)
    {
        nodes[i]->setObjectName("mapNode" + QString::number(i));
    }
}

void generateMapNodeTerrainTypes(
        const std::vector<MapNode*>& nodes,
        const std::vector<TerrainType*>& terrainTypes
    )
{
    if (terrainTypes.empty())
        return;

    std::srand(std::time(0));
    const std::size_t range = terrainTypes.size();

    for (MapNode* node : nodes)
    {
        const std::size_t index = std::rand() % range;
        node->setTerrainType(terrainTypes[index]);
    }
}

namespace {

void addMapNodeRing(std::vector<MapNode*> &nodes)
{
    std::vector<MapNode*> newNodes;

    for (MapNode* node : nodes)
    {
        for (Direction direction : directions)
        {
            if (node->getNeighbour(direction) == nullptr)
                newNodes.push_back(createNeighbour(node, direction));
        }
    }

    std::copy(newNodes.begin(), newNodes.end(), std::back_inserter(nodes));
}

MapNode* createNeighbour(MapNode* node, const Direction direction)
{
    MapNode* newNode = new MapNode();

    node->setNeighbour(direction, newNode);
    newNode->setNeighbour(oppositeDirection(direction), node);

    const std::pair<Direction, Direction> axis = neighbourDirections(direction);

    connectWithCommonNeighbour(node, newNode, direction, std::get<0>(axis));
    connectWithCommonNeighbour(node, newNode, direction, std::get<1>(axis));

    return newNode;
}

void connectWithCommonNeighbour(
        MapNode* n1,
        MapNode* n2,
        const Direction dn1n2,
        const Direction dn1n3
    )
{
    MapNode* n3 = n1->getNeighbour(dn1n3);

    if (n3 == nullptr)
        return;

    std::pair<Direction, Direction> connectionAxis = connectingDirections(dn1n2, dn1n3);

    n2->setNeighbour(std::get<0>(connectionAxis), n3);
    n3->setNeighbour(std::get<1>(connectionAxis), n2);
}

}

} // namespace core
} // namespace warmonger
