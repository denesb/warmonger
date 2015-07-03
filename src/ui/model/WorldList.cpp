#include "ui/model/WorldList.h"

using namespace ui::model;

WorldList::WorldList(QObject *parent) :
    QAbstractListModel(parent),
    worlds()
{
}

void WorldList::setData(const QList<core::WorldMeta *> &worlds)
{
    this->worlds.clear();
    beginInsertRows(QModelIndex(), this->rowCount(), this->rowCount());
    for (const core::WorldMeta *world : worlds)
    {
        this->worlds << world;
    }
    endInsertRows();
}

int WorldList::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return this->worlds.count();
}

QVariant WorldList::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= worlds.count())
        return QVariant();

    const core::WorldMeta *world = worlds[index.row()];

    if (role == ObjectNameRole)
        return world->objectName();
    else if (role == DisplayNameRole)
        return world->getDisplayName();
    return QVariant();
}

QHash<int, QByteArray> WorldList::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames[ObjectNameRole] = "objectName";
    roleNames[DisplayNameRole] = "displayName";

    return std::move(roleNames);
}
