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

#include <catch.hpp>

#include "core/Map.h"
#include "io/JsonSerializer.h"

using namespace warmonger;

TEST_CASE("World serialized and unserilized", "[Serializer]")
{
    core::World w("uuid", core::WorldRules::Type::Lua);
    w.setName("Brave new world");
    w.setRulesEntryPoint("/home/warmonger/rules/script.lua");

    const io::JsonSerializer serializer;

    auto json = serializer.serialize(w.serialize());

    core::World newWorld(serializer.unserialize(json));

    REQUIRE(newWorld.getUuid() == w.getUuid());
    REQUIRE(newWorld.getName() == w.getName());
    REQUIRE(newWorld.getRulesType() == w.getRulesType());
    REQUIRE(newWorld.getRulesEntryPoint() == w.getRulesEntryPoint());
}

TEST_CASE("Map serialized and unserilized", "[Serializer]")
{
    core::World w("uuid", core::WorldRules::Type::Lua);
    w.setName("Brave new world");
    w.setRulesEntryPoint("/home/warmonger/rules/script.lua");
    auto* b0 = w.createBanner("Striped");
    auto* b1 = w.createBanner("Medusa");
    auto* col0 = w.createColor("Red");
    auto* col1 = w.createColor("Blue");
    auto* civ0 = w.createCivilization("Persians");
    auto* civ1 = w.createCivilization("Greeks");

    core::Map m;

    m.setName("The undiscovered map");
    m.setWorld(&w);
    m.generateMapNodes(8);

    auto* f0 = m.createFaction();
    f0->setName("The Achmeid Empire");
    f0->setCivilization(civ0);
    f0->setBanner(b0);
    f0->setPrimaryColor(col0);
    f0->setSecondaryColor(col1);

    auto* f1 = m.createFaction();
    f1->setName("Sparta");
    f1->setCivilization(civ1);
    f1->setBanner(b1);
    f1->setPrimaryColor(col1);
    f1->setSecondaryColor(col0);

    const io::JsonSerializer serializer;

    auto json = serializer.serialize(m.serialize());

    core::Map newMap(serializer.unserialize(json), w, nullptr);

    REQUIRE(newMap.getWorld() == &w);
    REQUIRE(newMap.getName() == m.getName());

    // MapNodes
    REQUIRE(newMap.getMapNodes().size() == m.getMapNodes().size());
    for (unsigned i = 0; i < newMap.getMapNodes().size(); ++i)
    {
        auto* oldMn = m.getMapNodes().at(i);
        auto* newMn = newMap.getMapNodes().at(i);

        REQUIRE(newMn->getId() == oldMn->getId());

        for (const auto& d : core::directions)
        {
            if (oldMn->getNeighbour(d))
            {
                REQUIRE(newMn->getNeighbour(d)->getId() == oldMn->getNeighbour(d)->getId());
            }
            else
            {
                REQUIRE(newMn->getNeighbour(d) == oldMn->getNeighbour(d));
            }
        }
    }

    // Factions
    REQUIRE(newMap.getFactions().size() == m.getFactions().size());
    for (unsigned i = 0; i < newMap.getFactions().size(); ++i)
    {
        auto* oldF = m.getFactions().at(i);
        auto* newF = newMap.getFactions().at(i);

        REQUIRE(newF->getId() == oldF->getId());
        REQUIRE(newF->getName() == oldF->getName());

        if (oldF->getBanner())
            REQUIRE(newF->getBanner()->getName() == oldF->getBanner()->getName());
        if (oldF->getCivilization())
            REQUIRE(newF->getCivilization()->getName() == oldF->getCivilization()->getName());
        if (oldF->getPrimaryColor())
            REQUIRE(newF->getPrimaryColor()->getName() == oldF->getPrimaryColor()->getName());
        if (oldF->getSecondaryColor())
            REQUIRE(newF->getSecondaryColor()->getName() == oldF->getSecondaryColor()->getName());
    }
}
