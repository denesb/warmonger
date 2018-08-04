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

    auto* e0 = m.createEntity("entity0");
    auto* e0comp0 = dynamic_cast<core::PositionComponent*>(e0->createComponent(core::PositionComponent::name));
    e0comp0->setMapNode(m.getMapNodes().front());
    auto* e0comp1 = dynamic_cast<core::GraphicsComponent*>(e0->createComponent(core::GraphicsComponent::name));
    e0comp1->setPath("terrain_image.png");
    e0comp1->setX(59);
    e0comp1->setY(67);
    e0comp1->setZ(70);

    auto* e1 = m.createEntity("entity1");
    e1->setParentEntity(e0);
    auto* e1comp0 = dynamic_cast<core::PositionComponent*>(e1->createComponent(core::PositionComponent::name));
    e1comp0->setMapNode(m.getMapNodes().back());
    auto* e1comp1 = dynamic_cast<core::GraphicsComponent*>(e1->createComponent(core::GraphicsComponent::name));
    e1comp1->setPath("soldier_image.png");
    e1comp1->setX(2);
    e1comp1->setY(3);
    e1comp1->setZ(100);

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

    // Entities
    REQUIRE(newMap.getEntities().size() == m.getEntities().size());

    // Entity 0
    auto newE0 = newMap.getEntities().front();
    REQUIRE(newE0->getId() == e0->getId());
    REQUIRE(newE0->getParentEntity() == nullptr);
    auto newE0comp0 = newE0->getPositionComponent();
    REQUIRE(newE0comp0->getMapNode()->getId() == e0comp0->getMapNode()->getId());
    auto newE0comp1 = newE0->getGraphicsComponent();
    REQUIRE(newE0comp1->getPath() == e0comp1->getPath());
    REQUIRE(newE0comp1->getX() == e0comp1->getX());
    REQUIRE(newE0comp1->getY() == e0comp1->getY());
    REQUIRE(newE0comp1->getZ() == e0comp1->getZ());

    // Entity 1
    auto newE1 = newMap.getEntities().back();
    REQUIRE(newE1->getId() == e1->getId());
    REQUIRE(newE1->getParentEntity() != nullptr);
    REQUIRE(newE1->getParentEntity()->getId() == newE0->getId());
    auto newE1comp0 = newE1->getPositionComponent();
    REQUIRE(newE1comp0->getMapNode()->getId() == e1comp0->getMapNode()->getId());
    auto newE1comp1 = newE1->getGraphicsComponent();
    REQUIRE(newE1comp1->getPath() == e1comp1->getPath());
    REQUIRE(newE1comp1->getX() == e1comp1->getX());
    REQUIRE(newE1comp1->getY() == e1comp1->getY());
    REQUIRE(newE1comp1->getZ() == e1comp1->getZ());

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
