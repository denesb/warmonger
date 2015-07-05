#ifndef CORE_WORLD_LOADER_H
#define CORE_WORLD_LOADER_H

#include <QList>
#include <QMap>
#include <QStringList>
#include <QString>

#include "core/World.h"

namespace warmonger {
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

    QList<World *> getWorldList() const;
    World * loadWorld(const QString &worldName);

signals:
    void searchPathChanged(const QStringList &searchPath);
    void worldListChanged(const QList<World *> &worldList);

public slots:

private:
    void scanSearchPath();
    void loadWorldList();

    QStringList searchPath;
    QMap<QString, QString> pathToWorldMap;
    QList<World*> worldList;
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_WORLD_LOADER_H
