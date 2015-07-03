#ifndef CORE_WORLD_LOADER_H
#define CORE_WORLD_LOADER_H

#include <QList>
#include <QStringList>
#include <QString>

#include "core/WorldMeta.h"
#include "core/World.h"

namespace core {

class WorldLoader :
    public QObject
{
    Q_OBJECT

public:
    WorldLoader(QObject *parent = nullptr);
    ~WorldLoader();

    QStringList getSearchPath() const;
    void setSearchPath(const QStringList &searchPath);

    QList<WorldMeta *> getWorldList() const;
    World * loadWorld(const QString &worldName);

signals:
    void searchPathChanged(const QStringList &searchPath);
    void worldListChanged(const QList<WorldMeta *> &worldList);

public slots:

private:
    void scanSearchPath();
    void loadWorldList();

    QStringList searchPath;
    QMap<QString, QString> pathToWorldMap;
    QList<WorldMeta*> worldList;
};

}; // namespace core

#endif // CORE_WORLD_LOADER_H
