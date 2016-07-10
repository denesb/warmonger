#include "utils/MapGenerator.h"

namespace warmonger {
namespace utils {

std::vector<core::MapNode *> generateNodes(const core::World *world, unsigned int radius)
{
    const std::vector<core::TerrainType *> terrainTypes = world->getTerrainTypes();
    std::vector<core::MapNode *> nodes;

    if (terrainTypes.empty())
    {
        return nodes;
    }

    for (unsigned i = 0; i < radius; ++i)
    {
    }

    core::MapNode *mapNode = new core::MapNode();
    mapNode->setTerrainType(terrainTypes.front());

    return nodes;
}

} // namespace utils
} // namespace warmonger
