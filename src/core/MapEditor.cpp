#include "core/Map.h"
#include "core/MapEditor.h"
#include "core/TerrainType.h"
#include "core/MapNode.h"

using namespace warmonger::core;

MapEditor::MapEditor(Map *map, QObject *parent) :
    QObject(parent),
    map(map)
{
}

MapEditor::~MapEditor()
{
}

void MapEditor::createMapNode(QObject *terrainType, QVariantMap &neighbours)
{
    MapNode *newMapNode = new MapNode(this->map);

    TerrainType *tt = qobject_cast<TerrainType *>(terrainType);
    if (tt == nullptr)
    {
        Exception e(Exception::NullPointer);
        wError("core.MapEditor") << e.getMessage() << " terrainType is null or has wrong type";
        throw e;
    }
    newMapNode->setTerrainType(tt);

    QVariantMap::ConstIterator it;
    for (it = neighbours.constBegin(); it != neighbours.constEnd(); it++)
    {
        QString directionName = it.key();
        if (!MapNode::str2direction.contains(directionName))
        {
            Exception e(Exception::InvalidValue, {directionName, "MapNode::Direction"});
            wError("core.MapEditor") << e.getMessage();
            throw e;
        }
        MapNode::Direction direction = MapNode::str2direction[directionName];
        
        QVariant v = it.value();

        if (!v.canConvert<MapNode *>())
        {
            Exception e(Exception::WrongType);
            wError("core.MapEditor") << e.getMessage();
            throw e;
        }

        MapNode *neighbour = v.value<MapNode *>();
        newMapNode->setNeighbour(direction, neighbour);
    }
}

void MapEditor::changeMapNodeTerrainType(QObject *mapNode, QObject *newTerrainType)
{
    MapNode *mn = qobject_cast<MapNode *>(mapNode);
    if (mn == nullptr)
    {
        Exception e(Exception::NullPointer);
        wError("core.MapEditor") << e.getMessage() << " mapNode is null or has wrong type";
        throw e;
    }

    TerrainType *tt = qobject_cast<TerrainType *>(newTerrainType);
    if (tt == nullptr)
    {
        Exception e(Exception::NullPointer);
        wError("core.MapEditor") << e.getMessage() << " terrainType is null or has wrong type";
        throw e;
    }

    mn->setTerrainType(tt);
}
