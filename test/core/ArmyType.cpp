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

#include "core/ArmyType.h"
#include "test/catch.hpp"
#include "utils/Exception.h"

using namespace warmonger;

TEST_CASE("ArmyType hierarchy and inheritance", "[ArmyType]")
{
    SECTION("root's getters throw")
    {
        core::ArmyType root;

        REQUIRE_THROWS_AS(root.getDisplayName(), utils::ValueError);
    }

    SECTION("getters return own values when they are set")
    {
        core::ArmyType root;

        root.setDisplayName("displayName");

        REQUIRE(root.getDisplayName() == "displayName");
    }

    SECTION("getters return parent's values when values are not set but has parent")
    {
        core::ArmyType parent;
        core::ArmyType child;

        child.setHierarchyParent(&parent);

        parent.setDisplayName("displayName");

        REQUIRE(child.getDisplayName() == "displayName");
    }
}
