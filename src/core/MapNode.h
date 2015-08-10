#ifndef CORE_MAP_NODE_H
#define CORE_MAP_NODE_H

#include <QObject>
#include <QJsonObject>
#include <QHash>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class TerrainType;

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
    Q_PROPERTY(QVariant terrainType READ readTerrainType NOTIFY terrainTypeChanged)
    Q_PROPERTY(QVariantMap neighbours READ readNeighbours NOTIFY neighboursChanged);

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

    MapNode(QObject *parent);
    ~MapNode();

    const TerrainType * getTerrainType() const;
    void setTerrainType(const TerrainType *terrainType);
    QVariant readTerrainType() const;

    const MapNode * getNeighbour(Direction direction) const;
    const QHash<Direction, const MapNode *> getNeighbours() const;
    void setNeighbour(Direction direction, const MapNode *neighbour);
    QVariantMap readNeighbours() const;

    Q_INVOKABLE QString oppositeDirection(QString directionStr) const;

signals:
    void terrainTypeChanged();
    void neighboursChanged();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    const TerrainType *terrainType;
    QHash<Direction, const MapNode *> neighbours;
};

} // namespace core
} // namespace warmonger

#endif // CORE_MAP_NODE_H
