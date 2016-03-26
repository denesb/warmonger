#ifndef CORE_GAME_H
#define CORE_GAME_H

#include <QObject>

#include "core/Map.h"

namespace warmonger {
namespace core {

class Game :
    public QObject
{
    Q_OBJECT

    Q_PROPERTY(int turn READ getTurn WRITE setTurn NOTIFY turnChanged)

public:
    Game(QObject *parent=nullptr);

    Map * getMap() const;
    void setMap(Map *map);

    int getTurn() const;
    void setTurn(int turn);

signals:
    void mapChanged();
    void turnChanged();

private:
    Map *map;
    int turn;
};

} // namespace core
} // namespace warmonger

#endif // CORE_GAME_H
