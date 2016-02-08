#include "io/JsonSerializer.h"
#include "io/QJsonUtil.h"

#include "core/Armor.h"
#include "core/DamageType.h"
#include "core/Faction.h"
#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/UnitClass.h"
#include "core/UnitLevel.h"
#include "core/UnitType.h"
#include "core/Weapon.h"

using namespace warmonger;
using namespace warmonger::io;

JsonSerializer::JsonSerializer(QJsonDocument::JsonFormat format) :
    format(format)
{
}

QByteArray JsonSerializer::serialize(const core::Armor *obj)
{
    QJsonDocument jdoc(this->toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::DamageType *obj)
{
    QJsonDocument jdoc(this->toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::Faction *obj)
{
    QJsonDocument jdoc(this->toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::Game *obj)
{
}

QByteArray JsonSerializer::serialize(const core::Map *obj)
{
}

QByteArray JsonSerializer::serialize(const core::MapNode *obj)
{
}

QByteArray JsonSerializer::serialize(const core::Player *obj)
{
}

QByteArray JsonSerializer::serialize(const core::Settlement *obj)
{
}

QByteArray JsonSerializer::serialize(const core::SettlementType *obj)
{
    QJsonDocument jdoc(this->toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::TerrainType *obj)
{
    QJsonDocument jdoc(this->toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::Unit *obj)
{
}

QByteArray JsonSerializer::serialize(const core::UnitClass *obj)
{
    QJsonDocument jdoc(this->toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::UnitLevel *obj)
{
    QJsonDocument jdoc(this->toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::UnitType *obj)
{
    QJsonDocument jdoc(this->toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::Weapon *obj)
{
    QJsonDocument jdoc(this->toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::World *obj)
{
}

QByteArray JsonSerializer::serialize(const core::WorldSurface *obj)
{
}

QJsonObject JsonSerializer::toJson(const core::Armor *obj)
{
    QJsonObject jobj(this->gameObjectToJson(obj));

    jobj["defenses"] = toQJsonObject(
        obj->getDefenses(),
        qObjectName,
        constructQJsonValue<int>
    );

    return jobj;
}

QJsonObject JsonSerializer::toJson(const core::DamageType *obj)
{
    return this->gameObjectToJson(obj);
}

QJsonObject JsonSerializer::toJson(const core::Faction *obj)
{
    QJsonObject jobj(this->gameObjectToJson(obj));

    jobj["unitTypes"] = toQJsonArray(obj->getUnitTypes(), qObjectName);
    jobj["recruits"] = toQJsonObject(
        obj->getRecruits(),
        qObjectName,
        std::bind(
            toQJsonArray<QList<core::UnitType *>, std::function<QString(core::UnitType *)>>,
            std::placeholders::_1,
            qObjectName
        )
    );

    return jobj;
}

QJsonObject JsonSerializer::toJson(const core::SettlementType *obj)
{
    QJsonObject jobj(this->gameObjectToJson(obj));

    jobj["goldPerTurn"] = obj->getGoldPerTurn();
    jobj["recruits"] = toQJsonArray(obj->getRecruits(), qObjectName);

    return jobj;
}

QJsonObject JsonSerializer::toJson(const core::TerrainType *obj)
{
    return this->gameObjectToJson(obj);
}

QJsonObject JsonSerializer::toJson(const core::UnitClass *obj)
{
    QJsonObject jobj(this->gameObjectToJson(obj));

    jobj["movementPoints"] = obj->getMovementPoints();
    jobj["movementCosts"] = toQJsonObject(
        obj->getMovementCosts(),
        qObjectName,
        constructQJsonValue<int>
    );
    jobj["attacks"] = toQJsonObject(
        obj->getAttacks(),
        qObjectName,
        constructQJsonValue<int>
    );
    jobj["defenses"] = toQJsonObject(
        obj->getDefenses(),
        qObjectName,
        constructQJsonValue<int>
    );

    return jobj;
}

QJsonObject JsonSerializer::toJson(const core::UnitLevel *obj)
{
    QJsonObject jobj(this->gameObjectToJson(obj));

    jobj["index"] = obj->getIndex();
    jobj["experiencePoints"] = obj->getExperiencePoints();

    return jobj;
}

QJsonObject JsonSerializer::toJson(const core::UnitType *obj)
{
    QJsonObject jobj(this->gameObjectToJson(obj));

    jobj["class"] = obj->getClass()->objectName();
    jobj["level"] = obj->getLevel()->objectName();
    jobj["hitPoints"] = obj->getHitPoints();
    jobj["recruitmentCost"] = obj->getRecruitmentCost();
    jobj["upkeepCost"] = obj->getUpkeepCost();
    jobj["armor"] = obj->getArmor()->objectName();
    jobj["weapons"] = toQJsonArray(obj->getWeapons(), qObjectName);
    jobj["upgrades"] = toQJsonArray(obj->getUpgrades(), qObjectName);

    return jobj;
}

QJsonObject JsonSerializer::toJson(const core::Weapon *obj)
{
    QJsonObject jobj(this->gameObjectToJson(obj));

    jobj["range"] = obj->getRange();
    jobj["damages"] = toQJsonObject(
        obj->getDamages(),
        qObjectName,
        constructQJsonValue<int>
    );

    return jobj;
}

QJsonObject JsonSerializer::gameObjectToJson(const core::GameObject *obj)
{
    QJsonObject jobj;

	jobj["objectName"] = obj->objectName();
	jobj["displayName"] = obj->getDisplayName();

    return jobj;
}
