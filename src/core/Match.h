#include <QJsonObject>
#include <QList>

#ifndef CORE_MATCH_H
#define CORE_MATCH_H

namespace core {

class World;
class Map;
class Player;

class Match :
    public QObject
{
    Q_OBJECT

public:
    Match(const World *world, Map *map, QObject *parent = nullptr);
    Match(const QJsonObject &json, QObject *parent = nullptr);

    Player * getPlayers() const;
    Player * getPlayer(int i) const;

    void save() const;

private:
    World *world;
    Map *map;
    QList<Player *> players;
};

}; // namespace core

#endif // CORE_MATCH_H
