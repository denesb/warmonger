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

#include <iostream>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>

#include "ui/SearchPaths.h"
#include "ui/UI.h"
#include "utils/Constants.h"
#include "utils/Logging.h"
#include "utils/Settings.h"
#include "warmonger/Context.h"

using namespace warmonger;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Too few arguments." << std::endl;
        std::cerr << "Usage: warmonger {world} {world-surface}" << std::endl;
        return 1;
    }

    QGuiApplication app(argc, argv);

    utils::initSettings();
    utils::initLogging();

    ui::setupSearchPaths();
    ui::initUI();

    //const char* const applicationName = utils::applicationName.toStdString().c_str();

    //qmlRegisterType<Context>(applicationName, 1, 0, "Context");

    QQmlApplicationEngine engine;
    Context* ctx = new Context(nullptr, nullptr, &engine);

    engine.rootContext()->setContextProperty("W", ctx);

    engine.load(QUrl("qrc:/Warmonger.qml"));

    return app.exec();
}
