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

#include "ui/CampaignMapEditor.h"
#include "test/catch.hpp"

using namespace warmonger;

TEST_CASE("Invokable map-editing members", "[CampaignMapEditor]")
{
    core::World world;

    auto civ{std::make_unique<core::Civilization>()};
    world.addCivilization(std::move(civ));

    auto banner{std::make_unique<core::Banner>()};
    world.addBanner(std::move(banner));
    world.setColors({QColor("black"), QColor("white"), QColor("gray")});

    core::CampaignMap map;
    map.setWorld(&world);

    ui::WorldSurface surface("./worldsurface-packages/test.wsp", &world);

    ui::CampaignMapEditor mapEditor;
    mapEditor.setCampaignMap(&map);
    mapEditor.setWorldSurface(&surface);

    SECTION("setNumberOfFactions")
    {
        REQUIRE(map.getFactions().empty());

        mapEditor.setNumberOfFactions(2);

        REQUIRE(map.getFactions().size() == 2);

        mapEditor.setNumberOfFactions(1);

        REQUIRE(map.getFactions().size() == 1);
    }
}
