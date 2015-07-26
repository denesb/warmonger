#include "ui/MapModel.h"
#include "core/TerrainType.h"

using namespace warmonger::ui;

const QHash<warmonger::core::MapNode::Direction, QPoint> MapModel::mapNodePosDiff{
    std::make_pair(core::MapNode::North, QPoint(0, 2)),
    std::make_pair(core::MapNode::NorthEast, QPoint(1, 1)),
    std::make_pair(core::MapNode::SouthEast, QPoint(1, -1)),
    std::make_pair(core::MapNode::South, QPoint(0, -2)),
    std::make_pair(core::MapNode::SouthWest, QPoint(-1, -1)),
    std::make_pair(core::MapNode::NorthWest, QPoint(-1, 1))
};

MapModel::MapModel(const core::Map *map, QObject *parent) :
    QAbstractItemModel(parent),
    minX(0),
    maxX(0),
    minY(0),
    maxY(0),
    map(map)
{
    this->setObjectName(map->objectName() + "_model");
    this->buildMapNodes();
}

int MapModel::rowCount(const QModelIndex &parent) const
{
    if (parent == QModelIndex())
        return this->maxY - this->minY + 1;
    else
        return 3;
}

int MapModel::columnCount(const QModelIndex &parent) const
{
    if (parent == QModelIndex())
        return this->maxX - this->minX + 1;
    else
        return 1;
}

QModelIndex MapModel::index(int row, int column, const QModelIndex & parent) const
{
    if (parent == QModelIndex())
    {
        if (row > this->rowCount() || column > this->columnCount())
            return QModelIndex();
    }
    else
    {
        if (row > 3 || column > 1)
            return QModelIndex();
    }

    return createIndex(row, column, static_cast<void *>(nullptr));
}

QModelIndex MapModel::parent(const QModelIndex & index) const
{
    return QModelIndex();
}

QVariant MapModel::data(const QModelIndex &index, int role) const
{
    wDebug("ui.MapModel") << index.column() << "," << index.row();
//    const core::MapPosition position(index.column(), index.row());
    //const core::MapTile *mapTile = map->getMapTile(position);

    //wDebug("ui.MapModel") << "tile: " << mapTile->objectName();

    //if (mapTile == nullptr)
     //   return QVariant();

    //return QVariant(mapTile->getTerrainType()->objectName());
    return QVariant();
}

void MapModel::buildMapNodes()
{
    QSet<const core::MapNode *> visitedNodes;
    this->traverseMapNodes(this->map->getMapNodes()[0], QPoint(0, 0), visitedNodes);

    QHash<QPoint, const core::MapNode *>::ConstIterator it;
    for (it = this->mapNodes.constBegin(); it != this->mapNodes.constEnd(); it++)
    {
        this->minX = std::min(it.key().x(), this->minX);
        this->minY = std::min(it.key().y(), this->minY);
        this->maxX = std::max(it.key().x(), this->maxX);
        this->maxY = std::max(it.key().y(), this->maxY);
    }

/*
    for (const core::Settlement *settlement : this->map->getSettlements())
    {
    }

    for (const core::Unit *unit : this->map->getUnits())
    {
    }
    */
}

void MapModel::traverseMapNodes(const core::MapNode *mapNode, QPoint pos, QSet<const core::MapNode *> &visitedNodes)
{
    wDebug("ui.MapModel") << mapNode->objectName() << ": (" << pos.x() << "," << pos.y() << ")";

    visitedNodes << mapNode;
    this->mapNodes[pos] = mapNode;

    const QHash<core::MapNode::Direction, const core::MapNode *> neighbours = mapNode->getNeighbours();
    QHash<core::MapNode::Direction, const core::MapNode *>::ConstIterator it;
    for (it = neighbours.constBegin(); it != neighbours.constEnd(); it++)
    {
        const core::MapNode *neighbour = it.value();
        if (neighbour != nullptr && !visitedNodes.contains(neighbour))
        {
            this->traverseMapNodes(neighbour, pos + MapModel::mapNodePosDiff[it.key()], visitedNodes);
        }
    }
}
