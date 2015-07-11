#ifndef UI_MODEL_MAP_H
#define UI_MODEL_MAP_H

#include <QAbstractTableModel>

#include "core/Map.h"

namespace warmonger {
namespace ui {

class MapModel :
    public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Roles
    {
        TerrainTypeRole = Qt::UserRole + 1
    };

    MapModel(const core::Map *map, QObject *parent);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    const core::Map *map;
};

} // namespace ui
} // namespace warmonger

#endif // UI_MODEL_MAP_H
