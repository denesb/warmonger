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

#ifndef W_WARMONGER_CONTEXT_H
#define W_WARMONGER_CONTEXT_H

#include <cassert>

#include <QVariant>

#include "Version.h"
#include "core/Map.h"
#include "core/World.h"
#include "ui/Palette.h"
#include "ui/WorldSurface.h"

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
    Q_PROPERTY(QString version READ getVersion CONSTANT)
    Q_PROPERTY(warmonger::ui::Palette* disabledPalette READ getDisabledPalette CONSTANT)
    Q_PROPERTY(warmonger::ui::Palette* inactivePalette READ getActivePalette CONSTANT)
    Q_PROPERTY(warmonger::ui::Palette* activePalette READ getInactivePalette CONSTANT)
    Q_PROPERTY(warmonger::ui::Palette* normalPalette READ getNormalPalette CONSTANT)
    Q_PROPERTY(State state READ getState WRITE setState NOTIFY stateChanged)
    // Each special context object exposed by their respective states.
    // QML doesn't appear to see newly added dynamic properties so they
    // need to be statically pre-declared here.
    Q_PROPERTY(QObject* randomMapGenerator READ getSpecialContextObject NOTIFY stateChanged)

public:
    enum class State
    {
        MainMenu,
        NewRandomMap,
        Gameplay,
    };
    Q_ENUM(State);

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
     * Get the application version.
     *
     * \returns the application version
     */
    QString getVersion() const
    {
        return version;
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

    State getState() const
    {
        return this->state;
    }

    /**
     * Set the state to `nextState`.
     *
     * The context object is always in a state. Each state is tied to
     * a specific QML page and almost each state has an associated
     * special context object that provides additional functionality
     * needed by the associated QML page. The default state is
     * MainMenu. For a list of states see the \ref State enum.
     * The following state transitions are valid:
     * * MainMenu -> NewRandomMap
     * * NewRandomMap -> Gameplay
     *
     * Transitioning from any state to MainMenu is always valid.
     *
     * Some states expose special context objects:
     * * NewRandomMap: \ref NewRandomMapContext
     *
     * \trows utils::ValueError if an invalid state transition is requested.
     */
    void setState(State nextState);

    /**
     * The special context object associated with the current state.
     *
     * The special context object will be different for each state and
     * may be null in some states. This is to be used from QML and hence
     * only a QObject* is exposed here.
     *
     * \see \ref setState()
     */
    QObject* getSpecialContextObject() const
    {
        return this->specialContextObject;
    }

    Q_INVOKABLE QColor color(warmonger::core::Color* color) const {
        return this->worldSurface->colorFor(*color);
    }

signals:
    void stateChanged();

private:
    core::World* world;
    ui::WorldSurface* worldSurface;
    ui::Palette* disabledPalette;
    ui::Palette* activePalette;
    ui::Palette* inactivePalette;
    ui::Palette* normalPalette;
    State state = State::MainMenu;
    QObject* specialContextObject = nullptr;
    const char* specialContextPropertyName;
};

/**
 * Context class for the random map generation QML page.
 */
class NewRandomMapContext : public QObject
{
    Q_OBJECT
    Q_PROPERTY(unsigned size READ getSize WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(unsigned numOfPlayers READ getNumOfPlayers WRITE setNumOfPlayers NOTIFY numOfPlayersChanged)
    Q_PROPERTY(QVariantList players READ readPlayers NOTIFY playersChanged)

    struct BannerConfiguration
    {
        core::Banner* banner = nullptr;
        core::Color* primaryColor = nullptr;
        core::Color* secondaryColor = nullptr;

        BannerConfiguration() = default;
        BannerConfiguration(core::Banner* banner, core::Color* primaryColor, core::Color* secondaryColor)
            : banner(banner)
            , primaryColor(primaryColor)
            , secondaryColor(secondaryColor)
        {
        }

        bool operator==(const BannerConfiguration& other) const;
    };

public:
    NewRandomMapContext(core::World& world, QObject* parent = nullptr)
        : QObject(parent)
        , world(world)
    {
    }

    unsigned getSize() const
    {
        return this->size;
    }

    void setSize(unsigned size);

    unsigned getNumOfPlayers() const
    {
        return this->numOfPlayers;
    }

    void setNumOfPlayers(unsigned numOfPlayers);

    QVariantList readPlayers() const;

    std::unique_ptr<core::Map> generateMap() const;

signals:
    void sizeChanged();
    void numOfPlayersChanged();
    void playersChanged();

private:
    void adjustPlayers();

    BannerConfiguration nextAvailableBannerConfiguration() const;

    core::World& world;
    unsigned size = 0;
    unsigned numOfPlayers = 0;
    std::vector<std::unique_ptr<core::Faction>> players;
};

class GameplayContext : public QObject
{
public:
    GameplayContext(std::unique_ptr<core::Map> map, QObject* parent = nullptr);

private:
    core::Map& map;
};

} // namespace warmonger

#endif // W_WARMONGER_CONTEXT_H
