#ifndef UI_MODEL_WORLDLIST_H
#define UI_MODEL_WORLDLIST_H

#include <QAbstractListModel>

#include "core/World.h"

namespace warmonger {
namespace ui {
namespace model {

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

    void setData(const QList<core::World *> &worlds);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<const core::World *> worlds;
};

}; // namespace model
}; // namespace ui
}; // namespace warmonger

#endif // UI_MODEL_WORLDLIST_H
