#include "core/World.h"

using namespace core;

World::World(WorldMeta * const worldMeta, QObject *parent) :
    QObject(parent),
    meta(worldMeta)
{
}
