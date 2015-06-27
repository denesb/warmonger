#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

#include "core/GameEngine.h"

using namespace core;

GameEngine::GameEngine()
{
    QDir worldsDir("worlds");
    worldsDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    worldsDir.setSorting(QDir::Name);
	const QString worldsDirPath = worldsDir.canonicalPath();

    for(const QString worldDirName : worldsDir.entryList())
    {
		this->worlds.append(this->worldFromDir(worldsDirPath + "/" + worldDirName));
	}
}

int GameEngine::getVersion()
{
    return 0;
}

QList<World*> GameEngine::getWorldList()
{
    return this->worlds;
}

World *GameEngine::worldFromDir(const QString &worldDir)
{
	QFile worldFile(worldDir + "/world.json");

    if (!worldFile.open(QIODevice::ReadOnly))
    {
        //TODO: throw something
    }

    QByteArray jsonData = worldFile.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(jsonData));

    World *world = new World(this);
	world->fromJson(doc.object());

	return world;
}
