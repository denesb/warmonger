#ifndef W_UTILS_MAP_GENERATOR_H
#define W_UTILS_MAP_GENERATOR_H

#include <vector>

#include "core/MapNode.h"

namespace warmonger {
namespace core {

std::vector<MapNode*> generateMapNodes(unsigned int radius);

void generateMapNodeNames(const std::vector<MapNode*> &nodes);

void generateMapNodeTerrainTypes(
        const std::vector<MapNode*>& nodes,
        const std::vector<TerrainType*>& terrainTypes
    );

} // namespace core
} // namespace warmonger

#endif // W_UTILS_MAP_GENERATOR_H
