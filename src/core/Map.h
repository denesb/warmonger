#ifndef CORE_MAP_H
#define CORE_MAP_H

#include <QPoint>
#include <QMap>
#include <QList>
#include <QString>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class MapTile;
class Player;
class Unit;
class Settlement;

class Map :
    public GameObject
{
    Q_OBJECT

public:
    Map(QObject *parent = nullptr);
    ~Map();

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    MapTile * getMapTile(const QPoint &position) const;
    void setMapTile(const QPoint &position, MapTile *mapTile);

    QList<Unit *> getUnits() const;
    void setUnits(const QList<Unit *> &units);

    QList<Settlement *> getSettlemets() const;
    void setSettlements(const QList<Settlement *> &settlements);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    QMap<QPoint, MapTile *> mapTilesFromJson(const QJsonObject &obj);
    QJsonObject mapTilesFromJson(const QMap<QPoint, MapTile *> &mapTiles);

    QString displayName;
    World *world;
    int width;
    int height;
    QMap<QPoint, MapTile *> mapTiles;
    QList<Player *> players;
    QList<Unit *> units;
    QList<Settlement *> settlements;
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_MAP_H
