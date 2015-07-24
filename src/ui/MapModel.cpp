#include "ui/MapModel.h"
#include "core/MapNode.h"
#include "core/TerrainType.h"

using namespace warmonger::ui;

MapModel::MapModel(const core::Map *map, QObject *parent) :
    QAbstractItemModel(parent),
    map(map)
{
    this->setObjectName(map->objectName() + "_model");
}

int MapModel::rowCount(const QModelIndex &parent) const
{
    if (parent == QModelIndex())
        return this->map->getMaxHeight();
    else
        return 3;
}

int MapModel::columnCount(const QModelIndex &parent) const
{
    if (parent == QModelIndex())
        return this->map->getMaxWidth();
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
    for (const core::Settlement *settlement : this->map->getSettlements())
    {
    }

    for (const core::Unit *unit : this->map->getUnits())
    {
    }
}
