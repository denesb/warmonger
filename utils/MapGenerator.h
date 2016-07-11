#ifndef W_UTILS_MAP_GENERATOR_H
#define W_UTILS_MAP_GENERATOR_H

#include <vector>

#include "core/MapNode.h"

namespace warmonger {
namespace utils {

std::vector<core::MapNode *> generateNodes(unsigned int radius);

void generateNodeNames(const std::vector<core::MapNode *> &nodes);

void generateNodeTerrainTypes(
        const std::vector<core::MapNode *> &nodes,
        const std::vector<core::TerrainType *> &terrainTypes
    );

} // namespace utils
} // namespace warmonger

#endif // W_UTILS_MAP_GENERATOR_H