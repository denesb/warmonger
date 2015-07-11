#ifndef UI_MODEL_MAPLIST_H
#define UI_MODEL_MAPLIST_H

#include <QAbstractListModel>

#include "core/Map.h"

namespace warmonger {
namespace ui {
namespace model {

class MapList :
    public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        ObjectNameRole = Qt::UserRole + 1,
        DisplayNameRole
    };

    MapList(QObject *parent = nullptr);

    void setData(const QList<core::Map *> &maps);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<const core::Map *> maps;
};

}; // namespace model
}; // namespace ui
}; // namespace warmonger

#endif // UI_MODEL_MAPLIST_H
