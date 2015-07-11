#ifndef CORE_MAP_H
#define CORE_MAP_H

#include <QMap>
#include <QList>
#include <QString>

#include "core/GameObject.h"
#include "core/MapPosition.h"

namespace warmonger {
namespace core {

class World;
class MapTile;
class Player;
class Unit;
class Settlement;

class Map :
    public GameObject
{
    Q_OBJECT

public:
    Map(QObject *parent);
    ~Map();

    QString getDisplayName()const;
    void setDisplayName(const QString &displayName);

    const World * getWorld() const;
    void setWorld(const World *world);

    int getWidth() const;
    void setWidth(int width);

    int getHeight() const;
    void setHeight(int height);

    const MapTile * getMapTile(const MapPosition &position) const;
    MapTile * getMapTile(const MapPosition &position);
    void setMapTile(const MapPosition &position, MapTile *mapTile);

    QList<const Player *> getPlayers() const;
    QList<Player *> getPlayers();
    void setPlayers(const QList<Player *> &units);

    QList<const Unit *> getUnits() const;
    QList<Unit *> getUnits();
    void setUnits(const QList<Unit *> &units);

    QList<const Settlement *> getSettlemets() const;
    QList<Settlement *> getSettlemets();
    void setSettlements(const QList<Settlement *> &settlements);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    QMap<MapPosition, MapTile *> mapTilesFromJson(const QJsonObject &obj);
    QJsonObject mapTilesToJson(const QMap<MapPosition, MapTile *> &mapTiles) const;

    QString displayName;
    const World *world;
    int width;
    int height;
    QMap<MapPosition, MapTile *> mapTiles;
    QList<Player *> players;
    QList<Unit *> units;
    QList<Settlement *> settlements;
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_MAP_H
