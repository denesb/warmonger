#include "core/MapNode.h"
#include "core/World.h"
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

QVariant MapNode::readTerrainType() const
{
    return QVariant::fromValue<QObject *>(this->terrainType);
}

void MapNode::writeTerrainType(QVariant terrainType)
{
    if (!terrainType.canConvert<TerrainType *>())
    {
        wError("core") << "terrainType has wrong type";
        throw Exception(Exception::WrongType);
    }
    TerrainType *tt = terrainType.value<TerrainType *>();

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
        this->neighbours[direction] = neighbour;
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

QVariant MapNode::readNeighbour(QString directionName) const
{
    if (!MapNode::str2direction.contains(directionName))
    {
        wError(category) << "Unknown direction " << directionName;
        throw Exception(Exception::InvalidValue);
    }
    MapNode::Direction direction = MapNode::str2direction[directionName];

    return QVariant::fromValue<QObject *>(this->getNeighbour(direction));
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

void MapNode::writeNeighbour(QString directionName, QVariant neighbour)
{
    if (!MapNode::str2direction.contains(directionName))
    {
        wError("core") << "invalid direction " << directionName;
        throw Exception(Exception::InvalidValue);
    }
    MapNode::Direction direction = MapNode::str2direction[directionName];

    if (!neighbour.canConvert<MapNode *>())
    {
        wError("core") << "neighbour has wrong type";
        throw Exception(Exception::WrongType);
    }
    MapNode *n = neighbour.value<MapNode *>();
    
    this->setNeighbour(direction, n);
}

void MapNode::writeNeighbours(QVariantMap neighbours)
{
    QVariantMap::ConstIterator it;
    for (it = neighbours.constBegin(); it != neighbours.constEnd(); it++)
    {
        this->writeNeighbour(it.key(), it.value());
    }
}

QString MapNode::oppositeDirection(QString directionStr) const
{
    if (!MapNode::str2direction.contains(directionStr))
    {
        wError("core") << "Unknown direction " << directionStr;
        throw Exception(Exception::InvalidValue);
    }

    Direction direction = MapNode::str2direction[directionStr];
    Direction oppositeDirection = MapNode::oppositeDirections[direction];

    return MapNode::direction2str[oppositeDirection];
}

void MapNode::dataFromJson(const QJsonObject &obj)
{
    World *world = this->parent()->findChild<World *>(QString(), Qt::FindDirectChildrenOnly);
    if (world == nullptr)
    {
        wError("core") << "World is null";
        throw Exception(Exception::NullPointer);
    }

    const QString terrainTypeName = obj["terrainType"].toString();
    this->terrainType = resolveReference<TerrainType>(terrainTypeName, world);
}

void MapNode::dataToJson(QJsonObject &obj) const
{
    obj["terrainType"] = this->terrainType->objectName();
}
