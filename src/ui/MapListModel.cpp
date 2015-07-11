#include "ui/model/MapList.h"

using namespace warmonger::ui::model;

MapList::MapList(QObject *parent) :
    QAbstractListModel(parent),
    maps()
{
}

void MapList::setData(const QList<core::Map *> &maps)
{
    this->maps.clear();
    beginInsertRows(QModelIndex(), this->rowCount(), this->rowCount());
    for (const core::Map *map : maps)
    {
        this->maps << map;
    }
    endInsertRows();
}

int MapList::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return this->maps.count();
}

QVariant MapList::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= maps.count())
        return QVariant();

    const core::Map *map = maps[index.row()];

    if (role == ObjectNameRole)
        return map->objectName();
    else if (role == DisplayNameRole)
        return map->getDisplayName();
    return QVariant();
}

QHash<int, QByteArray> MapList::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames[ObjectNameRole] = "objectName";
    roleNames[DisplayNameRole] = "displayName";

    return std::move(roleNames);
}
