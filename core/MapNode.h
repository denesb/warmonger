#ifndef W_CORE_MAP_NODE_H
#define W_CORE_MAP_NODE_H

#include <map>

#include "core/Hexagon.h"
#include "core/MapNodeNeighbours.h"
#include "core/TerrainType.h"

namespace warmonger {
namespace core {

class MapNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(TerrainType* terrainType READ getTerrainType WRITE setTerrainType NOTIFY terrainTypeChanged)

public:
    explicit MapNode(QObject* parent = nullptr);

    const QString& getDisplayName() const
    {
        return this->displayName;
    }

    void setDisplayName(const QString& displayName);

    TerrainType* getTerrainType() const
    {
        return this->terrainType;
    }

    void setTerrainType(TerrainType* terrainType);

    const MapNodeNeighbours& getNeighbours() const
    {
        return this->neighbours;
    }

    void setNeighbours(const MapNodeNeighbours& neighbours);
    void setNeighbours(MapNodeNeighbours&& neighbours);

    MapNode* getNeighbour(Direction direction) const
    {
        return this->neighbours.at(direction);
    }

    void setNeighbour(Direction direction, MapNode* mapNode);

signals:
    void displayNameChanged();
    void terrainTypeChanged();
    void neighboursChanged();

private:
    QString displayName;

    TerrainType* terrainType;
    MapNodeNeighbours neighbours;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_MAP_NODE_H
