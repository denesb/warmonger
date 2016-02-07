#include <memory>
#include <string>
#include <ostream>

#include <QJsonDocument>

#include "test/catch.hpp"

#include "core/Armor.h"
#include "core/DamageType.h"
#include "core/TerrainType.h"
#include "core/UnitClass.h"
#include "core/UnitLevel.h"
#include "io/JsonSerializer.h"

using namespace warmonger;

std::ostream &operator<<(std::ostream &os, const QString &s)
{
    os << s.toStdString();
    return os;
}

template <typename T>
QJsonObject serialize(T obj)
{
    std::unique_ptr<io::Serializer> serializer(new io::JsonSerializer());
    QByteArray json(serializer->serialize(obj));

    QJsonDocument jdoc(QJsonDocument::fromJson(json));
    return jdoc.object();
}

TEST_CASE("Armor can be serialized to JSON", "[JsonSerializer]")
{
    core::Armor a(nullptr);

    const QString aObjectName{"armor1"};
    const QString aDisplayName{"Armor 1"};

    a.setObjectName(aObjectName);
    a.setDisplayName(aDisplayName);

    const QString dtObjectName{"damageType1"};
    const QString dtDisplayName{"DamageType 1"};

    const int dtDefense{10};

    core::DamageType dt(nullptr);
    dt.setObjectName(dtObjectName);
    dt.setDisplayName(dtDisplayName);

    a.setDefense(&dt, dtDefense);

    SECTION("serializing Armor")
    {
        QJsonObject jobj(serialize(&a));

        REQUIRE(jobj["objectName"].toString() == aObjectName);
        REQUIRE(jobj["displayName"].toString() == aDisplayName);
        REQUIRE(jobj["defenses"].isObject() == true);

        QJsonObject defenses(jobj["defenses"].toObject());
        REQUIRE(defenses.size() == 1);
        REQUIRE(defenses[dtObjectName] == dtDefense);
    }
}

TEST_CASE("DamageType can be serialized to JSON", "[JsonSerializer]")
{
    core::DamageType dt(nullptr);

    const QString objectName{"damageType1"};
    const QString displayName{"DamageType 1"};

    dt.setObjectName(objectName);
    dt.setDisplayName(displayName);

    SECTION("serializing DamageType")
    {
        QJsonObject jobj(serialize(&dt));

        REQUIRE(jobj["objectName"] == objectName);
        REQUIRE(jobj["displayName"] == displayName);
    }
}

TEST_CASE("TerrainType can be serialized to JSON", "[JsonSerializer]")
{
    core::TerrainType tt(nullptr);

    const QString ttObjectName{"terrainType1"};
    const QString ttDisplayName{"TerrainType 1"};

    tt.setObjectName(ttObjectName);
    tt.setDisplayName(ttDisplayName);

    SECTION("serializing TerrainType")
    {
        QJsonObject jobj(serialize(&tt));

        REQUIRE(jobj["objectName"] == ttObjectName);
        REQUIRE(jobj["displayName"] == ttDisplayName);
    }
}

TEST_CASE("UnitClass can be serialized to JSON", "[JsonSerializer]")
{
    core::UnitClass uc(nullptr);

    const QString ucObjectName{"unitClass1"};
    const QString ucDisplayName{"UnitClass 1"};
    const int ucMP{16};

    uc.setObjectName(ucObjectName);
    uc.setDisplayName(ucDisplayName);
    uc.setMovementPoints(ucMP);

    core::TerrainType tt(nullptr);
    const QString ttObjectName{"terrainType1"};
    tt.setObjectName(ttObjectName);

    const int ucMC{4};
    const int ucA{5};
    const int ucD{6};

    uc.setMovementCost(&tt, ucMC);
    uc.setAttack(&tt, ucA);
    uc.setDefense(&tt, ucD);

    SECTION("serializing UnitClass")
    {
        QJsonObject jobj(serialize(&uc));

        REQUIRE(jobj["objectName"] == ucObjectName);
        REQUIRE(jobj["displayName"] == ucDisplayName);
        REQUIRE(jobj["movementPoints"] == ucMP);

        QJsonObject mc(jobj["movementCosts"].toObject());
        REQUIRE(mc.size() == 1);
        REQUIRE(mc[ttObjectName] == ucMC);

        QJsonObject attacks(jobj["attacks"].toObject());
        REQUIRE(attacks.size() == 1);
        REQUIRE(attacks[ttObjectName] == ucA);

        QJsonObject defenses(jobj["defenses"].toObject());
        REQUIRE(defenses.size() == 1);
        REQUIRE(defenses[ttObjectName] == ucD);
    }
}

TEST_CASE("UnitLevel can be serialized to JSON", "[JsonSerializer]")
{
    core::UnitLevel ul(nullptr);

    const QString ulObjectName{"unitLevel1"};
    const QString ulDisplayName{"UnitLevel 1"};
    const int ulIndex{1};
    const int ulXP{100};

    ul.setObjectName(ulObjectName);
    ul.setDisplayName(ulDisplayName);
    ul.setIndex(ulIndex);
    ul.setExperiencePoints(ulXP);

    SECTION("serializing UnitLevel")
    {
        QJsonObject jobj(serialize(&ul));

        REQUIRE(jobj["objectName"] == ulObjectName);
        REQUIRE(jobj["displayName"] == ulDisplayName);
        REQUIRE(jobj["index"] == ulIndex);
        REQUIRE(jobj["experiencePoints"] == ulXP);
    }
}
