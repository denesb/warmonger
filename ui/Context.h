#ifndef W_UI_CONTEXT_H
#define W_UI_CONTEXT_H

#include <QDir>
#include <QQuickWindow>
#include <QVariant>

#include "core/CampaignMap.h"
#include "core/Game.h"
#include "core/World.h"
#include "ui/WorldSurface.h"

namespace warmonger {
namespace ui {

class Context :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(warmonger::core::World *world READ getWorld NOTIFY worldChanged)
    Q_PROPERTY(warmonger::ui::WorldSurface *worldSurface READ getWorldSurface NOTIFY worldSurfaceChanged)
    Q_PROPERTY(warmonger::core::CampaignMap *campaignMap READ getCampaignMap NOTIFY campaignMapChanged)
    Q_PROPERTY(warmonger::core::Game *game READ getGame NOTIFY gameChanged)
    Q_PROPERTY(QVariantList worlds READ readWorlds NOTIFY worldsChanged)
    Q_PROPERTY(QVariantList worldSurfaces READ readWorldSurfaces NOTIFY worldSurfacesChanged)
    Q_PROPERTY(QVariantList campaignMaps READ readCampaignMaps NOTIFY campaignMapsChanged)

public:
    Context(QQuickWindow *window, QObject *parent=nullptr);

    core::World * getWorld() const;
    ui::WorldSurface * getWorldSurface() const;
    core::CampaignMap * getCampaignMap() const;
    core::Game * getGame() const;
    QVariantList readWorlds() const;
    QVariantList readWorldSurfaces() const;
    QVariantList readCampaignMaps() const;

public slots:
    /**
     * Create a new map.
     *
     * The map will be set as the current map and the world as the
     * current world.
     */
    void newCampaignMap(warmonger::core::World *world);

signals:
    void worldChanged();
    void aboutToChangeWorldSurface();
    void worldSurfaceChanged();
    void campaignMapChanged();
    void gameChanged();
    void worldsChanged();
    void worldSurfacesChanged();
    void campaignMapsChanged();

private:
    void setWorld(core::World *world);
    void setWorldSurface(ui::WorldSurface *worldSurface);
    void setDefaultWorldSurface();
    void setCampaignMap(core::CampaignMap *campaignMap);
    void loadWorlds();
    void loadMapsFromDir(const QDir& mapsDir, core::World *world);
    void loadSurfacesFromDir(const QDir& surfacesDir, core::World *world);

    QQuickWindow *window;
    core::World *world;
    ui::WorldSurface *worldSurface;
    core::CampaignMap *campaignMap;
    core::Game *game;
    std::vector<core::World *> worlds;
    std::map<core::World *, std::vector<ui::WorldSurface *>> worldSurfaces;
    std::vector<core::CampaignMap *> campaignMaps;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_CONTEXT_H