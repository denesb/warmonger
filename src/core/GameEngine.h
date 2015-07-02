#ifndef CORE_GAME_ENGINE_H
#define CORE_GAME_ENGINE_H

#include <QList>
#include <QStringList>
#include <QString>

#include "core/WorldMeta.h"
#include "core/World.h"

namespace core {

class GameEngine :
    public QObject
{
    Q_OBJECT

public:
    GameEngine();
    ~GameEngine();

    QStringList getWorldSearchPath() const;
    void setWorldSearchPath(const QStringList &worldSearchPath);

    QList<WorldMeta *> getWorldMetaList() const;
    World * loadWorld(const QString &worldName);

signals:
    void worldSearchPathChanged(const QStringList &worldSearchPath);
    void worldMetaListChanged(const QList<WorldMeta *> &worldMetaList);
    
private:
    void scanWorldSearchPath();
    void loadWorldMetaList();

    QStringList worldSearchPath;
    QMap<QString, QString> pathToWorld;
    QList<WorldMeta*> worldMetas;
    World* world;
};

}; // namespace core

#endif // CORE_GAME_ENGINE_H
