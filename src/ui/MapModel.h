#ifndef UI_MAP_MODEL_H
#define UI_MAP_MODEL_H

#include <QAbstractItemModel>
#include <QHash>
#include <QSet>
#include <QPoint>

#include "core/Map.h"
#include "core/MapNode.h"

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
    static const QHash<core::MapNode::Direction, QPoint> mapNodePosDiff;

    void buildMapNodes();
    void traverseMapNodes(const core::MapNode *mapNode, QPoint pos, QSet<const core::MapNode *> &visitedNodes);

    int maxWidth;
    int maxHeight;
    int minX, maxX;
    int minY, maxY;
    const core::Map *map;
    QHash<QPoint, const core::MapNode *> mapNodes;
};

} // namespace ui
} // namespace warmonger

inline uint qHash(const QPoint &key, uint seed = 0)
{
    ulong val = (key.x() << ((sizeof(int) / 2 ) * 8)) + key.y();
    return qHash(val, seed);
}

#endif // UI_MAP_MODEL_H
