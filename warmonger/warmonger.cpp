/**
 * Copyright (C) 2015-2017 Botond Dénes
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

#include "io/File.h"
#include "io/SanityCheck.h"
#include "ui/SearchPaths.h"
#include "ui/UI.h"
#include "utils/Exception.h"
#include "utils/Logging.h"
#include "utils/Settings.h"
#include "warmonger/Context.h"

using namespace warmonger;

static std::unique_ptr<Context> createContext(const QString& worldName, const QString& worldSurfaceName);
static std::unique_ptr<core::Map> generateBackgroundMap(core::World* world);

int main(int argc, char* argv[])
{
    const QString worldName{argv[1]};
    const QString worldSurfaceName{argv[2]};

    QGuiApplication app(argc, argv);

    utils::initSettings();
    utils::initLogging();

    if (argc < 3)
    {
        wFatal << "Too few arguments. Usage: warmonger {world} {world-surface}";
        return 1;
    }

    ui::setupSearchPaths();
    ui::initUI();

    std::unique_ptr<Context> ctx;

    try
    {
        ctx = createContext(worldName, worldSurfaceName);
    }
    catch (const utils::Exception& e)
    {
        wFatal << "Failed to create context: " << e.what();
        return 1;
    }

    std::unique_ptr<core::Map> backgroundMap = generateBackgroundMap(ctx->getWorld());

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("W", ctx.get());
    engine.rootContext()->setContextProperty("backgroundMap", backgroundMap.get());

    engine.load(QUrl("qrc:/Warmonger.qml"));

    return app.exec();
}

static std::unique_ptr<Context> createContext(const QString& worldName, const QString& worldSurfaceName)
{
    std::unique_ptr<core::World> world;
    const QString worldPath = utils::worldPath(worldName);

    try
    {
        world = io::readWorld(worldPath);
    }
    catch (const utils::Exception& e)
    {
        throw utils::Exception(
            utils::MsgBuilder() << "Loading world " << worldName << " from " << worldPath << " failed: " << e.what());
    }

    if (!io::isWorldSane(*world.get()))
    {
        throw utils::Exception(utils::MsgBuilder() << "Loaded world " << *world.get() << "is not sane");
    }

    wInfo << "Loaded world " << worldName << " from " << worldPath;

    std::unique_ptr<ui::WorldSurface> worldSurface;
    const QString worldSurfacePath = utils::worldSurfacePath(worldName, worldSurfaceName);

    try
    {
        worldSurface = std::make_unique<ui::WorldSurface>(worldSurfacePath, world.get());
        worldSurface->activate();
    }
    catch (const utils::Exception& e)
    {
        throw utils::Exception(utils::MsgBuilder() << "Loading world-surface " << worldSurfaceName << " from "
                                                   << worldSurfacePath
                                                   << " failed: "
                                                   << e.what());
    }

    wInfo << "Loaded world-surface " << worldSurfaceName << " from " << worldSurfacePath;

    return std::make_unique<Context>(std::move(world), std::move(worldSurface));
}

static std::unique_ptr<core::Map> generateBackgroundMap(core::World* world)
{
    std::unique_ptr<core::Map> map = std::make_unique<core::Map>();
    map->setWorld(world);

    // TODO: fix map generation, call into the world rules to generate the map

    return map;
}
