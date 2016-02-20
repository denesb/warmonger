#ifndef CORE_MAP_NODE_H
#define CORE_MAP_NODE_H

#include <QObject>
#include <QJsonObject>
#include <QHash>

#include "core/GameObject.h"
#include "core/TerrainType.h"

namespace warmonger {
namespace core {

/*
 *
 *  NW  /\  NE
 *    /    \
 * W |      | E
 *   |      |
 *    \    /
 *  SW  \/  SE
 *
 */
class MapNode :
    public GameObject
{
    Q_OBJECT
    Q_PROPERTY(TerrainType *terrainType READ getTerrainType WRITE setTerrainType NOTIFY terrainTypeChanged)
    Q_PROPERTY(QVariantMap neighbours READ readNeighbours WRITE writeNeighbours NOTIFY neighboursChanged)

public:
    enum Direction
    {
        West = 0,
        NorthWest = 1,
        NorthEast = 2,
        East = 3,
        SouthEast = 4,
        SouthWest = 5
    };
    static const QHash<QString, Direction> str2direction;
    static const QHash<Direction, QString> direction2str;
    static const QHash<Direction, Direction> oppositeDirections;

    MapNode(QObject *parent=nullptr);
    ~MapNode();

    TerrainType * getTerrainType() const;
    void setTerrainType(TerrainType *terrainType);

    MapNode * getNeighbour(Direction direction) const;
    QHash<Direction, MapNode *> getNeighbours() const;
    void setNeighbour(Direction direction, MapNode *neighbour);
    void setNeighbours(const QHash<Direction, MapNode *> &neighbours);
    QVariantMap readNeighbours() const;
    void writeNeighbours(QVariantMap neighbours);

signals:
    void terrainTypeChanged();
    void neighboursChanged();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    void addNeighbour(Direction direction, MapNode *neighbour);
    void removeNeighbour(Direction direction);

    TerrainType *terrainType;
    QHash<Direction, MapNode *> neighbours;
};

} // namespace core
} // namespace warmonger

#endif // CORE_MAP_NODE_H
