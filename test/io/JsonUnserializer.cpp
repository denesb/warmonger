#include <QJsonDocument>
#include <QJsonObject>

#include "core/Armor.h"
#include "core/DamageType.h"
#include "io/Context.h"
#include "io/JsonUnserializer.h"
#include "test/catch.hpp"

using namespace warmonger;

TEST_CASE("Armor can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    QJsonObject jobj;
    jobj["objectName"] = "armor1";
    jobj["displayName"] = "Armor 1";

    core::DamageType dt;
    dt.setObjectName("damageType1");

    ctx.add(&dt);

    QJsonObject jdefenses;
    jdefenses[dt.objectName()] = 12;
    jobj["defenses"] = jdefenses;

    SECTION("unserializing DamageType")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const core::Armor *a = unserializer.unserializeArmor(jdoc.toJson());

        REQUIRE(a->objectName() == jobj["objectName"].toString());
        REQUIRE(a->getDisplayName() == jobj["displayName"].toString());

        for (const QString key : jdefenses.keys())
        {
            core::DamageType *dt = ctx.get<core::DamageType *>(key);
            REQUIRE(a->getDefense(dt) == jdefenses[key].toInt());
        }
    }
}

TEST_CASE("DamageType can be unserialized from JSON", "[JsonUnserializer]")
{
    io::Context ctx;

    QJsonObject jobj;
    jobj["objectName"] = "damageType1";
    jobj["displayName"] = "DamageType 1";

    SECTION("unserializing DamageType")
    {
        io::JsonUnserializer unserializer(ctx);
        QJsonDocument jdoc(jobj);
        const core::DamageType *dt = unserializer.unserializeDamageType(jdoc.toJson());

        REQUIRE(dt->objectName() == jobj["objectName"].toString());
        REQUIRE(dt->getDisplayName() == jobj["displayName"].toString());
    }
}
