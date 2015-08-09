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

void MapEditor::createMapNode(const TerrainType *terrainType, const QHash<MapNode::Direction, const MapNode *> &neighbours)
{
    if (neighbours.empty())
    {
        Exception e(Exception::InvalidValue, {"empty QMap", "neighbours"});
        wError("core.MapEditor") << e.getMessage();
        throw e;
    }

    MapNode *newMapNode = new MapNode(this->map);
    newMapNode->setTerrainType(terrainType);

    QHash<MapNode::Direction, const MapNode *>::ConstIterator it;
    for (it = neighbours.constBegin(); it != neighbours.constEnd(); it++)
    {
        newMapNode->setNeighbour(it.key(), it.value());
    }
}

void MapEditor::createMapNode(QObject *terrainType, QVariant neighbours)
{
    TerrainType *tt = qobject_cast<TerrainType *>(terrainType);
    if (tt == nullptr)
    {
        Exception e(Exception::NullPointer, {"terrainType"});
        wError("core.MapEditor") << e.getMessage();
        throw e;
    }

    if (!neighbours.canConvert(QMetaType::QVariantMap))
    {
        Exception e(Exception::WrongType, {"neighbours", "QVariantMap", "?"});
        wError("core.MapEditor") << e.getMessage();
        throw e;
    }
    QVariantMap neighboursMap = neighbours.toMap();
    QHash<MapNode::Direction, const MapNode *> nm;

    QVariantMap::ConstIterator it;
    for (it = neighboursMap.constBegin(); it != neighboursMap.constEnd(); it++)
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
            Exception e(Exception::WrongType, {"mapNode", "MapNode *", "?"});
            wError("core.MapEditor") << e.getMessage();
            throw e;
        }

        MapNode *neighbour = v.value<MapNode *>();
        nm.insert(direction, neighbour);
    }

    this->createMapNode(tt, nm);
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
