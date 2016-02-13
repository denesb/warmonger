#include <functional>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "io/JsonSerializer.h"

#include "core/Armor.h"
#include "core/DamageType.h"
#include "core/Faction.h"
#include "core/Map.h"
#include "core/MapNode.h"
#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/Player.h"
#include "core/TerrainType.h"
#include "core/Unit.h"
#include "core/UnitClass.h"
#include "core/UnitLevel.h"
#include "core/UnitType.h"
#include "core/Weapon.h"
#include "core/World.h"
#include "core/WorldSurface.h"

using namespace warmonger;
using namespace warmonger::io;

QJsonObject toJson(const core::Armor *obj);
QJsonObject toJson(const core::DamageType *obj);
QJsonObject toJson(const core::Faction *obj);
QJsonObject toJson(const core::Map *obj);
QJsonObject toJson(const core::MapNode *obj);
QJsonObject toJson(const core::Player *obj);
QJsonObject toJson(const core::Settlement *obj);
QJsonObject toJson(const core::SettlementType *obj);
QJsonObject toJson(const core::TerrainType *obj);
QJsonObject toJson(const core::Unit *obj);
QJsonObject toJson(const core::UnitClass *obj);
QJsonObject toJson(const core::UnitLevel *obj);
QJsonObject toJson(const core::UnitType *obj);
QJsonObject toJson(const core::Weapon *obj);
QJsonObject toJson(const core::World *obj);
QJsonObject toJson(const core::WorldSurface *obj);

JsonSerializer::JsonSerializer(QJsonDocument::JsonFormat format) :
    format(format)
{
}

