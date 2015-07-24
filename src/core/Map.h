#ifndef CORE_MAP_H
#define CORE_MAP_H

#include <QList>
#include <QString>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class World;
class MapNode;
class Player;
class Unit;
class Settlement;

class Map :
    public GameObject
{
    Q_OBJECT

public:
    static const QString DefinitionFile;

    Map(QObject *parent);
    ~Map();

    QString getDescription()const;
    void setDescription(const QString &description);

    const World * getWorld() const;
    void setWorld(const World *world);

    int getMaxWidth() const;
    void setMaxWidth(int maxWidth);

    int getMaxHeight() const;
    void setMaxHeight(int maxHeight);

    QList<const MapNode *> getMapNodes() const;
    QList<MapNode *> getMapNodes();
    void setMapNodes(const QList<MapNode *> &mapNodes);

    QList<const Player *> getPlayers() const;
    QList<Player *> getPlayers();
    void setPlayers(const QList<Player *> &units);

    QList<const Unit *> getUnits() const;
    QList<Unit *> getUnits();
    void setUnits(const QList<Unit *> &units);

    QList<const Settlement *> getSettlements() const;
    QList<Settlement *> getSettlements();
    void setSettlements(const QList<Settlement *> &settlements);

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    QString description;
    const World *world;
    int maxWidth;
    int maxHeight;
    QList<MapNode *> mapNodes;
    QList<Player *> players;
    QList<Unit *> units;
    QList<Settlement *> settlements;
};

} // namespace core
} // namespace warmonger

#endif // CORE_MAP_H
