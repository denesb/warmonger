#include "core/MapNode.h"
#include "core/World.h"
#include "core/TerrainType.h"
#include "core/JsonUtil.hpp"
#include "Util.h"

using namespace warmonger::core;

const QHash<MapNode::Direction, QString> MapNode::direction2str{
    std::make_pair(MapNode::North, "North"),
    std::make_pair(MapNode::NorthEast, "NorthEast"),
    std::make_pair(MapNode::SouthEast, "SouthEast"),
    std::make_pair(MapNode::South, "South"),
    std::make_pair(MapNode::SouthWest, "SouthWest"),
    std::make_pair(MapNode::NorthWest, "NorthWest")
};

const QHash<QString, MapNode::Direction> MapNode::str2direction{
    std::make_pair("North", MapNode::North),
    std::make_pair("NorthEast", MapNode::NorthEast),
    std::make_pair("SouthEast", MapNode::SouthEast),
    std::make_pair("South", MapNode::South),
    std::make_pair("SouthWest", MapNode::SouthWest),
    std::make_pair("NorthWest", MapNode::NorthWest)
};

MapNode::MapNode(QObject *parent) :
    GameObject(parent),
    terrainType(nullptr),
    neighbours({
        std::make_pair(MapNode::North, nullptr),
        std::make_pair(MapNode::NorthEast, nullptr),
        std::make_pair(MapNode::SouthEast, nullptr),
        std::make_pair(MapNode::South, nullptr),
        std::make_pair(MapNode::SouthWest, nullptr),
        std::make_pair(MapNode::NorthWest, nullptr)
    })
{
}

MapNode::~MapNode()
{
}

const TerrainType * MapNode::getTerrainType() const
{
    return this->terrainType;
}

void MapNode::setTerrainType(const TerrainType *terrainType)
{
    if (this->terrainType != terrainType)
    {
        this->terrainType = terrainType;
        emit terrainTypeChanged();
    }
}

QVariant MapNode::readTerrainType() const
{
    TerrainType *o = const_cast<TerrainType *>(this->terrainType);
    return QVariant::fromValue<QObject *>(o);
}

const MapNode * MapNode::getNeighbour(Direction direction) const
{
    return this->neighbours[direction];
}

const QHash<MapNode::Direction, const MapNode *> MapNode::getNeighbours() const
{
    return this->neighbours;
}

void MapNode::setNeighbour(MapNode::Direction direction, const MapNode *neighbour)
{
    if (this->neighbours[direction] != neighbour)
    {
        this->neighbours[direction] = neighbour;
        emit neighboursChanged();
    }
}

void MapNode::setNeighbours(const QHash<MapNode::Direction, const MapNode *> &neighbours)
{
    if (this->neighbours != neighbours)
    {
        this->neighbours = neighbours;
        emit neighboursChanged();
    }
}

QVariantMap MapNode::readNeighbours() const
{
    QVariantMap vmap;
    QHash<Direction, const MapNode *>::ConstIterator it;
    for (it = this->neighbours.constBegin(); it != this->neighbours.constEnd(); it++)
    {
        if (it.value() != nullptr)
        {
            MapNode *neighbour = const_cast<MapNode *>(it.value());
            vmap.insert(MapNode::direction2str[it.key()], QVariant::fromValue<QObject *>(neighbour));
        }
    }

    return std::move(vmap);
}

void MapNode::dataFromJson(const QJsonObject &obj)
{
    World *world = this->parent()->findChild<World *>(QString(), Qt::FindDirectChildrenOnly);
    if (world == nullptr)
    {
        Exception e(Exception::NullPointer);
        wError("core.MapNode") << e.getMessage();
        throw e;
    }

    const QString terrainTypeName = obj["terrainType"].toString();
    this->terrainType = resolveReference<TerrainType>(terrainTypeName, world, "core.MapNode");
}

void MapNode::dataToJson(QJsonObject &obj) const
{
    obj["terrainType"] = this->terrainType->objectName();
}
