#include "core/JsonUtil.h"
#include "core/Map.h"
#include "core/MapNode.h"
#include "core/QVariantUtil.h"
#include "core/World.h"

using namespace warmonger;
using namespace warmonger::core;

static const QString loggerName{"core.MapNode"};

MapNode::Direction convertStr2direction(const QString &str);

const QHash<MapNode::Direction, QString> MapNode::direction2str{
    std::make_pair(MapNode::West, "West"),
    std::make_pair(MapNode::NorthWest, "NorthWest"),
    std::make_pair(MapNode::NorthEast, "NorthEast"),
    std::make_pair(MapNode::East, "East"),
    std::make_pair(MapNode::SouthEast, "SouthEast"),
    std::make_pair(MapNode::SouthWest, "SouthWest")
};

const QHash<QString, MapNode::Direction> MapNode::str2direction{
    std::make_pair("West", MapNode::West),
    std::make_pair("NorthWest", MapNode::NorthWest),
    std::make_pair("NorthEast", MapNode::NorthEast),
    std::make_pair("East", MapNode::East),
    std::make_pair("SouthEast", MapNode::SouthEast),
    std::make_pair("SouthWest", MapNode::SouthWest)
};

const QHash<MapNode::Direction, MapNode::Direction> MapNode::oppositeDirections{
    std::make_pair(MapNode::West, MapNode::East),
    std::make_pair(MapNode::NorthWest, MapNode::SouthEast),
    std::make_pair(MapNode::NorthEast, MapNode::SouthWest),
    std::make_pair(MapNode::East, MapNode::West),
    std::make_pair(MapNode::SouthEast, MapNode::NorthWest),
    std::make_pair(MapNode::SouthWest, MapNode::NorthEast)
};

MapNode::MapNode(QObject *parent) :
    GameObject(parent),
    terrainType(nullptr),
    neighbours({
        std::make_pair(MapNode::West, nullptr),
        std::make_pair(MapNode::NorthWest, nullptr),
        std::make_pair(MapNode::NorthEast, nullptr),
        std::make_pair(MapNode::East, nullptr),
        std::make_pair(MapNode::SouthEast, nullptr),
        std::make_pair(MapNode::SouthWest, nullptr)
    })
{
}

MapNode::~MapNode()
{
}

TerrainType * MapNode::getTerrainType() const
{
    return this->terrainType;
}

void MapNode::setTerrainType(TerrainType *terrainType)
{
    if (this->terrainType != terrainType)
    {
        this->terrainType = terrainType;
        emit terrainTypeChanged();
    }
}

MapNode * MapNode::getNeighbour(Direction direction) const
{
    return this->neighbours[direction];
}

QHash<MapNode::Direction, MapNode *> MapNode::getNeighbours() const
{
    return this->neighbours;
}

void MapNode::setNeighbour(MapNode::Direction direction, MapNode *neighbour)
{
    if (this->neighbours[direction] != neighbour)
    {
        MapNode *formerNeighbour = this->neighbours[direction];
        if (formerNeighbour != nullptr)
        {
            Direction neighbourDirection =
                MapNode::oppositeDirections[direction];
            formerNeighbour->removeNeighbour(neighbourDirection);
        }

        this->neighbours[direction] = neighbour;
        neighbour->addNeighbour(MapNode::oppositeDirections[direction], this);

        emit neighboursChanged();
    }
}

void MapNode::setNeighbours(const QHash<MapNode::Direction, MapNode *> &neighbours)
{
    QHash<MapNode::Direction, MapNode *>::ConstIterator it;
    for (it = neighbours.constBegin(); it != neighbours.constEnd(); it++)
    {
        this->setNeighbour(it.key(), it.value());
    }
}

QVariantMap MapNode::readNeighbours() const
{
    return toQVariantMap(
        this->neighbours,
        [&](const Direction &d){return MapNode::direction2str[d];},
        QVariant::fromValue<MapNode *>
    );
}

void MapNode::writeNeighbours(QVariantMap neighbours)
{
    //FIXME: what to do in case of exceptions???
    this->neighbours = fromQVariantMap<QHash<Direction, MapNode *>>(
        neighbours,
        convertStr2direction,
        fromQVariant<MapNode *>
    );
}

void MapNode::dataFromJson(const QJsonObject &obj)
{
    Map *map = qobject_cast<Map *>(this->parent());
    World *world = map->getWorld();

    this->terrainType = resolveReference<TerrainType>(
        obj["terrainType"].toString(),
        world
    );
}

void MapNode::dataToJson(QJsonObject &obj) const
{
    obj["terrainType"] = this->terrainType->objectName();
}

void MapNode::addNeighbour(MapNode::Direction direction, MapNode *neighbour)
{
    MapNode *formerNeighbour = this->neighbours[direction];
    if (formerNeighbour != nullptr)
    {
        Direction neighbourDirection = MapNode::oppositeDirections[direction];
        formerNeighbour->removeNeighbour(neighbourDirection);
    }

    this->neighbours[direction] = neighbour;
    emit neighboursChanged();
}

void MapNode::removeNeighbour(MapNode::Direction direction)
{
    this->neighbours[direction] = nullptr;
    emit neighboursChanged();
}

MapNode::Direction convertStr2direction(const QString &str)
{
    if (!MapNode::str2direction.contains(str))
    {
        throw ValueError(
            "Invalid value for MapNode::Direction " + str
        );
    }
    return MapNode::str2direction[str];
}
