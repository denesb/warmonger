#ifndef TERRAIN_TYPE_WORLD_H
#define TERRAIN_TYPE_WORLD_H

#include "core/WorldItem.h"

namespace warmonger {
namespace core {

class TerrainType :
    public WorldItem
{
    Q_OBJECT

public:
    TerrainType(QObject *parent = nullptr);
    ~TerrainType();
};

}; // namespace core
}; // namespace warmonger

#endif // TERRAIN_TYPE_WORLD_H
