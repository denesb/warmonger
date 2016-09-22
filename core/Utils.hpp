#ifndef W_CORE_UTIL_HPP
#define W_CORE_UTIL_HPP

#include "core/MapNode.h"

namespace warmonger {
namespace core {

template <typename T>
struct IsOnMapNode
{
    IsOnMapNode(const MapNode* mapNode) : mapNode(mapNode)
    {
    }

    bool operator()(const T* obj)
    {
        return obj->getMapNode() == mapNode;
    }

    const MapNode* mapNode;

};

} // namespace core
} // namespace warmonger

#endif // W_CORE_UTIL_HPP
