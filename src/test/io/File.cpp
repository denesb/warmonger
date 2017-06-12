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

#include <QFile>
#include <catch.hpp>

#include "io/File.h"
#include "io/MapJsonSerializer.h"
#include "io/WorldJsonSerializer.h"
#include "test/Util.h"

void createWorldFile(const QString& path)
{
    const auto worlds = makeWorld();
    const auto world = worlds.first.get();

    io::WorldJsonSerializer serializer;

    QFile file(path);
    file.open(QIODevice::WriteOnly);

    file.write(serializer.serializeWorld(world));

    QFile rulesFile(world->getRulesEntryPoint());
    rulesFile.open(QIODevice::WriteOnly);

    rulesFile.write("function init(W) w_debug(\"init\"); end");
}

void createMapFile(const QString& path)
{
    const auto maps = makeMap();
    const auto map{maps.first.get()};

    io::MapJsonSerializer serializer;

    QFile file(path);
    file.open(QIODevice::WriteOnly);

    file.write(serializer.serializeMap(map));
}

TEST_CASE("World can be written to file", "[File]")
{
    const auto worlds = makeWorld();
    const auto world = worlds.first.get();

    const QString path("./write_world.wwd");

    SECTION("writing World")
    {
        REQUIRE_NOTHROW(io::writeWorld(world, path));

        QFile file(path);
        REQUIRE(file.exists() == true);

        file.remove();
    }
}

TEST_CASE("World can be read from file", "[File]")
{
    const QString path("./read_world.wwd");
    createWorldFile(path);

    SECTION("reading World")
    {
        REQUIRE_NOTHROW(io::readWorld(path));
        REQUIRE(io::readWorld(path));
    }

    QFile file(path);
    file.remove();
}

TEST_CASE("Map can be written to file", "[File]")
{
    const auto maps = makeMap();
    const auto map{maps.first.get()};

    const QString path("./write_map.wmd");

    SECTION("writing Map")
    {
        REQUIRE_NOTHROW(io::writeMap(map, path));

        QFile file(path);
        REQUIRE(file.exists() == true);

        file.remove();
    }
}

TEST_CASE("Map can be read from file", "[File]")
{
    const QString path("./read_map.wmd");

    const auto worlds = makeWorld();
    const auto world = worlds.first.get();

    createMapFile(path);

    SECTION("reading Map")
    {
        REQUIRE_NOTHROW(io::readMap(path, world));
        REQUIRE(io::readMap(path, world));
    }

    QFile file(path);
    file.remove();
}
