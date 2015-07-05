#include "core/World.h"
#include "core/TerrainType.h"
#include "core/SettlementType.h"
#include "core/UnitClass.h"
#include "core/DamageType.h"
#include "core/Weapon.h"
#include "core/Armor.h"
#include "core/UnitType.h"
#include "core/Faction.h"
#include "core/JsonUtil.hpp"

using namespace warmonger::core;

const QString World::WorldDefinitionFile = "world.json";

World::World(QObject *parent) :
    WorldItem(parent),
    description(""),
    terrainTypes(),
    unitClasses(),
    damageTypes(),
    weapons(),
    armors(),
    unitTypes(),
    factions()
{
}

QString World::getDescription() const
{
    return this->description;
}

void World::setDescription(const QString &description)
{
    this->description = description;
}

QList<TerrainType *> World::getTerrainTypes() const
{
    return this->terrainTypes;
}

void World::setTerrainTypes(const QList<TerrainType *> &terrainTypes)
{
    this->terrainTypes = terrainTypes;
}

QList<SettlementType *> World::getSettlementTypes() const
{
    return this->settlementTypes;
}

void World::setUnitClasses(const QList<UnitClass *> &unitClasses)
{
    this->unitClasses = unitClasses;
}

QList<DamageType *> World::getDamageTypes() const
{
    return this->damageTypes;
}

void World::setDamageTypes(const QList<DamageType *> &damageTypes)
{
    this->damageTypes = damageTypes;
}

void World::setSettlementTypes(const QList<SettlementType *> &settlementTypes)
{
    this->settlementTypes = settlementTypes;
}

QList<UnitClass *> World::getUnitClasses() const
{
    return this->unitClasses;
}

void World::fromJson(const QJsonObject &obj)
{
    this->description = obj["description"].toString();
    this->terrainTypes = newListFromJson<TerrainType>(obj["terrainTypes"].toArray(), this);
    this->settlementTypes = newListFromJson<SettlementType>(obj["settlementTypes"].toArray(), this);
    this->unitClasses = newListFromJson<UnitClass>(obj["unitClasses"].toArray(), this);
    this->damageTypes = newListFromJson<DamageType>(obj["damageTypes"].toArray(), this);
    this->weapons = newListFromJson<Weapon>(obj["weapons"].toArray(), this);
    this->armors = newListFromJson<Armor>(obj["armors"].toArray(), this);
    this->unitTypes = newListFromJson<UnitType>(obj["unitTypes"].toArray(), this);
    this->factions = newListFromJson<Faction>(obj["factions"].toArray(), this);
}

QJsonObject World::toJson() const
{
    QJsonObject &&obj = WorldItem::toJson();

    obj["description"] = this->description;
    obj["terrainTypes"] = listToJson<TerrainType>(this->terrainTypes);
    obj["settlementTypes"] = listToJson<SettlementType>(this->settlementTypes);
    obj["unitClasses"] = listToJson<UnitClass>(this->unitClasses);
    obj["damageTypes"] = listToJson<DamageType>(this->damageTypes);
    obj["weapons"] = listToJson<Weapon>(this->weapons);
    obj["armors"] = listToJson<Armor>(this->armors);
    obj["unitTypes"] = listToJson<UnitType>(this->unitTypes);
    obj["factions"] = listToJson<Faction>(this->factions);

    return std::move(obj);
}
