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
    Q_PROPERTY(QVariant terrainType READ readTerrainType)
    Q_PROPERTY(QVariantMap neighbours READ readNeighbours);

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
    static const QHash<QString, Direction> str2direction;
    static const QHash<Direction, QString> direction2str;

    MapNode(QObject *parent);
    ~MapNode();

    const TerrainType * getTerrainType() const;
    void setTerrainType(const TerrainType *terrainType);
    QVariant readTerrainType() const;

    const MapNode * getNeighbour(Direction direction) const;
    const QHash<Direction, const MapNode *> getNeighbours() const;
    void setNeighbour(Direction direction, const MapNode *neighbour);
    void setNeighbours(const QHash<Direction, const MapNode *> &neighbours);
    QVariantMap readNeighbours() const;

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    const TerrainType *terrainType;
    QHash<Direction, const MapNode *> neighbours;
};

} // namespace core
} // namespace warmonger

#endif // CORE_MAP_TILE_H
