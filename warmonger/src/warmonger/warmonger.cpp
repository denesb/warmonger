/**
 * Copyright (C) 2015-2018 Botond Dénes
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

#include <backward.hpp>
#include <Godot.hpp>

#include "utils/Logging.h"
#include "utils/Settings.h"
#include "warmonger/Context.h"

namespace backward {

backward::SignalHandling sh;

} // namespace backward

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
    godot::Godot::gdnative_init(o);
    warmonger::utils::initSettings();
    warmonger::utils::initLogging();
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
    godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
    godot::Godot::nativescript_init(handle);

    godot::register_class<warmonger::Context>();
}

#if 0

static std::unique_ptr<Context> createContext(const QString& worldPath, const QString& worldSurfacePath)
{
    std::unique_ptr<core::World> world;

    try
    {
        world = io::readWorld(worldPath);
    }
    catch (const utils::Exception& e)
    {
        throw utils::Exception(fmt::format("Loading world `{}' failed: {}", worldPath, e.what()));
    }

    wInfo.format("Loaded world `{}'", worldPath);

    std::unique_ptr<ui::WorldSurface> worldSurface;
    const QString worldSurfaceFile = ui::findWorldSurface(worldSurfacePath, worldPath);

    if (worldSurfaceFile.isNull())
    {
        throw utils::Exception(fmt::format("Failed to find world-surface `{}'", worldSurfacePath));
    }

    try
    {
        worldSurface = std::make_unique<ui::WorldSurface>(worldSurfaceFile, world.get());
        worldSurface->activate();
    }
    catch (const utils::Exception& e)
    {
        throw utils::Exception(fmt::format("Loading world-surface `{}' failed: {}", worldSurfacePath, e.what()));
    }

    wInfo.format("Loaded world-surface `{}'", worldSurfacePath);

    return std::make_unique<Context>(std::move(world), std::move(worldSurface));
}

static std::unique_ptr<core::Map> generateBackgroundMap(core::World* world)
{
    std::vector<std::unique_ptr<core::Faction>> players;

    players.reserve(2);
    players.emplace_back(std::make_unique<core::Faction>());
    players.emplace_back(std::make_unique<core::Faction>());

    for (auto& player : players)
    {
        auto conf = core::nextAvailableBannerConfiguration(*world, {});
        player->setBanner(conf.banner);
        player->setPrimaryColor(conf.primaryColor);
        player->setSecondaryColor(conf.secondaryColor);
    }

    return world->getRules()->generateMap(0, 10, std::move(players));
}

#endif
