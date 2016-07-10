#ifndef W_UTILS_MAP_GENERATOR_H
#define W_UTILS_MAP_GENERATOR_H

#include <vector>

#include "core/MapNode.h"
#include "core/World.h"

namespace warmonger {
namespace utils {

std::vector<core::MapNode *> generateNodes(const core::World *world, unsigned int radius);

} // namespace utils
} // namespace warmonger

#endif // W_UTILS_MAP_GENERATOR_H
