/** \file
 * Context class.
 *
 * \copyright (C) 2015-2018 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef W_WMAPEDITOR_CONTEXT_H
#define W_WMAPEDITOR_CONTEXT_H

#include <QDir>
#include <QVariant>

#include "core/Map.h"
#include "core/World.h"
#include "ui/Palette.h"
#include "ui/WorldSurface.h"

namespace warmonger {
namespace wmapeditor {

/**
 * Provides a campaign-map editor specific context for the QML code.
 *
 * Context is main bridge between C++ and QML/JS code. It is accessable as a
 * globally available object. It provides the current world, world-surface,
 * campaign-map the list of available worlds, world-surfaces, campaign-maps,
 * palette-colors and others.
 * It also provides functions to access core functionality.
 * This context provides functions specific to the camaign-map editor.
 */
class Context : public QObject
{
    Q_OBJECT
    Q_PROPERTY(warmonger::core::World* world READ getWorld NOTIFY worldChanged)
    Q_PROPERTY(warmonger::ui::WorldSurface* worldSurface READ getWorldSurface NOTIFY worldSurfaceChanged)
    Q_PROPERTY(warmonger::core::Map* map READ getMap WRITE setMap NOTIFY mapChanged)
    Q_PROPERTY(QVariantList worlds READ readWorlds NOTIFY worldsChanged)
    Q_PROPERTY(QVariantList worldSurfaces READ readWorldSurfaces NOTIFY worldSurfacesChanged)
    Q_PROPERTY(QVariantList maps READ readMaps NOTIFY mapsChanged)
    Q_PROPERTY(warmonger::ui::Palette* disabledPalette READ getDisabledPalette CONSTANT)
    Q_PROPERTY(warmonger::ui::Palette* inactivePalette READ getActivePalette CONSTANT)
    Q_PROPERTY(warmonger::ui::Palette* activePalette READ getInactivePalette CONSTANT)
    Q_PROPERTY(warmonger::ui::Palette* normalPalette READ getNormalPalette CONSTANT)

public:
    /**
     * Error
     */
    enum class ErrorCategory
    {
        IOError,
        ContentError,
        InternalError,
        UnknownError
    };
    Q_ENUM(ErrorCategory)

    /**
     * Constructs a context object.
     *
     * Upon construction the context will load all available worlds and their
     * surfaces and maps.
     *
     * \param parent the parent QObject
     */
    Context(QObject* parent = nullptr);

    /**
     * Add world to the world list.
     *
     * The world is added to the list of worlds. This list is built by scanning
     * the worlds dir provided in the settings and any other world added
     * explicitely.
     */
    void addWorld(const QString& path);

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
    core::Map* getMap() const
    {
        return this->map;
    }

    /**
     * Set the campaign-map.
     *
     * The context will take ownership of the map.
     * Will emit the signal Context::mapChanged() if the newly set
     * value is different than the current one.
     *
     * \param map the new campaign-map
     */
    void setMap(core::Map* map);

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
    QVariantList readMaps() const;

    /**
     * Get the disabled palette.
     *
     * \see Palette
     *
     * \return the disabled palette
     */
    ui::Palette* getDisabledPalette() const
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
    ui::Palette* getActivePalette() const
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
    ui::Palette* getInactivePalette() const
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
    ui::Palette* getNormalPalette() const
    {
        return this->normalPalette;
    }

    /**
     * Create a new map.
     *
     * The map will be set as the current map and the world as the
     * current world.
     */
    Q_INVOKABLE void create(warmonger::core::World* world);

    /**
     * Save the current campaign-map.
     *
     * The map will be saved to the current path for the campaign-map.
     * This will be the path used for the last save operation. If there
     * was no previous save, the map will be saved to the default map
     * location for the world: {world-path}/maps/{object-name}.wmd.
     * Where world-path is the path to the directory where the current world
     * is and object-name is the object-name of the map.
     * If the save fails retrieve the error with Context::getLastError().
     *
     * \return true if the save succeded, false otherwise
     */
    Q_INVOKABLE bool save();

    /**
     * Save the current campaign-map as path.
     *
     * The map will be saved to path. The path has to be writable.
     * If the save fails retrieve the error with Context::getLastError().
     *
     * \return true if the save succeded, false otherwise
     *
     * \see Context::save()
     */
    Q_INVOKABLE bool saveAs(const QString& path);

    /**
     * Load the campaign-map from path.
     *
     * If the load is successfull campaign-map will be the current
     * campaign-map and it's world the current world.
     * If the load fails retrieve the error with Context::getLastError().
     *
     * \return true if the load succeded, false otherwise
     */
    Q_INVOKABLE bool load(const QString& path);

    /**
     * Get the path used for the last save operation.
     *
     * Retrieve the path used in the last call of Context::save() or
     * Context::saveAs() call.
     *
     * \return the last path
     */
    Q_INVOKABLE QString getLastPath() const
    {
        return this->lastPath;
    }

    /**
     * Get the error category for the last failed operation.
     *
     * Retrieve the error category for the last failed Context::save(),
     * Context::saveAs() or Context::load() call.
     *
     * \return the error category
     */
    Q_INVOKABLE ErrorCategory getLastErrorCategory() const
    {
        return this->lastErrorCategory;
    }

    /**
     * Get the error message for the last failed operation.
     *
     * Retrieve the error message for the last failed Context::save(),
     * Context::saveAs() or Context::load() call.
     *
     * \return the human-readable error message
     */
    Q_INVOKABLE QString getLastErrorMessage() const
    {
        return this->lastErrorMessage;
    }

signals:
    /**
     * Emitted when the world changes.
     */
    void worldChanged();

    /**
     * Emitted when the world-surface has changed.
     */
    void worldSurfaceChanged();

    /**
     * Emitted when the campaign-map changes.
     */
    void mapChanged();

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
    void mapsChanged();

private:
    void setWorld(core::World* world);
    void setWorldSurface(ui::WorldSurface* worldSurface);
    void setDefaultWorldSurface();
    void loadWorlds();
    void loadMapsFromDir(const QDir& mapsDir, core::World* world);
    void loadSurfacesFromDir(const QDir& surfacesDir, core::World* world);

    core::World* world;
    ui::WorldSurface* worldSurface;
    core::Map* map;
    std::vector<core::World*> worlds;
    std::map<core::World*, std::vector<ui::WorldSurface*>> worldSurfaces;
    std::vector<core::Map*> maps;
    ui::Palette* disabledPalette;
    ui::Palette* activePalette;
    ui::Palette* inactivePalette;
    ui::Palette* normalPalette;
    ErrorCategory lastErrorCategory;
    QString lastErrorMessage;
    QString lastPath;
};

} // namespace wmapeditor
} // namespace warmonger

#endif // W_WMAPEDITOR_CONTEXT_H
