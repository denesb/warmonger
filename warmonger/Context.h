/** \file
 * Context class.
 *
 * \copyright (C) 2015-2016 Botond Dénes
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

#ifndef W_WARMONGER_CONTEXT_H
#define W_WARMONGER_CONTEXT_H

#include <cassert>

#include <QVariant>

#include "core/CampaignMap.h"
#include "core/World.h"
#include "ui/Palette.h"
#include "ui/WorldSurface.h"
#include "Version.h"

namespace warmonger {

/**
 * Provides a context for the QML code.
 *
 * Context is main bridge between C++ and QML/JS code. It is accessable as a
 * globally available object. It provides the current world, world-surface,
 * campaign-map and the list of available campaign-maps, palette-colors
 * and others.
 * It also provides functions to access core functionality.
 */
class Context : public QObject
{
    Q_OBJECT
    Q_PROPERTY(warmonger::core::World* world READ getWorld CONSTANT)
    Q_PROPERTY(warmonger::ui::WorldSurface* worldSurface READ getWorldSurface CONSTANT)
    Q_PROPERTY(
        warmonger::core::CampaignMap* campaignMap READ getCampaignMap WRITE setCampaignMap NOTIFY campaignMapChanged)
    Q_PROPERTY(QVariantList campaignMaps READ readCampaignMaps NOTIFY campaignMapsChanged)
    Q_PROPERTY(QString version READ getVersion CONSTANT)
    Q_PROPERTY(warmonger::ui::Palette* disabledPalette READ getDisabledPalette CONSTANT)
    Q_PROPERTY(warmonger::ui::Palette* inactivePalette READ getActivePalette CONSTANT)
    Q_PROPERTY(warmonger::ui::Palette* activePalette READ getInactivePalette CONSTANT)
    Q_PROPERTY(warmonger::ui::Palette* normalPalette READ getNormalPalette CONSTANT)

public:
    /**
     * Constructs a context object.
     *
     * \param world the world to use
     * \param worldSurface the worldSurface to use
     * \param parent the parent QObject
     *
     * The context object takes ownership of the world and worldSurface.
     */
    Context(std::unique_ptr<core::World>&& world,
        std::unique_ptr<ui::WorldSurface>&& worldSurface,
        QObject* parent = nullptr);

    /**
     * Get the world.
     *
     * Will be nullptr when there is no world selected.
     *
     * \return the world
     */
    core::World* getWorld() const
    {
        assert(this->world != nullptr);

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
        assert(this->worldSurface != nullptr);

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
     * Set the campaign-map.
     *
     * The context will take ownership of the map.
     * Will emit the signal Context::campaignMapChanged() if the newly set
     * value is different than the current one.
     *
     * \param campaignMap the new campaign-map
     */
    void setCampaignMap(core::CampaignMap* campaignMap);

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
     * Get the application version.
     *
     * \returns the application version
     */
    QString getVersion() const
    {
        return QString(version.c_str());
    }

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

signals:
    /**
     * Emitted when the campaign-map changes.
     */
    void campaignMapChanged();

    /**
     * Emitted when the campaign-maps change.
     */
    void campaignMapsChanged();

private:
    core::World* world;
    ui::WorldSurface* worldSurface;
    core::CampaignMap* campaignMap;
    std::vector<core::CampaignMap*> campaignMaps;
    ui::Palette* disabledPalette;
    ui::Palette* activePalette;
    ui::Palette* inactivePalette;
    ui::Palette* normalPalette;
};

} // namespace warmonger

#endif // W_WARMONGER_CONTEXT_H
