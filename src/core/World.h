#include "core/WorldMeta.h"

#ifndef CORE_WORLD_H
#define CORE_WORLD_H

namespace core {

class World :
    public QObject
{
    Q_OBJECT

public:
    World(WorldMeta * const worldMeta, QObject *parent = nullptr);

private:
    WorldMeta *meta;
};

}; // namespace core

#endif // CORE_WORLD_H
