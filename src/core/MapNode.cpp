#include "core/MapNode.h"
#include "core/TerrainType.h"
#include "core/JsonUtil.h"

using namespace warmonger::core;

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

static const QString category{"core"};

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

QObject * MapNode::readTerrainType() const
{
    return this->terrainType;
}

void MapNode::writeTerrainType(QObject *terrainType)
{
    TerrainType *tt = qobject_cast<TerrainType *>(terrainType);
    if (tt == nullptr)
    {
        wError(category) << "terrainType is null or has wrong type";
        throw Exception(Exception::InvalidValue);
    }

    this->setTerrainType(tt);
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

QObject * MapNode::getNeighbour(QString directionName) const
{
    if (!MapNode::str2direction.contains(directionName))
    {
        wError(category) << "Unknown direction " << directionName;
        throw Exception(Exception::InvalidValue);
    }
    MapNode::Direction direction = MapNode::str2direction[directionName];

    return this->getNeighbour(direction);
}

void MapNode::setNeighbour(QString directionName, QObject * neighbour)
{
    if (!MapNode::str2direction.contains(directionName))
    {
        wError(category) << "invalid direction " << directionName;
        throw Exception(Exception::InvalidValue);
    }
    MapNode::Direction direction = MapNode::str2direction[directionName];

    MapNode *n = qobject_cast<MapNode *>(neighbour);
    if (neighbour == nullptr)
    {
        wError(category) << "neighbour is null or has wrong type";
        throw Exception(Exception::InvalidValue);
    }

    this->setNeighbour(direction, n);
}


QVariantMap MapNode::readNeighbours() const
{
    QVariantMap vmap;
    QHash<Direction, MapNode *>::ConstIterator it;
    for (it = this->neighbours.constBegin(); it != this->neighbours.constEnd(); it++)
    {
        QVariant val;
        if (it.value() != nullptr)
        {
            val = QVariant::fromValue<QObject *>(it.value());
        }
        vmap.insert(MapNode::direction2str[it.key()], val);
    }

    return std::move(vmap);
}

void MapNode::writeNeighbours(QVariantMap neighbours)
{
    QVariantMap::ConstIterator it;
    for (it = neighbours.constBegin(); it != neighbours.constEnd(); it++)
    {
        QVariant neighbour = it.value();
        if (!neighbour.canConvert<MapNode *>())
        {
            wError(category) << "neighbour has wrong type";
            throw Exception(Exception::WrongType);
        }
        MapNode *n = neighbour.value<MapNode *>();

        this->setNeighbour(it.key(), n);
    }
}

QString MapNode::oppositeDirection(QString directionStr) const
{
    if (!MapNode::str2direction.contains(directionStr))
    {
        wError(category) << "Unknown direction " << directionStr;
        throw Exception(Exception::InvalidValue);
    }

    Direction direction = MapNode::str2direction[directionStr];
    Direction oppositeDirection = MapNode::oppositeDirections[direction];

    return MapNode::direction2str[oppositeDirection];
}

void MapNode::dataFromJson(const QJsonObject &obj)
{
    this->terrainType = resolveReference<TerrainType>(
        obj["terrainType"].toString(),
       this->parent()
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
