/** \file
 * Generic utility functions and function objects.
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
