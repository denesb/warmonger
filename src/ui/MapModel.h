#ifndef UI_MAP_MODEL_H
#define UI_MAP_MODEL_H

#include <QAbstractItemModel>
#include <QHash>

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
    void buildMapNodes();

    const core::Map *map;
};

} // namespace ui
} // namespace warmonger

#endif // UI_MAP_MODEL_H
