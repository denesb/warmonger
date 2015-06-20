#include <QDir>

#include "core/GameEngine.h"

using namespace core;

GameEngine::GameEngine()
{
    QDir worldsDir("worlds");
    worldsDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    worldsDir.setSorting(QDir::Name);

    for(const QString worldDirName : worldsDir.entryList())
    {
        QString worldPath = worldsDir.canonicalPath() + "/" + worldDirName;
        WorldMeta *worldMeta = new WorldMeta(worldPath, this);

        this->worldsMeta.append(worldMeta);
    }
}

int GameEngine::getVersion()
{
    return 0;
}

QList<WorldMeta*> GameEngine::getWorldList()
{
    return this->worldsMeta;
}
