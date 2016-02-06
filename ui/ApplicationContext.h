#ifndef UI_APPLICATION_CONTEXT_H
#define UI_APPLICATION_CONTEXT_H

#include <QVariant>
#include <QUrl>
#include <QRect>

#include "core/Game.h"
#include "core/Map.h"
#include "core/MapNode.h"
#include "core/Settlement.h"
#include "core/World.h"
#include "core/Unit.h"

namespace warmonger {
namespace ui {

class ApplicationContext :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList maps READ readMaps NOTIFY mapsChanged)
    Q_PROPERTY(warmonger::core::Map *map READ getMap NOTIFY mapChanged)
    Q_PROPERTY(warmonger::core::Game *game READ getGame NOTIFY gameChanged)
    Q_PROPERTY(warmonger::core::World *world READ getWorld NOTIFY worldChanged)

public:
    ApplicationContext(QObject *parent=nullptr);
    ~ApplicationContext();

    QVariantList readMaps() const;
    core::Map * getMap() const;
    core::Game * getGame() const;
    core::World * getWorld() const;

public slots:
    void loadMaps();
    void closeMaps();

    void newMap();
    void loadMap(QString objectName);
    void loadMapFromPath(QString path);
    void loadMapFromUrl(QUrl url);
    void closeMap();

    void newGame(warmonger::core::Map *map);
    void loadGame(QString objectName);
    void loadGameFromPath(QString path);
    void loadGameFromUrl(QUrl url);
    void closeGame();

signals:
    void mapsChanged();
    void mapChanged();
    void gameChanged();
    void worldChanged();

private:
    void setWorld(core::World *world);

private:
    QList<core::Map *> maps;
    core::Map *map;
    core::Game *game;
    core::World *world;
};

} // namespace ui
} // namespace warmonger

#endif // UI_APPLICATION_CONTEXT_H