QByteArray JsonSerializer::serialize(const core::Armor *obj)
{
    QJsonDocument jdoc(toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::DamageType *obj)
{
    QJsonDocument jdoc(toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::Faction *obj)
{
    QJsonDocument jdoc(toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::Game *obj)
{
}

QByteArray JsonSerializer::serialize(const core::Map *obj)
{
    QJsonDocument jdoc(toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::MapNode *obj)
{
    QJsonDocument jdoc(toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::Player *obj)
{
    QJsonDocument jdoc(toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::Settlement *obj)
{
    QJsonDocument jdoc(toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::SettlementType *obj)
{
    QJsonDocument jdoc(toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::TerrainType *obj)
{
    QJsonDocument jdoc(toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::Unit *obj)
{
    QJsonDocument jdoc(toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::UnitClass *obj)
{
    QJsonDocument jdoc(toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::UnitLevel *obj)
{
    QJsonDocument jdoc(toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::UnitType *obj)
{
    QJsonDocument jdoc(toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::Weapon *obj)
{
    QJsonDocument jdoc(toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::World *obj)
{
    QJsonDocument jdoc(toJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serialize(const core::WorldSurface *obj)
{
    QJsonDocument jdoc(toJson(obj));
    return jdoc.toJson(this->format);
}

template <typename T>
QJsonObject namesToJson(T obj)
{
    QJsonObject jobj;

	jobj["objectName"] = obj->objectName();
	jobj["displayName"] = obj->getDisplayName();

    return jobj;
}

template <typename T>
std::function<QJsonObject(const T *)> typeToJsonFunc()
{
    QJsonObject (*func)(const T *) = toJson;
    return std::function<QJsonObject(const T *)>(func);
}

/**
 * Get the objectName of a QObject.
 */
inline QString qObjectName(const QObject * const object)
{
    return object->objectName();
}

/**
 * Construct a QJsonValue object directly from T.
 *
 * Only usable if QJsonValue has a constructor with T (or a type
 * implicitly convertable to T).
 */
template<typename T>
QJsonValue constructQJsonValue(const T &value)
{
    return QJsonValue(value);
}

/**
 * Convert Container to QJsonArray.
 *
 * The convertFunc function converts Container::value_type to
 * QJsonValue.
 */
template<typename Container, typename ConvertFunc>
QJsonArray toQJsonArray(Container container, ConvertFunc convertFunc)
{
    QJsonArray array;

    std::transform(
        container.cbegin(),
        container.cend(),
        std::back_inserter(array),
        convertFunc
    );

    return array;
}

/**
 * Convert a Qt-style mapping-type Container to QJsonObject.
 *
 * Qt-style means that the iterator has a key() and value() member
 * returning the key and value respectively.
 * For the conversion a `convertKey` and a `convertValue` function must
 * be supplied, where `convertKey` converts from Container::key_type to
 * QString and `convertValue converts from Container::mapping_type to
 * QJsonValue.
 */
template<typename Container, typename ConvertKeyFunc, typename ConvertValueFunc>
QJsonObject toQJsonObject(
    Container container,
    ConvertKeyFunc convertKey,
    ConvertValueFunc convertValue
)
{
    QJsonObject object;

    for(auto it = container.cbegin(); it != container.cend(); it++)
    {
        QString key = convertKey(it.key());
        QJsonValue value = convertValue(it.value());

        object[key] = value;
    }

    return object;
}

QJsonObject toJson(const core::Armor *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["defenses"] = toQJsonObject(
        obj->getDefenses(),
        qObjectName,
        constructQJsonValue<int>
    );

    return jobj;
}

QJsonObject toJson(const core::DamageType *obj)
{
    return namesToJson(obj);
}

QJsonObject toJson(const core::Faction *obj)
{
    QJsonObject jobj(namesToJson(obj));

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

QJsonObject toJson(const core::Map *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["world"] = obj->getWorld()->objectName();
    jobj["mapNodeIndex"] = obj->getMapNodeIndex();
    jobj["settlementIndex"] = obj->getSettlementIndex();
    jobj["unitIndex"] = obj->getUnitIndex();
    jobj["mapNodes"] = toQJsonArray(
        obj->getMapNodes(),
        typeToJsonFunc<core::MapNode>()
    );
    jobj["players"] = toQJsonArray(
        obj->getPlayers(),
        typeToJsonFunc<core::Player>()
    );
    jobj["settlements"] = toQJsonArray(
        obj->getSettlements(),
        typeToJsonFunc<core::Settlement>()
    );
    jobj["units"] = toQJsonArray(
        obj->getUnits(),
        typeToJsonFunc<core::Unit>()
    );

    return jobj;
}

QJsonObject toJson(const core::MapNode *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["terrainType"] = obj->getTerrainType()->objectName();

    return jobj;
}

QJsonObject toJson(const core::Player *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["color"] = obj->getColor().name();
    jobj["goldBalance"] = obj->getGoldBalance();
    jobj["faction"] = obj->getFaction()->objectName();

    return jobj;
}

QJsonObject toJson(const core::Settlement *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["mapNode"] = obj->getMapNode()->objectName();
    jobj["type"] = obj->getType()->objectName();
    jobj["owner"] = obj->getOwner()->objectName();

    return jobj;
}

QJsonObject toJson(const core::SettlementType *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["goldPerTurn"] = obj->getGoldPerTurn();
    jobj["recruits"] = toQJsonArray(obj->getRecruits(), qObjectName);

    return jobj;
}

QJsonObject toJson(const core::TerrainType *obj)
{
    return namesToJson(obj);
}

QJsonObject toJson(const core::Unit *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["rank"] = core::Unit::rank2str[obj->getRank()];
    jobj["type"] = obj->getType()->objectName();
    jobj["mapNode"] = obj->getMapNode()->objectName();
    jobj["owner"] = obj->getOwner()->objectName();
    jobj["experiencePoints"] = obj->getExperiencePoints();
    jobj["hitPoints"] = obj->getHitPoints();
    jobj["movementPoints"] = obj->getMovementPoints();

    return jobj;
}

QJsonObject toJson(const core::UnitClass *obj)
{
    QJsonObject jobj(namesToJson(obj));

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

QJsonObject toJson(const core::UnitLevel *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["index"] = obj->getIndex();
    jobj["experiencePoints"] = obj->getExperiencePoints();

    return jobj;
}

QJsonObject toJson(const core::UnitType *obj)
{
    QJsonObject jobj(namesToJson(obj));

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

QJsonObject toJson(const core::Weapon *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["range"] = obj->getRange();
    jobj["damages"] = toQJsonObject(
        obj->getDamages(),
        qObjectName,
        constructQJsonValue<int>
    );

    return jobj;
}

QJsonObject toJson(const core::World *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["damageTypes"] = toQJsonArray(
        obj->getDamageTypes(),
        typeToJsonFunc<core::DamageType>()
    );
    jobj["armors"] = toQJsonArray(
        obj->getArmors(),
        typeToJsonFunc<core::Armor>()
    );
    jobj["weapons"] = toQJsonArray(
        obj->getWeapons(),
        typeToJsonFunc<core::Weapon>()
    );
    jobj["terrainTypes"] = toQJsonArray(
        obj->getTerrainTypes(),
        typeToJsonFunc<core::TerrainType>()
    );
    jobj["unitClasses"] = toQJsonArray(
        obj->getUnitClasses(),
        typeToJsonFunc<core::UnitClass>()
    );
    jobj["unitLevels"] = toQJsonArray(
        obj->getUnitLevels(),
        typeToJsonFunc<core::UnitLevel>()
    );
    jobj["unitTypes"] = toQJsonArray(
        obj->getUnitTypes(),
        typeToJsonFunc<core::UnitType>()
    );
    jobj["settlementTypes"] = toQJsonArray(
        obj->getSettlementTypes(),
        typeToJsonFunc<core::SettlementType>()
    );
    jobj["factions"] = toQJsonArray(
        obj->getFactions(),
        typeToJsonFunc<core::Faction>()
    );

    return jobj;
}

QJsonObject toJson(const core::WorldSurface *obj)
{
    QJsonObject jobj(namesToJson(obj));

	jobj["tileWidth"] = obj->getTileWidth();
	jobj["tileHeight"] = obj->getTileHeight();

    return jobj;
}
