#include "ui/MapModel.h"
#include "core/MapPosition.h"
#include "core/MapTile.h"

using namespace warmonger::ui;

MapModel::MapModel(const core::Map *map, QObject *parent) :
    QAbstractTableModel(parent),
    map(map)
{
}

int MapModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return this->map->getHeight();
}

int MapModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return this->map->getWidth();
}

QVariant MapModel::data(const QModelIndex &index, int role) const
{
    const core::MapPosition position(index.column(), index.row());
    const core::MapTile *mapTile = map->getMapTile(position);

    if (mapTile == nullptr)
        return QVariant();

    if (role == TerrainTypeRole)
        return QVariant(mapTile->objectName());
    
    return QVariant();
}

QHash<int, QByteArray> MapModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames[TerrainTypeRole] = "terrainType";

    return std::move(roleNames);
}
