#ifndef UI_MODEL_WORLDLIST_H
#define UI_MODEL_WORLDLIST_H

#include <QAbstractListModel>

#include "core/WorldMeta.h"

namespace ui { namespace model {

class WorldList :
    public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        ObjectNameRole = Qt::UserRole + 1,
        DisplayNameRole
    };

    WorldList(QObject *parent = nullptr);

    void addWorld(const core::WorldMeta *world);

    void addWorlds(const QList<core::WorldMeta *> &worlds);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<const core::WorldMeta *> worlds;
};

}; }; // namespace ui::model

#endif // UI_MODEL_WORLDLIST_H
