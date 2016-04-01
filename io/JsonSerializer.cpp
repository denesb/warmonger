#include <functional>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "io/JsonSerializer.h"

#include "core/Army.h"
#include "core/Armor.h"
#include "core/CampaignMap.h"
#include "core/Civilization.h"
#include "core/DamageType.h"
#include "core/Faction.h"
#include "core/MapNode.h"
#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/Unit.h"
#include "core/UnitClass.h"
#include "core/UnitLevel.h"
#include "core/UnitType.h"
#include "core/Weapon.h"
#include "core/WeaponClass.h"
#include "core/WeaponType.h"
#include "core/World.h"
#include "core/WorldSurface.h"

using namespace warmonger;
using namespace warmonger::io;

QJsonObject armorToJson(const core::Armor *obj);
QJsonObject armyToJson(const core::Army *obj);
QJsonObject campaignMapToJson(const core::CampaignMap *obj);
QJsonObject civilizationToJson(const core::Civilization *obj);
QJsonObject damageTypeToJson(const core::DamageType *obj);
QJsonObject factionToJson(const core::Faction *obj);
QJsonObject mapNodeToJson(const core::MapNode *obj);
QJsonObject settlementToJson(const core::Settlement *obj);
QJsonObject settlementTypeToJson(const core::SettlementType *obj);
QJsonObject terrainTypeToJson(const core::TerrainType *obj);
QJsonObject unitToJson(const core::Unit *obj);
QJsonObject unitClassToJson(const core::UnitClass *obj);
QJsonObject unitLevelToJson(const core::UnitLevel *obj);
QJsonObject unitTypeToJson(const core::UnitType *obj);
QJsonObject weaponToJson(const core::Weapon *obj);
QJsonObject weaponClassToJson(const core::WeaponClass *obj);
QJsonObject weaponTypeToJson(const core::WeaponType *obj);
QJsonObject worldToJson(const core::World *obj);
QJsonObject worldSurfaceToJson(const core::WorldSurface *obj);

JsonSerializer::JsonSerializer(QJsonDocument::JsonFormat format) :
    format(format)
{
}

