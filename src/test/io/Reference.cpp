/**
 * \copyright (C) 2015-2017 Botond Dénes
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
#include "io/Reference.h"

using namespace warmonger;

TEST_CASE("serializeReference()", "[io][Serialize]")
{
    core::WObject obj1(nullptr);
    REQUIRE(io::serializeReference(&obj1) == "warmonger::core::WObject#-1");

    core::World w("uuid0");

    core::WObject* obj2 = new core::WObject(&w);
    REQUIRE(io::serializeReference(obj2) == "warmonger::core::World/warmonger::core::WObject#1000");

    core::WObject* obj3 = new core::WObject(obj2);
    REQUIRE(io::serializeReference(obj3) == "warmonger::core::World/warmonger::core::WObject#1001");
}

TEST_CASE("unserializeReference(core::World*)", "[io][Unserialize]")
{
    core::World* nullWorld{nullptr};
    REQUIRE(io::unserializeReference("warmonger::core::World/warmonger::core::WObject#1000", nullWorld) == nullptr);

    core::World w("uuid0");

    REQUIRE(io::unserializeReference("warmonger::core::World/warmonger::core::WObject#-1", &w) == nullptr);
    REQUIRE(io::unserializeReference("warmonger::core::World/warmonger::core::WObject#1000", &w) == nullptr);

    core::WObject* obj0 = new core::WObject(&w);

    REQUIRE(io::unserializeReference("warmonger::core::World/warmonger::core::WObject#1000", &w) == obj0);

    core::WObject* obj1 = new core::WObject(&w);

    REQUIRE(io::unserializeReference("warmonger::core::World/warmonger::core::WObject#1000", &w) == obj0);
    REQUIRE(io::unserializeReference("warmonger::core::World/warmonger::core::WObject#1001", &w) == obj1);

    core::WObject* obj2 = new core::WObject(obj0);

    REQUIRE(io::unserializeReference("warmonger::core::World/warmonger::core::WObject#1000", &w) == obj0);
    REQUIRE(io::unserializeReference("warmonger::core::World/warmonger::core::WObject#1001", &w) == obj1);
    REQUIRE(io::unserializeReference("warmonger::core::World/warmonger::core::WObject#1002", &w) == obj2);

    core::Civilization* obj3 = new core::Civilization(obj0);

    REQUIRE(io::unserializeReference("warmonger::core::World/warmonger::core::Civilization#1003", &w) == obj3);
    REQUIRE(io::unserializeReference("warmonger::core::World/warmonger::core::WObject#1003", &w) == nullptr);

    // malformed references
    REQUIRE(io::unserializeReference("warmonger::core::WObject", &w) == nullptr);
    REQUIRE(io::unserializeReference("warmonger::core::WObject#1000", &w) == nullptr);
    REQUIRE(io::unserializeReference("", &w) == nullptr);
    REQUIRE(io::unserializeReference("warmonger::core::World/ab/warmonger::core::WObject#1000", &w) == nullptr);
    REQUIRE(io::unserializeReference("warmonger::core::WObject#sdasd#1000", &w) == nullptr);
    REQUIRE(io::unserializeReference("3#warmonger::core::WObject#1000", &w) == nullptr);

    // other parent specified
    REQUIRE(io::unserializeReference("warmonger::core::Map/warmonger::core::WObject#1000", &w) == nullptr);
    REQUIRE(io::unserializeReference("warmonger::core::Map/warmonger::core::WObject#1001", &w) == nullptr);
    REQUIRE(io::unserializeReference("warmonger::core::Map/warmonger::core::WObject#1002", &w) == nullptr);

    // reference has the right id but wrong type
    REQUIRE(io::unserializeReference("warmonger::core::Map/warmonger::core::Civilization#1000", &w) == nullptr);
    REQUIRE(io::unserializeReference("warmonger::core::Map/warmonger::core::Civilization#1001", &w) == nullptr);
    REQUIRE(io::unserializeReference("warmonger::core::Map/warmonger::core::Civilization#1002", &w) == nullptr);
}

TEST_CASE("unserializeReference(core::Map*)", "[io][Unserialize]")
{
    core::Map* nullMap{nullptr};
    REQUIRE(io::unserializeReference("warmonger::core::World/warmonger::core::WObject#1000", nullMap) == nullptr);

    core::World w("uuid0");
    core::Map m;

    m.setWorld(&w);

    REQUIRE(io::unserializeReference("warmonger::core::Map/warmonger::core::WObject#-1", &m) == nullptr);

    core::WObject* obj0 = new core::WObject(&m);
    REQUIRE(io::unserializeReference("warmonger::core::Map/warmonger::core::WObject#1000", &m) == obj0);

    core::WObject* obj1 = new core::WObject(&w);
    REQUIRE(io::unserializeReference("warmonger::core::World/warmonger::core::WObject#1000", &m) == obj1);

    core::WObject* obj2 = new core::Faction(&m);
    REQUIRE(io::unserializeReference("warmonger::core::Map/warmonger::core::Faction#1001", &m) == obj2);
    REQUIRE(io::unserializeReference("warmonger::core::Map/warmonger::core::WObject#1001", &m) == nullptr);

    core::WObject* obj3 = new core::Civilization(&w);
    REQUIRE(io::unserializeReference("warmonger::core::World/warmonger::core::Civilization#1001", &m) == obj3);
    REQUIRE(io::unserializeReference("warmonger::core::World/warmonger::core::WObject#1001", &m) == nullptr);

    // unknown parent specified
    REQUIRE(io::unserializeReference("warmonger::core::ASDASD/warmonger::core::WObject#1000", &m) == nullptr);

    // no parent specified
    REQUIRE(io::unserializeReference("warmonger::core::WObject#1000", &m) == nullptr);

    // malformed references
    REQUIRE(io::unserializeReference("warmonger::core::WObject", &m) == nullptr);
    REQUIRE(io::unserializeReference("warmonger::core::WObject#1000", &m) == nullptr);
    REQUIRE(io::unserializeReference("", &m) == nullptr);
    REQUIRE(io::unserializeReference("warmonger::core::World/ab/warmonger::core::WObject#1000", &m) == nullptr);
    REQUIRE(io::unserializeReference("warmonger::core::WObject#sdasd#1000", &m) == nullptr);
    REQUIRE(io::unserializeReference("3#warmonger::core::WObject#1000", &m) == nullptr);
}
