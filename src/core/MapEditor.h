#ifndef CORE_MAP_EDITOR_H
#define CORE_MAP_EDITOR_H

#include <QObject>

#include "core/MapNode.h"

namespace warmonger {
namespace core {

class Map;

class MapEditor :
    public QObject
{
    Q_OBJECT

public:
    MapEditor(Map *map, QObject *parent);
    ~MapEditor();

    void createMapNode(const TerrainType *terrainType, const QHash<MapNode::Direction, const MapNode *> &neighbours);
    Q_INVOKABLE void createMapNode(QObject *terrainType, QVariant neighbours);

    Q_INVOKABLE void changeMapNodeTerrainType(QObject *mapNode, QObject *newTerrainType);

private:
    Map *map;
};

} // namespace core
} // namespace warmonger

#endif // CORE_MAP_EDITOR_H
