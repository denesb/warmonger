#ifndef CORE_MAP_TILE_H
#define CORE_MAP_TILE_H

#include <QObject>
#include <QJsonObject>
#include <QHash>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class TerrainType;

/*      N
 *     ____
 * NW /    \ NE
 *   /      \
 *   \      /
 * SW \____/ SE
 *      S
 */
class MapNode :
    public GameObject
{
    Q_OBJECT

public:
    enum Direction
    {
        North = 0,
        NorthEast = 1,
        SouthEast = 2,
        South = 3,
        SouthWest = 4,
        NorthWest = 5
    };

    MapNode(QObject *parent);
    ~MapNode();

    const TerrainType * getTerrainType() const;
    void setTerrainType(const TerrainType *terrainType);

    const MapNode * getNeighbour(Direction direction) const;
    const QList<const MapNode *> getNeighbours() const;
    void setNeighbour(Direction direction, const MapNode *neighbour);
    void setNeighbours(const QList<const MapNode *> &neighbours);

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    const TerrainType *terrainType;
    QList<const MapNode *> neighbours;
};

} // namespace core
} // namespace warmonger

#endif // CORE_MAP_TILE_H
