#ifndef CORE_GAME_ENGINE_H
#define CORE_GAME_ENGINE_H

#include <QList>

#include "core/World.h"

namespace core {

class GameEngine :
    public QObject
{
    Q_OBJECT

public:
    GameEngine();

    int getVersion();

    QList<World*> getWorldList();
	
private:
	World *worldFromDir(const QString &worldDir);

    QList<World*> worlds;
};

}; // namespace core

#endif // CORE_GAME_ENGINE_H
