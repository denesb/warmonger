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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <backward.hpp>

#include "io/File.h"
#include "ui/SearchPaths.h"
#include "ui/UI.h"
#include "utils/Exception.h"
#include "utils/Logging.h"
#include "utils/Settings.h"
#include "warmonger/Context.h"

namespace backward {

backward::SignalHandling sh;

} // namespace backward

using namespace warmonger;

static std::unique_ptr<Context> createContext(const QString& worldName, const QString& worldSurfacePath);
static std::unique_ptr<core::Map> generateBackgroundMap(core::World* world);

int main(int argc, char* argv[])
{
    const QString worldPath{argv[1]};
    const QString worldSurfacePath{argv[2]};

    QGuiApplication app(argc, argv);

    utils::initSettings();
    utils::initLogging();

    if (argc < 3)
    {
        std::cout << "Too few arguments. Usage: warmonger {world} {world-surface}" << std::endl;
        return 1;
    }

    ui::setupSearchPaths();
    ui::initUI();

    std::unique_ptr<Context> ctx;

    try
    {
        ctx = createContext(worldPath, worldSurfacePath);
    }
    catch (const utils::Exception& e)
    {
        wError.format("Failed to create context: {}", e.what());
        return 1;
    }

    std::unique_ptr<core::Map> backgroundMap = generateBackgroundMap(ctx->getWorld());

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("W", ctx.get());
    engine.rootContext()->setContextProperty("backgroundMap", backgroundMap.get());

    engine.load(QUrl("qrc:/Warmonger.qml"));

    return app.exec();
}

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
    return world->getRules()->generateMap(10, 0);
}
