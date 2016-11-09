#ifndef W_UI_CONTEXT_H
#define W_UI_CONTEXT_H

#include <QDir>
#include <QVariant>

#include "core/CampaignMap.h"
#include "core/World.h"
#include "ui/Palette.h"
#include "ui/WorldSurface.h"

namespace warmonger {
namespace ui {

/**
 * Provides a context for the QML code.
 *
 * Context is main bridge between C++ and QML/JS code. It is accessable as a
 * globally available object. It provides the current world, world-surface,
 * campaign-map the list of available worlds, world-surfaces, campaign-maps,
 * palette-colors and others.
 * It also provides functions to access core functionality.
 */
class Context : public QObject
{
    Q_OBJECT
    Q_PROPERTY(warmonger::core::World* world READ getWorld NOTIFY worldChanged)
    Q_PROPERTY(warmonger::ui::WorldSurface* worldSurface READ getWorldSurface NOTIFY worldSurfaceChanged)
    Q_PROPERTY(warmonger::core::CampaignMap* campaignMap READ getCampaignMap NOTIFY campaignMapChanged)
    Q_PROPERTY(QVariantList worlds READ readWorlds NOTIFY worldsChanged)
    Q_PROPERTY(QVariantList worldSurfaces READ readWorldSurfaces NOTIFY worldSurfacesChanged)
    Q_PROPERTY(QVariantList campaignMaps READ readCampaignMaps NOTIFY campaignMapsChanged)
    Q_PROPERTY(warmonger::ui::Palette* disabledPalette READ getDisabledPalette NOTIFY colorPaletteChanged)
    Q_PROPERTY(warmonger::ui::Palette* inactivePalette READ getActivePalette NOTIFY colorPaletteChanged)
    Q_PROPERTY(warmonger::ui::Palette* activePalette READ getInactivePalette NOTIFY colorPaletteChanged)
    Q_PROPERTY(warmonger::ui::Palette* normalPalette READ getNormalPalette NOTIFY colorPaletteChanged)

public:
    /**
     * Constructs a context object.
     *
     * Upon construction the context will load all available worlds and their
     * surfaces and maps.
     */
    Context(QObject* parent = nullptr);

    /**
     * Get the current world.
     *
     * Will be nullptr when there is no world selected.
     *
     * \return the world
     */
    core::World* getWorld() const
    {
        return this->world;
    }

    /**
     * Get the current world-surface.
     *
     * Will be nullptr when there is no world selected. It will always have a
     * valid value when a world is selected.
     *
     * \return the world-surface
     */
    ui::WorldSurface* getWorldSurface() const
    {
        return this->worldSurface;
    }

    /**
     * Get the campaign-map.
     *
     * Will be nullptr when there is no campaign-map selected.
     *
     * \return the campaign-map
     */
    core::CampaignMap* getCampaignMap() const
    {
        return this->campaignMap;
    }

    /**
     * Get the worlds as a QVariantList.
     *
     * This function is used as a read function for the worlds property and is
     * not supposed to be called from C++ code.
     *
     * \returns the worlds
     */
    QVariantList readWorlds() const;

    /**
     * Get the world-surfaces as a QVariantList.
     *
     * This function is used as a read function for the world-surfaces property
     * and is not supposed to be called from C++ code.
     *
     * \returns the world-surfaces
     */
    QVariantList readWorldSurfaces() const;

    /**
     * Get the campaign-maps as a QVariantList.
     *
     * This function is used as a read function for the campaign-maps property
     * and is not supposed to be called from C++ code.
     *
     * \returns the campaign-maps
     */
    QVariantList readCampaignMaps() const;

    /**
     * Get the disabled palette.
     *
     * \see Palette
     *
     * \return the disabled palette
     */
    Palette* getDisabledPalette() const
    {
        return this->disabledPalette;
    }

    /**
     * Get the active palette.
     *
     * \see Palette
     *
     * \return the active palette
     */
    Palette* getActivePalette() const
    {
        return this->activePalette;
    }

    /**
     * Get the inactive palette.
     *
     * \see Palette
     *
     * \return the inactive palette
     */
    Palette* getInactivePalette() const
    {
        return this->inactivePalette;
    }

    /**
     * Get the normal palette.
     *
     * \see Palette
     *
     * \return the normal palette
     */
    Palette* getNormalPalette() const
    {
        return this->normalPalette;
    }

public slots:
    /**
     * Create a new map.
     *
     * The map will be set as the current map and the world as the
     * current world.
     */
    void newCampaignMap(warmonger::core::World* world);

signals:
    /**
     * Emitted when the world changes.
     */
    void worldChanged();

    /**
     * Emitted right before the world-surface is changed.
     */
    void aboutToChangeWorldSurface();

    /**
     * Emitted after the world-surface has changed.
     */
    void worldSurfaceChanged();

    /**
     * Emitted when the campaign-map changes.
     */
    void campaignMapChanged();

    /**
     * Emitted when the worlds change.
     */
    void worldsChanged();

    /**
     * Emitted when the world-surfaces change.
     */
    void worldSurfacesChanged();

    /**
     * Emitted when the campaign-maps change.
     */
    void campaignMapsChanged();

    /**
     * Emitted when the color-palette changes.
     */
    void colorPaletteChanged();

private:
    void setWorld(core::World* world);
    void setWorldSurface(ui::WorldSurface* worldSurface);
    void setDefaultWorldSurface();
    void setCampaignMap(core::CampaignMap* campaignMap);
    void loadWorlds();
    void loadMapsFromDir(const QDir& mapsDir, core::World* world);
    void loadSurfacesFromDir(const QDir& surfacesDir, core::World* world);

    core::World* world;
    ui::WorldSurface* worldSurface;
    core::CampaignMap* campaignMap;
    std::vector<core::World*> worlds;
    std::map<core::World*, std::vector<ui::WorldSurface*>> worldSurfaces;
    std::vector<core::CampaignMap*> campaignMaps;
    Palette* disabledPalette;
    Palette* activePalette;
    Palette* inactivePalette;
    Palette* normalPalette;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_CONTEXT_H
