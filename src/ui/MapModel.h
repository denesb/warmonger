#ifndef UI_MODEL_MAP_H
#define UI_MODEL_MAP_H

#include <QAbstractItemModel>

#include "core/Map.h"

namespace warmonger {
namespace ui {

class MapModel :
    public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY(int rowCount READ rowCount)
    Q_PROPERTY(int columnCount READ columnCount)

public:
    MapModel(const core::Map *map, QObject *parent);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;

    Q_INVOKABLE QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    Q_INVOKABLE QModelIndex parent(const QModelIndex & index) const;

    Q_INVOKABLE QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

private:
    void buildDataModel();

    const core::Map *map;
};

} // namespace ui
} // namespace warmonger

#endif // UI_MODEL_MAP_H
