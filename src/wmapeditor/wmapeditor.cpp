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

#include <QCommandLineParser>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <backward.hpp>

#include "ui/SearchPaths.h"
#include "ui/UI.h"
#include "utils/Logging.h"
#include "utils/Settings.h"
#include "wmapeditor/Context.h"

namespace backward {

backward::SignalHandling sh;

} // namespace backward

using namespace warmonger;

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    bool parserBuilt{true};

    parser.setApplicationDescription("Warmonger map editor");
    parserBuilt &= parser.addOption(QCommandLineOption({"w", "world"}, "The world to use", "world"));
    parser.addPositionalArgument("map", "The map to edit");
    parser.addHelpOption();
    parser.addVersionOption();

    assert(parserBuilt);

    parser.process(app);

    const auto world{parser.value("world")};
    const auto map{parser.value("map")};

    utils::initSettings();
    utils::initLogging();

    ui::setupSearchPaths();
    ui::initUI();

    QQmlApplicationEngine engine;
    wmapeditor::Context ctx;

    if (!world.isEmpty())
        ctx.addWorld(world);

    if (!map.isEmpty())
        ctx.load(map);

    engine.rootContext()->setContextProperty("W", &ctx);

    engine.load(QUrl("qrc:/MapEditor.qml"));

    return app.exec();
}
