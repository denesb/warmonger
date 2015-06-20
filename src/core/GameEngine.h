#include <QList>

#include "core/World.h"

#ifndef CORE_GAME_ENGINE_H
#define CORE_GAME_ENGINE_H

namespace core {

class GameEngine :
    public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(int               version READ getVersion)
    Q_PROPERTY(QList<WorldMeta*> worlds  READ getWorldList)

    GameEngine();

    int getVersion();

    QList<WorldMeta*> getWorldList();

private:
    QList<WorldMeta*> worldsMeta;
    QList<World*> worlds;
};

}; // namespace core

#endif // CORE_GAME_ENGINE_H
