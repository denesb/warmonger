/**
 * \copyright (C) 2015-2016 Botond Dénes
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

#include <QObject>

#include "core/ArmyType.h"
#include "core/UnitType.h"
#include "io/Context.h"
#include "test/Util.h"
#include "test/catch.hpp"

using namespace warmonger;

TEST_CASE("QObject", "[Context]")
{
    io::Context ctx;

    QObject o;
    o.setObjectName("objectName");

    ctx.add(&o);

    SECTION("QObject can retrieved")
    {
        QObject* o1 = ctx.get<QObject*>(o.objectName());

        REQUIRE(o1 != nullptr);
        REQUIRE(o.objectName() == o1->objectName());
        REQUIRE(&o == o1);
    }
}

TEST_CASE("Objects derived from QObject", "[Context]")
{
    io::Context ctx;

    core::ArmyType a;
    a.setObjectName("armyType1");
    core::UnitType dt;
    dt.setObjectName("unitType1");

    ctx.add(&a);
    ctx.add(&dt);

    SECTION("ArmyType can retrieved")
    {
        core::ArmyType* a1 = ctx.get<core::ArmyType*>(a.objectName());

        REQUIRE(a1 != nullptr);
        REQUIRE(a.objectName() == a1->objectName());
        REQUIRE(&a == a1);
    }

    SECTION("UnitType can retrieved")
    {
        core::UnitType* dt1 = ctx.get<core::UnitType*>(dt.objectName());

        REQUIRE(dt1 != nullptr);
        REQUIRE(dt.objectName() == dt1->objectName());
        REQUIRE(&dt == dt1);
    }
}

TEST_CASE("Objects with the same name but different type", "[Context]")
{
    io::Context ctx;

    core::ArmyType a;
    a.setObjectName("object1");
    core::UnitType dt;
    dt.setObjectName("object1");

    ctx.add(&a);
    ctx.add(&dt);

    SECTION("ArmyType can retrieved")
    {
        core::ArmyType* a1 = ctx.get<core::ArmyType*>(a.objectName());

        REQUIRE(a1 != nullptr);
        REQUIRE(a.objectName() == a1->objectName());
        REQUIRE(&a == a1);
    }

    SECTION("UnitType can retrieved")
    {
        core::UnitType* dt1 = ctx.get<core::UnitType*>(dt.objectName());

        REQUIRE(dt1 != nullptr);
        REQUIRE(dt.objectName() == dt1->objectName());
        REQUIRE(&dt == dt1);
    }
}

TEST_CASE("Inexistent object", "[Context]")
{
    io::Context ctx;

    core::ArmyType a;
    a.setObjectName("object1");

    SECTION("ArmyType can retrieved")
    {
        core::ArmyType* a1 = ctx.get<core::ArmyType*>(a.objectName());

        REQUIRE(a1 == nullptr);
    }
}

class TestCallback
{
public:
    TestCallback(QObject* obj)
        : obj(obj)
    {
    }

    void operator()(const QString& className, const QString& objectName, io::Context& ctx)
    {
        const QMetaObject* mo = this->obj->metaObject();
        if (className == mo->className() && this->obj->objectName() == objectName)
        {
            ctx.add(this->obj);
        }
    }

private:
    QObject* obj;
};

TEST_CASE("Lookup callback", "[Context]")
{
    core::ArmyType a;
    a.setObjectName("retrievedObject");

    SECTION("ArmyType can retrieved, via callback")
    {
        TestCallback cb(&a);
        io::Context ctx(cb);

        core::ArmyType* a1 = ctx.get<core::ArmyType*>(a.objectName());

        REQUIRE(a1 == &a);
    }
}
