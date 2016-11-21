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

#include <functional>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include "core/CampaignMap.h"
#include "core/Hexagon.h"
#include "core/World.h"
#include "io/JsonSerializer.h"

using namespace warmonger;
using namespace warmonger::io;

QJsonObject armyToJson(const core::Army* obj);
QJsonObject armyTypeToJson(const core::ArmyType* obj);
QJsonObject bannerToJson(const core::Banner* obj);
QJsonObject campaignMapToJson(const core::CampaignMap* obj);
QJsonObject civilizationToJson(const core::Civilization* obj);
QJsonObject factionToJson(const core::Faction* obj);
QJsonObject mapNodeToJson(const core::MapNode* obj);
QJsonObject settlementToJson(const core::Settlement* obj);
QJsonObject settlementTypeToJson(const core::SettlementType* obj);
QJsonObject terrainTypeToJson(const core::TerrainType* obj);
QJsonObject unitToJson(const core::Unit* obj);
QJsonObject unitTypeToJson(const core::UnitType* obj);
QJsonObject worldToJson(const core::World* obj);

JsonSerializer::JsonSerializer(QJsonDocument::JsonFormat format)
    : format(format)
{
}

QByteArray JsonSerializer::serializeArmy(const core::Army* obj)
{
    QJsonDocument jdoc(armyToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeArmyType(const core::ArmyType* obj)
{
    QJsonDocument jdoc(armyTypeToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeBanner(const core::Banner* obj)
{
    QJsonDocument jdoc(bannerToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeCampaignMap(const core::CampaignMap* obj)
{
    QJsonDocument jdoc(campaignMapToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeCivilization(const core::Civilization* obj)
{
    QJsonDocument jdoc(civilizationToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeFaction(const core::Faction* obj)
{
    QJsonDocument jdoc(factionToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeMapNode(const core::MapNode* obj)
{
    QJsonDocument jdoc(mapNodeToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeSettlement(const core::Settlement* obj)
{
    QJsonDocument jdoc(settlementToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeSettlementType(const core::SettlementType* obj)
{
    QJsonDocument jdoc(settlementTypeToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeTerrainType(const core::TerrainType* obj)
{
    QJsonDocument jdoc(terrainTypeToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeUnit(const core::Unit* obj)
{
    QJsonDocument jdoc(unitToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeUnitType(const core::UnitType* obj)
{
    QJsonDocument jdoc(unitTypeToJson(obj));
    return jdoc.toJson(this->format);
}

QByteArray JsonSerializer::serializeWorld(const core::World* obj)
{
    QJsonDocument jdoc(worldToJson(obj));
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
inline QString qObjectName(const QObject* const object)
{
    return object->objectName();
}

/**
 * Construct a QJsonValue object directly from T.
 *
 * Only usable if QJsonValue has a constructor with T (or a type
 * implicitly convertable to T).
 */
template <typename T>
QJsonValue constructQJsonValue(const T& value)
{
    return QJsonValue(value);
}

/**
 * Convert Container to QJsonArray.
 *
 * The convertFunc function converts Container::value_type to
 * QJsonValue.
 */
template <typename Container, typename ConvertFunc>
QJsonArray toQJsonArray(Container container, ConvertFunc convertFunc)
{
    QJsonArray array;

    std::transform(container.cbegin(), container.cend(), std::back_inserter(array), convertFunc);

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
template <typename Container, typename ConvertKeyFunc, typename ConvertValueFunc>
QJsonObject toQJsonObject(Container container, ConvertKeyFunc convertKey, ConvertValueFunc convertValue)
{
    QJsonObject object;

    for (const auto& element : container)
    {
        QString key = convertKey(element.first);
        QJsonValue value = convertValue(element.second);

        object[key] = value;
    }

    return object;
}

QJsonObject armyToJson(const core::Army* obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["type"] = obj->getType()->objectName();
    jobj["mapNode"] = obj->getMapNode()->objectName();
    jobj["owner"] = obj->getOwner()->objectName();
    jobj["units"] = toQJsonArray(obj->getUnits(), qObjectName);

    return jobj;
}

QJsonObject armyTypeToJson(const core::ArmyType* obj)
{
    QJsonObject jobj;

    jobj["objectName"] = obj->objectName();

    const bool isRoot = obj->isHierarchyRoot();
    const core::ArmyType* parent = obj->getHierarchyParent();

    if (!isRoot)
    {
        jobj["hierarchyParent"] = parent->objectName();
    }

    if (isRoot || obj->getDisplayName() != parent->getDisplayName())
        jobj["displayName"] = obj->getDisplayName();

    return jobj;
}

QJsonObject bannerToJson(const core::Banner* obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["civilizations"] = toQJsonArray(obj->getCivilizations(), qObjectName);

    return jobj;
}

QJsonObject campaignMapToJson(const core::CampaignMap* obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["world"] = obj->getWorld()->objectName();
    jobj["mapNodes"] = toQJsonArray(obj->getMapNodes(), mapNodeToJson);
    jobj["factions"] = toQJsonArray(obj->getFactions(), factionToJson);
    jobj["settlements"] = toQJsonArray(obj->getSettlements(), settlementToJson);
    jobj["units"] = toQJsonArray(obj->getUnits(), unitToJson);
    jobj["armies"] = toQJsonArray(obj->getArmies(), armyToJson);

    return jobj;
}

QJsonObject civilizationToJson(const core::Civilization* obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["unitTypes"] = toQJsonArray(obj->getUnitTypes(), qObjectName);

    return jobj;
}

QJsonObject factionToJson(const core::Faction* obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["primaryColor"] = obj->getPrimaryColor().name();
    jobj["secondaryColor"] = obj->getSecondaryColor().name();
    jobj["banner"] = obj->getBanner()->objectName();
    jobj["civilization"] = obj->getCivilization()->objectName();

    return jobj;
}

QJsonObject mapNodeToJson(const core::MapNode* obj)
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

QJsonObject settlementToJson(const core::Settlement* obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["mapNode"] = obj->getMapNode()->objectName();
    jobj["type"] = obj->getType()->objectName();
    jobj["owner"] = obj->getOwner()->objectName();

    return jobj;
}

QJsonObject settlementTypeToJson(const core::SettlementType* obj)
{
    QJsonObject jobj;

    jobj["objectName"] = obj->objectName();

    const bool isRoot = obj->isHierarchyRoot();
    const core::SettlementType* parent = obj->getHierarchyParent();

    if (!isRoot)
    {
        jobj["hierarchyParent"] = parent->objectName();
    }

    if (isRoot || obj->getDisplayName() != parent->getDisplayName())
        jobj["displayName"] = obj->getDisplayName();

    return jobj;
}

QJsonObject terrainTypeToJson(const core::TerrainType* obj)
{
    QJsonObject jobj;

    jobj["objectName"] = obj->objectName();

    const bool isRoot = obj->isHierarchyRoot();
    const core::TerrainType* parent = obj->getHierarchyParent();

    if (!isRoot)
    {
        jobj["hierarchyParent"] = parent->objectName();
    }

    if (isRoot || obj->getDisplayName() != parent->getDisplayName())
        jobj["displayName"] = obj->getDisplayName();

    return jobj;
}

QJsonObject unitToJson(const core::Unit* obj)
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

QJsonObject unitTypeToJson(const core::UnitType* obj)
{
    QJsonObject jobj;

    jobj["objectName"] = obj->objectName();

    const bool isRoot = obj->isHierarchyRoot();
    const core::UnitType* parent = obj->getHierarchyParent();

    if (!isRoot)
    {
        jobj["hierarchyParent"] = parent->objectName();
    }

    if (isRoot || obj->getDisplayName() != parent->getDisplayName())
        jobj["displayName"] = obj->getDisplayName();

    if (isRoot || obj->getHitPoints() != parent->getHitPoints())
        jobj["hitPoints"] = obj->getHitPoints();

    if (isRoot || obj->getExperiencePoints() != parent->getExperiencePoints())
        jobj["experiencePoints"] = obj->getExperiencePoints();

    if (isRoot || obj->getMovementPoints() != parent->getMovementPoints())
        jobj["movementPoints"] = obj->getMovementPoints();

    if (isRoot || obj->getRecruitmentCost() != parent->getRecruitmentCost())
        jobj["recruitmentCost"] = obj->getRecruitmentCost();

    if (isRoot || obj->getUpkeepCost() != parent->getUpkeepCost())
        jobj["upkeepCost"] = obj->getUpkeepCost();

    if (isRoot || obj->getMovementCosts() != parent->getMovementCosts())
        jobj["movementCosts"] = toQJsonObject(obj->getMovementCosts(), qObjectName, constructQJsonValue<int>);

    if (isRoot || obj->getUpgrades() != parent->getUpgrades())
        jobj["upgrades"] = toQJsonArray(obj->getUpgrades(), qObjectName);

    return jobj;
}

QJsonObject worldToJson(const core::World* obj)
{
    QJsonObject jobj(namesToJson(obj));

    jobj["armyTypes"] = toQJsonArray(obj->getArmyTypes(), armyTypeToJson);
    jobj["banners"] = toQJsonArray(obj->getBanners(), bannerToJson);
    jobj["civilizations"] = toQJsonArray(obj->getCivilizations(), civilizationToJson);
    jobj["colors"] = toQJsonArray(obj->getColors(), [](const QColor& c) { return c.name(); });
    jobj["terrainTypes"] = toQJsonArray(obj->getTerrainTypes(), terrainTypeToJson);
    jobj["unitTypes"] = toQJsonArray(obj->getUnitTypes(), unitTypeToJson);
    jobj["settlementTypes"] = toQJsonArray(obj->getSettlementTypes(), settlementTypeToJson);

    return jobj;
}
