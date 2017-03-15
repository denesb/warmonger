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

#include "io/CampaignMapJsonSerializer.h"
#include "io/File.h"
#include "io/WorldJsonSerializer.h"
#include "test/Util.h"
#include "test/catch.hpp"

void createWorldFile(const QString& path)
{
    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    io::WorldJsonSerializer serializer;

    QFile file(path);
    file.open(QIODevice::WriteOnly);

    file.write(serializer.serializeWorld(world.get()));
}

void createCampaignMapFile(const QString& path)
{
    const std::pair<core::CampaignMap*, QJsonObject> campaignMaps = makeMap();
    const std::unique_ptr<core::CampaignMap> campaignMap{campaignMaps.first};

    io::CampaignMapJsonSerializer serializer;

    QFile file(path);
    file.open(QIODevice::WriteOnly);

    file.write(serializer.serializeCampaignMap(campaignMap.get()));
}

TEST_CASE("World can be written to file", "[File]")
{
    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    const QString path("./write_world.json");

    SECTION("writing World")
    {
        io::writeWorld(world.get(), path);

        QFile file(path);
        REQUIRE(file.exists() == true);

        file.remove();
    }
}

TEST_CASE("World can be read from file", "[File]")
{
    const QString path("./read_world.json");
    createWorldFile(path);

    SECTION("reading World")
    {
        std::unique_ptr<core::World> world = io::readWorld(path);

        REQUIRE(world);
    }

    QFile file(path);
    file.remove();
}

TEST_CASE("Map can be written to file", "[File]")
{
    const std::pair<core::CampaignMap*, QJsonObject> campaignMaps = makeMap();
    const std::unique_ptr<core::CampaignMap> campaignMap{campaignMaps.first};

    const QString path("./write_campaignmap.json");

    SECTION("writing Map")
    {
        io::writeCampaignMap(campaignMap.get(), path);

        QFile file(path);
        REQUIRE(file.exists() == true);

        file.remove();
    }
}

TEST_CASE("Map can be read from file", "[File]")
{
    const QString path("./read_campaignmap.json");

    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    core::World* world{worlds.first};

    createCampaignMapFile(path);

    SECTION("reading CampaignMap")
    {
        std::unique_ptr<core::CampaignMap> campaignMap = io::readCampaignMap(path, world);

        REQUIRE(campaignMap);
        REQUIRE(campaignMap->getWorld() != nullptr);
    }

    QFile file(path);
    file.remove();
}
