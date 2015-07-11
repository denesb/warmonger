#ifndef CORE_MAP_TILE_H
#define CORE_MAP_TILE_H

#include <QObject>
#include <QJsonObject>
#include <QMap>

#include "core/MapPosition.h"

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
class MapTile :
    public QObject
{
    Q_OBJECT

public:
    enum Direction
    {
        North,
        NorthEast,
        SouthEast,
        South,
        SouthWest,
        NorthWest
    };

    MapTile(QObject *parent);
    ~MapTile();

    const TerrainType *getTerrainType() const;
    void setTerrainType(const TerrainType *terrainType);

    MapPosition getPosition() const;
    void setPosition(const MapPosition &position);

    const MapTile * getNeighbour(Direction direction) const;
    void setNeighbour(Direction direction, const MapTile *mapTile);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    const TerrainType *terrainType;
    MapPosition position;
    QMap<Direction, const MapTile *> neighbours;
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_MAP_TILE_H
