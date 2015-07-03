#include <QDir>

#include "core/WorldLoader.h"
#include "core/JsonUtil.hpp"

using namespace core;

WorldLoader::WorldLoader(QObject *parent) :
    QObject(parent)
{
}

WorldLoader::~WorldLoader()
{
}

QStringList WorldLoader::getSearchPath() const
{
    return this->searchPath;
}

void WorldLoader::setSearchPath(const QStringList &searchPath)
{
    if (this->searchPath != searchPath)
    {
        this->searchPath = searchPath;

        emit this->searchPathChanged(this->searchPath);

        this->scanSearchPath();
        this->loadWorldList();
    }
}

QList<WorldMeta*> WorldLoader::getWorldList() const
{
    return this->worldList;
}

World * WorldLoader::loadWorld(const QString &worldName)
{
    return newFromJsonFile<World>(this->pathToWorldMap[worldName] + "/" + World::WorldDefinitionFile);
}

void WorldLoader::scanSearchPath()
{
    this->pathToWorldMap.clear();

    for (const QString scanPath : this->searchPath)
    {
        QDir scanDir(scanPath);
        scanDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
        scanDir.setSorting(QDir::Name);

        const QString scanCanonicalPath = scanDir.canonicalPath();

        for(const QString dir : scanDir.entryList())
        {
            const QString path = scanCanonicalPath + "/" + dir;
            if (QFile::exists(path + "/" + World::WorldDefinitionFile))
            {
                this->pathToWorldMap[dir] = path;
            }
        }
    }
}

void WorldLoader::loadWorldList()
{
    for (WorldMeta *worldMeta : this->worldList)
    {
        worldMeta->deleteLater();
    }
    this->worldList.clear();

    QMap<QString, QString>::ConstIterator it;
    for (it = this->pathToWorldMap.constBegin(); it != this->pathToWorldMap.constEnd(); it++)
    {
        this->worldList.append(newFromJsonFile<WorldMeta>(it.value() + "/" + World::WorldDefinitionFile, this));
    }
}
