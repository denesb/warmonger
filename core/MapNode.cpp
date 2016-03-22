#include "core/MapNode.h"

using namespace warmonger;
using namespace warmonger::core;

static const QString loggerName{"core.MapNode"};

MapNode::MapNode(QObject *parent) :
    QObject(parent),
    terrainType(nullptr)
{
}

MapNode::~MapNode()
{
}

QString MapNode::getDisplayName() const
{
    return this->displayName;
}

void MapNode::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

TerrainType * MapNode::getTerrainType() const
{
    return this->terrainType;
}

void MapNode::setTerrainType(TerrainType *terrainType)
{
    if (this->terrainType != terrainType)
    {
        this->terrainType = terrainType;
        emit terrainTypeChanged();
    }
}