QByteArray JsonSerializer::serializeArmor(const core::Armor *obj)
{
    QJsonDocument jdoc(armorToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeArmy(const core::Army *obj)
{
    QJsonDocument jdoc(armyToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeCampaignMap(const core::CampaignMap *obj)
{
    QJsonDocument jdoc(campaignMapToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeCivilization(const core::Civilization *obj)
{
    QJsonDocument jdoc(civilizationToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeDamageType(const core::DamageType *obj)
{
    QJsonDocument jdoc(damageTypeToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeFaction(const core::Faction *obj)
{
    QJsonDocument jdoc(factionToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeMapNode(const core::MapNode *obj)
{
    QJsonDocument jdoc(mapNodeToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeSettlement(const core::Settlement *obj)
{
    QJsonDocument jdoc(settlementToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeSettlementType(
    const core::SettlementType *obj
)
{
    QJsonDocument jdoc(settlementTypeToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeTerrainType(const core::TerrainType *obj)
{
    QJsonDocument jdoc(terrainTypeToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeUnit(const core::Unit *obj)
{
    QJsonDocument jdoc(unitToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeUnitClass(const core::UnitClass *obj)
{
    QJsonDocument jdoc(unitClassToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeUnitLevel(const core::UnitLevel *obj)
{
    QJsonDocument jdoc(unitLevelToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeUnitType(const core::UnitType *obj)
{
    QJsonDocument jdoc(unitTypeToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeWeapon(const core::Weapon *obj)
{
    QJsonDocument jdoc(weaponToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeWeaponClass(const core::WeaponClass *obj)
{
    QJsonDocument jdoc(weaponClassToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeWeaponType(const core::WeaponType *obj)
{
    QJsonDocument jdoc(weaponTypeToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeWorld(const core::World *obj)
{
    QJsonDocument jdoc(worldToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeWorldSurface(const core::WorldSurface *obj)
{
    QJsonDocument jdoc(worldSurfaceToJson(obj));
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

    for(const auto& element : container)
    {
        QString key = convertKey(element.first);
        QJsonValue value = convertValue(element.second);

        object[key] = value;
    }

    return object;
}

QJsonObject armorToJson(const core::Armor *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["defenses"] = toQJsonObject(
        obj->getDefenses(),
        qObjectName,
        constructQJsonValue<int>
    );

    return jobj;
}

QJsonObject armyToJson(const core::Army *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["mapNode"] = obj->getMapNode()->objectName();
    jobj["owner"] = obj->getOwner()->objectName();
    jobj["units"] = toQJsonArray(
        obj->getUnits(),
        qObjectName
    );

    return jobj;
}

QJsonObject campaignMapToJson(const core::CampaignMap *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["world"] = obj->getWorld()->objectName();
    jobj["mapNodeIndex"] = obj->getMapNodeIndex();
    jobj["settlementIndex"] = obj->getSettlementIndex();
    jobj["unitIndex"] = obj->getUnitIndex();
    jobj["mapNodes"] = toQJsonArray(
        obj->getMapNodes(),
        mapNodeToJson
    );
    jobj["factions"] = toQJsonArray(
        obj->getFactions(),
        factionToJson
    );
    jobj["settlements"] = toQJsonArray(
        obj->getSettlements(),
        settlementToJson
    );
    jobj["units"] = toQJsonArray(
        obj->getUnits(),
        unitToJson
    );
    jobj["armies"] = toQJsonArray(
        obj->getArmies(),
        armyToJson
    );

    return jobj;
}

QJsonObject civilizationToJson(const core::Civilization *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["unitTypes"] = toQJsonArray(obj->getUnitTypes(), qObjectName);
    jobj["recruits"] = toQJsonObject(
        obj->getRecruits(),
        qObjectName,
        std::bind(
            toQJsonArray<std::vector<core::UnitType *>, std::function<QString(core::UnitType *)>>,
            std::placeholders::_1,
            qObjectName
        )
    );

    return jobj;
}

QJsonObject damageTypeToJson(const core::DamageType *obj)
{
    return namesToJson(obj);
}

QJsonObject factionToJson(const core::Faction *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["color"] = obj->getColor().name();
    jobj["goldBalance"] = obj->getGoldBalance();
    jobj["civilization"] = obj->getCivilization()->objectName();

    return jobj;
}

QJsonObject mapNodeToJson(const core::MapNode *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["terrainType"] = obj->getTerrainType()->objectName();

    QJsonObject jneighbours;
    for (const auto& neighbour : obj->getNeighbours())
    {
        QString neighbourName{""};
        if (neighbour.second != nullptr)
        {
            neighbourName = neighbour.second->objectName();
        }

        jneighbours[core::direction2str(neighbour.first)] = neighbourName;
    }

    jobj["neighbours"] = jneighbours;

    return jobj;
}

QJsonObject settlementToJson(const core::Settlement *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["mapNode"] = obj->getMapNode()->objectName();
    jobj["type"] = obj->getType()->objectName();
    jobj["owner"] = obj->getOwner()->objectName();

    return jobj;
}

QJsonObject settlementTypeToJson(const core::SettlementType *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["goldPerTurn"] = obj->getGoldPerTurn();
    jobj["recruits"] = toQJsonArray(obj->getRecruits(), qObjectName);

    return jobj;
}

QJsonObject terrainTypeToJson(const core::TerrainType *obj)
{
    return namesToJson(obj);
}

QJsonObject unitToJson(const core::Unit *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["type"] = obj->getType()->objectName();
    jobj["mapNode"] = obj->getMapNode()->objectName();
    jobj["owner"] = obj->getOwner()->objectName();
    jobj["experiencePoints"] = obj->getExperiencePoints();
    jobj["hitPoints"] = obj->getHitPoints();
    jobj["movementPoints"] = obj->getMovementPoints();

    return jobj;
}

QJsonObject unitClassToJson(const core::UnitClass *obj)
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

QJsonObject unitLevelToJson(const core::UnitLevel *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["index"] = obj->getIndex();
    jobj["experiencePoints"] = obj->getExperiencePoints();

    return jobj;
}

QJsonObject unitTypeToJson(const core::UnitType *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["class"] = obj->getClass()->objectName();
    jobj["level"] = obj->getLevel()->objectName();
    jobj["hitPoints"] = obj->getHitPoints();
    jobj["recruitmentCost"] = obj->getRecruitmentCost();
    jobj["upkeepCost"] = obj->getUpkeepCost();
    jobj["armor"] = obj->getArmor()->objectName();
    jobj["weapons"] = toQJsonArray(obj->getWeapons(), qObjectName);
    jobj["attackSkills"] = toQJsonObject(
        obj->getAttackSkills(),
        qObjectName,
        constructQJsonValue<int>
    );
    jobj["defenseSkills"] = toQJsonObject(
        obj->getDefenseSkills(),
        qObjectName,
        constructQJsonValue<int>
    );
    jobj["upgrades"] = toQJsonArray(obj->getUpgrades(), qObjectName);

    return jobj;
}

QJsonObject weaponToJson(const core::Weapon *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["range"] = obj->getRange();
    jobj["damages"] = toQJsonObject(
        obj->getDamages(),
        qObjectName,
        constructQJsonValue<int>
    );
    jobj["type"] = obj->getType()->objectName();

    return jobj;
}

QJsonObject weaponClassToJson(const core::WeaponClass *obj)
{
    QJsonObject jobj(namesToJson(obj));

    return jobj;
}

QJsonObject weaponTypeToJson(const core::WeaponType *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["class"] = obj->getClass()->objectName();

    return jobj;
}

QJsonObject worldToJson(const core::World *obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["damageTypes"] = toQJsonArray(
        obj->getDamageTypes(),
        damageTypeToJson
    );
    jobj["armors"] = toQJsonArray(
        obj->getArmors(),
        armorToJson
    );
    jobj["weaponClasses"] = toQJsonArray(
        obj->getWeaponClasses(),
        weaponClassToJson
    );
    jobj["weaponTypes"] = toQJsonArray(
        obj->getWeaponTypes(),
        weaponTypeToJson
    );
    jobj["weapons"] = toQJsonArray(
        obj->getWeapons(),
        weaponToJson
    );
    jobj["terrainTypes"] = toQJsonArray(
        obj->getTerrainTypes(),
        terrainTypeToJson
    );
    jobj["unitClasses"] = toQJsonArray(
        obj->getUnitClasses(),
        unitClassToJson
    );
    jobj["unitLevels"] = toQJsonArray(
        obj->getUnitLevels(),
        unitLevelToJson
    );
    jobj["unitTypes"] = toQJsonArray(
        obj->getUnitTypes(),
        unitTypeToJson
    );
    jobj["settlementTypes"] = toQJsonArray(
        obj->getSettlementTypes(),
        settlementTypeToJson
    );
    jobj["civilizations"] = toQJsonArray(
        obj->getCivilizations(),
        civilizationToJson
    );

    return jobj;
}

QJsonObject worldSurfaceToJson(const core::WorldSurface *obj)
{
    QJsonObject jobj(namesToJson(obj));

	jobj["tileWidth"] = obj->getTileWidth();
	jobj["tileHeight"] = obj->getTileHeight();

    return jobj;
}
