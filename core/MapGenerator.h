/** \file
 * Map-generation related functions.
 *
 * \copyright (C) 2015-2017 Botond Dénes
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

#ifndef W_UTILS_MAP_GENERATOR_H
#define W_UTILS_MAP_GENERATOR_H

#include <vector>

#include "core/MapNode.h"

namespace warmonger {
namespace core {

std::vector<MapNode*> generateMapNodes(unsigned int radius);

void generateMapNodeNames(const std::vector<MapNode*>& nodes);

void generateMapNodeTerrainTypes(const std::vector<MapNode*>& nodes, const std::vector<TerrainType*>& terrainTypes);

} // namespace core
} // namespace warmonger

#endif // W_UTILS_MAP_GENERATOR_H
