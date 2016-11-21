/** \file
 * Generic utility functions and function objects.
 *
 * \copyright (C) 2015-2016 Botond Dénes
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

#ifndef W_CORE_UTIL_HPP
#define W_CORE_UTIL_HPP

#include "core/MapNode.h"
#include "core/CampaignMap.h"

namespace warmonger {
namespace core {

template <typename T>
struct IsOnMapNode
{
    explicit constexpr IsOnMapNode(const MapNode* mapNode) : mapNode(mapNode)
    {
    }

    bool operator()(const T* obj)
    {
        return obj->getMapNode() == this->mapNode;
    }

    const MapNode* mapNode;

};

struct HasMapNode
{
    explicit constexpr HasMapNode(const MapNode* mapNode) : mapNode(mapNode)
    {
    }

    bool operator()(const CampaignMap::Content& content)
    {
        return std::get<MapNode*>(content) == this->mapNode;
    }

    const MapNode* mapNode;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_UTIL_HPP
