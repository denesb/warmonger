#ifndef CORE_MAP_TILE_H
#define CORE_MAP_TILE_H

#include <QObject>
#include <QJsonObject>
#include <QPoint>
#include <QMap>

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

    MapTile(QObject *parent = nullptr);
    ~MapTile();

    TerrainType *getTerrainType() const;
    void setTerrainType(TerrainType *terrainType);

    QPoint getPosition() const;
    void setPosition(const QPoint &position);

    MapTile * getNeighbour(Direction direction) const;
    void setNeighbour(Direction direction, MapTile *mapTile);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    TerrainType *terrainType;
    QPoint position;
    QMap<Direction, MapTile *> neighbours;
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_MAP_TILE_H
