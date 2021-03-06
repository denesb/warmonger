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

#include "test/WObject.h"

using namespace warmonger;

TEST_CASE("getObjectTreeRoot()", "[WObject]")
{
    QObject root;

    REQUIRE(core::getObjectTreeRoot(nullptr) == nullptr);

    auto obj0 = new TestWObject2(&root);
    REQUIRE(core::getObjectTreeRoot(obj0) == &root);

    auto obj1 = new TestWObject1(obj0);
    REQUIRE(core::getObjectTreeRoot(obj1) == &root);

    TestWObject1 wobj0(nullptr);
    REQUIRE(core::getObjectTreeRoot(&wobj0) == nullptr);

    TestWObject1* wobj1 = new TestWObject1(&wobj0);
    REQUIRE(core::getObjectTreeRoot(wobj1) == nullptr);
}

TEST_CASE("Children ids", "[WObject]")
{
    QObject root;

    auto obj0 = new TestWObject1(&root);
    REQUIRE(obj0->getId() == core::ObjectId(0));

    auto obj1 = new TestWObject1(&root);
    REQUIRE(obj1->getId() == core::ObjectId(1));

    auto obj2 = new TestWObject1(&root);
    REQUIRE(obj2->getId() == core::ObjectId(2));

    delete obj1;

    auto obj3 = new TestWObject1(&root);
    REQUIRE(obj3->getId() == core::ObjectId(3));

    auto obj4 = new TestWObject2(&root);
    REQUIRE(obj4->getId() == core::ObjectId(4));

    auto obj5 = new TestWObject2(&root);
    REQUIRE(obj5->getId() == core::ObjectId(5));

    auto obj6 = new TestWObject2(obj2);
    REQUIRE(obj6->getId() == core::ObjectId(6));

    auto obj7 = new TestWObject2(obj6);
    REQUIRE(obj7->getId() == core::ObjectId(7));
}
