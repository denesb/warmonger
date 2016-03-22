#ifndef CORE_MAP_NODE_H
#define CORE_MAP_NODE_H

#include <QObject>

#include "core/TerrainType.h"

namespace warmonger {
namespace core {

class MapNode :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(TerrainType *terrainType READ getTerrainType WRITE setTerrainType NOTIFY terrainTypeChanged)

public:
    explicit MapNode(QObject *parent=nullptr);
    ~MapNode();

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    TerrainType * getTerrainType() const;
    void setTerrainType(TerrainType *terrainType);

signals:
    void displayNameChanged();
    void terrainTypeChanged();

private:
    QString displayName;

    TerrainType *terrainType;
};

} // namespace core
} // namespace warmonger

#endif // CORE_MAP_NODE_H
