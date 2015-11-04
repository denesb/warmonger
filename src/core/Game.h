#ifndef CORE_GAME_H
#define CORE_GAME_H

#include <QSet>

#include "core/Map.h"

namespace warmonger {
namespace core {

class MapNode;
class Unit;

class Game :
    public Map
{
    Q_OBJECT

public:

    Game();
    ~Game();

    Q_INVOKABLE virtual QString fileExtension() const;

    QSet<MapNode *> reachableMapNodes(Unit *unit) const;

    QList<MapNode *> shortestPath(
        Unit *unit,
        MapNode *node1,
        MapNode *node2
    ) const;

    void fromMapJson(const QJsonObject &obj);

signals:

private:
    void reachableMapNodes(
        QSet<MapNode *> &reachedNodes,
        MapNode *node,
        Unit *unit,
        double mp
    ) const;

    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    int turnNumber;
    int playerIndex;
};

} // namespace core
} // namespace warmonger

#endif // CORE_GAME_H
