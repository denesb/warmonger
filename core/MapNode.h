#ifndef CORE_MAP_NODE_H
#define CORE_MAP_NODE_H

#include <map>

#include <QObject>

#include "core/TerrainType.h"

namespace warmonger {
namespace core {

/*
 *  NW  /\  NE
 *    /    \
 * W |      | E
 *   |      |
 *    \    /
 *  SW  \/  SE
 */
enum class Direction
{
    West,
    NorthWest,
    NorthEast,
    East,
    SouthEast,
    SouthWest
};

/**
 * Converts Directions to QString.
 */
QString direction2str(Direction d);

/**
 * Converts QString to Direction.
 *
 * Will throw a ValueError if str is not a valid direction name.
 */
Direction str2direction(const QString &str);

/**
 * Return the opposite direction of d.
 */
Direction oppositeDirection(Direction d);

class MapNode :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(TerrainType *terrainType READ getTerrainType WRITE setTerrainType NOTIFY terrainTypeChanged)

public:
    explicit MapNode(QObject *parent=nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    TerrainType * getTerrainType() const;
    void setTerrainType(TerrainType *terrainType);

    std::map<Direction, MapNode *> getNeighbours() const;
    void setNeighbours(const std::map<Direction, MapNode *> &neighbours);
    void setNeighbours(std::map<Direction, MapNode *> &&neighbours);

    MapNode * getNeighbour(Direction direction) const;
    void setNeighbour(Direction direction, MapNode *mapNode);

signals:
    void displayNameChanged();
    void terrainTypeChanged();
    void neighboursChanged();

private:
    QString displayName;

    TerrainType *terrainType;
    std::map<Direction, MapNode *> neighbours;
};

} // namespace core
} // namespace warmonger

#endif // CORE_MAP_NODE_H
