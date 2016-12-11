/**
 * Copyright (C) 2015-2016 Botond Dénes
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

#include "io/JsonUnserializer.h"
#include "io/File.h"
#include "ui/SearchPaths.h"
#include "ui/UI.h"
#include "utils/Exception.h"
#include "utils/Logging.h"
#include "utils/Settings.h"
#include "warmonger/Context.h"

using namespace warmonger;

int main(int argc, char* argv[])
{
    const QString worldName{argv[1]};
    const QString worldSurfaceName{argv[2]};

    QGuiApplication app(argc, argv);

    utils::initSettings();
    utils::initLogging();

    if (argc < 3)
    {
        wError << "Too few arguments. Usage: warmonger {world} {world-surface}";
        return 1;
    }

    ui::setupSearchPaths();
    ui::initUI();

    std::unique_ptr<core::World> world;
    const QString worldPath = utils::worldPath(worldName);

    try
    {
        io::JsonUnserializer unserializer;

        world = io::readWorld(worldPath, unserializer);
    }
    catch (const utils::Exception& e)
    {
        wFatal << "Loading world " << worldName << " from " << worldPath << " failed: " << e.what();
        return 1;
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
        wFatal << "Loading world-surface " << worldSurfaceName << " from " << worldSurfacePath
               << " failed: " << e.what();
        return 1;
    }

    wInfo << "Loaded world-surface " << worldSurfaceName << " from " << worldSurfacePath;

    QQmlApplicationEngine engine;
    Context* ctx = new Context(std::move(world), std::move(worldSurface), &engine);

    engine.rootContext()->setContextProperty("W", ctx);

    engine.load(QUrl("qrc:/Warmonger.qml"));

    return app.exec();
}
