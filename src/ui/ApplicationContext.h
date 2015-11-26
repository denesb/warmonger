#ifndef UI_APPLICATION_CONTEXT_H
#define UI_APPLICATION_CONTEXT_H

#include <QVariant>
#include <QUrl>
#include <QRect>

#include "core/Game.h"

namespace warmonger {
namespace ui {

class ApplicationContext :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList maps READ readMaps NOTIFY mapsChanged)
    Q_PROPERTY(QObject * map READ readMap NOTIFY mapChanged)
    Q_PROPERTY(QObject * game READ readGame NOTIFY gameChanged)
    Q_PROPERTY(QObject * world READ readWorld NOTIFY worldChanged)

public:
    ApplicationContext(QObject *parent=nullptr);
    ~ApplicationContext();

    QVariantList readMaps() const;
    QObject * readMap() const;
    QObject * readGame() const;
    QObject * readWorld() const;

    Q_INVOKABLE bool intersects(QRect rect1, QRect rect2) const;

public slots:
    void loadMaps();
    void closeMaps();

    void newMap();
    void loadMap(QString objectName);
    void loadMapFromPath(QString path);
    void loadMapFromUrl(QUrl url);
    void closeMap();

    void newGame(QObject *map);
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

private slots:
    void onWorldSurfaceChanged();

private:
    QList<core::Map *> maps;
    core::Map *map;
    core::Game *game;
    core::World *world;
};

} // namespace ui
} // namespace warmonger

#endif // UI_APPLICATION_CONTEXT_H
