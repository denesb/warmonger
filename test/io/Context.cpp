#include <QObject>

#include "core/Armor.h"
#include "core/DamageType.h"
#include "io/Context.h"
#include "test/catch.hpp"
#include "test/Util.h"

using namespace warmonger;

TEST_CASE("QObject", "[Context]")
{
    io::Context ctx;

    QObject o;
    o.setObjectName("objectName");

    ctx.add(&o);

    SECTION("QObject can retrieved")
    {
        QObject *o1 = ctx.get<QObject *>(o.objectName());

        REQUIRE(o1 != nullptr);
        REQUIRE(o.objectName() == o1->objectName());
        REQUIRE(&o == o1);
    }
}

TEST_CASE("Objects derived from QObject", "[Context]")
{
    io::Context ctx;

    core::Armor a;
    a.setObjectName("armor1");
    core::DamageType dt;
    dt.setObjectName("damageType1");

    ctx.add(&a);
    ctx.add(&dt);

    SECTION("Armor can retrieved")
    {
        core::Armor *a1 = ctx.get<core::Armor *>(a.objectName());

        REQUIRE(a1 != nullptr);
        REQUIRE(a.objectName() == a1->objectName());
        REQUIRE(&a == a1);
    }

    SECTION("DamageType can retrieved")
    {
        core::DamageType *dt1 = ctx.get<core::DamageType *>(dt.objectName());

        REQUIRE(dt1 != nullptr);
        REQUIRE(dt.objectName() == dt1->objectName());
        REQUIRE(&dt == dt1);
    }
}

TEST_CASE("Objects with the same name but different type", "[Context]")
{
    io::Context ctx;

    core::Armor a;
    a.setObjectName("object1");
    core::DamageType dt;
    dt.setObjectName("object1");

    ctx.add(&a);
    ctx.add(&dt);

    SECTION("Armor can retrieved")
    {
        core::Armor *a1 = ctx.get<core::Armor *>(a.objectName());

        REQUIRE(a1 != nullptr);
        REQUIRE(a.objectName() == a1->objectName());
        REQUIRE(&a == a1);
    }

    SECTION("DamageType can retrieved")
    {
        core::DamageType *dt1 = ctx.get<core::DamageType *>(dt.objectName());

        REQUIRE(dt1 != nullptr);
        REQUIRE(dt.objectName() == dt1->objectName());
        REQUIRE(&dt == dt1);
    }
}

TEST_CASE("Inexistent object", "[Context]")
{
    io::Context ctx;

    core::Armor a;
    a.setObjectName("object1");

    SECTION("Armor can retrieved")
    {
        core::Armor *a1 = ctx.get<core::Armor *>(a.objectName());

        REQUIRE(a1 == nullptr);
    }
}
