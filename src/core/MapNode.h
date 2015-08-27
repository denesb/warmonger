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
    Q_PROPERTY(QObject * terrainType READ readTerrainType WRITE writeTerrainType NOTIFY terrainTypeChanged)
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

    MapNode(QObject *parent);
    ~MapNode();

    TerrainType * getTerrainType() const;
    void setTerrainType(TerrainType *terrainType);
    QObject * readTerrainType() const;
    void writeTerrainType(QObject *terrainType);

    MapNode * getNeighbour(Direction direction) const;
    QHash<Direction, MapNode *> getNeighbours() const;
    void setNeighbour(Direction direction, MapNode *neighbour);
    void setNeighbours(const QHash<Direction, MapNode *> &neighbours);
    Q_INVOKABLE QObject * getNeighbour(QString directionName) const;
    Q_INVOKABLE void setNeighbour(QString directionName, QObject *neighbour);
    QVariantMap readNeighbours() const;
    void writeNeighbours(QVariantMap neighbours);

    Q_INVOKABLE QString oppositeDirection(QString directionStr) const;

signals:
    void terrainTypeChanged();
    void neighboursChanged();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    TerrainType *terrainType;
    QHash<Direction, MapNode *> neighbours;
};

} // namespace core
} // namespace warmonger

#endif // CORE_MAP_NODE_H
