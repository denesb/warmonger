#include <QDir>

#include "core/GameEngine.h"
#include "core/JsonUtil.hpp"

using namespace core;

GameEngine::GameEngine()
{
}

GameEngine::~GameEngine()
{
}

QStringList GameEngine::getWorldSearchPath() const
{
	return this->worldSearchPath;
}

void GameEngine::setWorldSearchPath(const QStringList &worldSearchPath)
{
	if (this->worldSearchPath != worldSearchPath)
	{
		this->worldSearchPath = worldSearchPath;
		this->scanWorldSearchPath();
	}
}

QList<WorldMeta*> GameEngine::getWorldMetaList() const
{
    return this->worldMetas;
}

World * GameEngine::loadWorld(const QString &worldName)
{
	return newFromJsonFile<World>(this->pathToWorld[worldName]);
}

void GameEngine::scanWorldSearchPath()
{
	this->pathToWorld.clear();

	for (const QString scanPath : this->worldSearchPath)
	{
		QDir scanDir(scanPath);
		scanDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
		scanDir.setSorting(QDir::Name);

		for(const QString dir : scanDir.entryList())
		{
			const QString path = scanPath + "/" + dir;
			if (QFile::exists(path + "/world.json"))
			{
				this->pathToWorld[dir] = path;
			}
		}
	}
}

void GameEngine::loadWorldMetaList()
{
	for (WorldMeta *worldMeta : this->worldMetas)
	{
		worldMeta->deleteLater();
	}
	this->worldMetas.clear();

	QMap<QString, QString>::ConstIterator it;
	for (it = this->pathToWorld.constBegin(); it != this->pathToWorld.constEnd(); it++)
	{
		this->worldMetas.append(newFromJsonFile<WorldMeta>(it.value(), this));
	}
}
