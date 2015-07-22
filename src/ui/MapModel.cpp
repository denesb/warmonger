#include "ui/MapModel.h"
#include "core/MapPosition.h"
#include "core/MapTile.h"
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
        return this->map->getHeight();
    else
        return 3;
}

int MapModel::columnCount(const QModelIndex &parent) const
{
    if (parent == QModelIndex())
        return this->map->getWidth();
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

    core::MapPosition *ptr = new core::MapPosition(row, column);
    return createIndex(row, column, static_cast<void *>(ptr));
}

QModelIndex MapModel::parent(const QModelIndex & index) const
{
    return QModelIndex();
}

QVariant MapModel::data(const QModelIndex &index, int role) const
{
    wDebug("ui.MapModel") << index.column() << "," << index.row();
    const core::MapPosition position(index.column(), index.row());
    const core::MapTile *mapTile = map->getMapTile(position);

    wDebug("ui.MapModel") << "tile: " << mapTile->objectName();

    if (mapTile == nullptr)
        return QVariant();

    wDebug("ui.MapModel") << "asdad";

    return QVariant(mapTile->getTerrainType()->objectName());
}
