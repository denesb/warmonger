#ifndef UI_APPLICATION_CONTEXT_H
#define UI_APPLICATION_CONTEXT_H

#include <QVariant>

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

public:
    ApplicationContext(QObject *parent);
    ~ApplicationContext();

    QVariantList readMaps() const;
    QObject * readMap() const;
    QObject * readGame() const;

public slots:
    void loadMaps();
    void closeMaps();

    void newMap();
    void loadMap(QString objectName);
    void loadMapFrom(QString path);
    void closeMap();

    void newGame(QObject *map);
    void loadGame(QString objectName);
    void loadGameFrom(QString path);
    void closeGame();

signals:
    void mapsChanged();
    void mapChanged();
    void gameChanged();

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
