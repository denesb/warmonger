#include "core/World.h"
#include "core/TerrainType.h"
#include "core/SettlementType.h"
#include "core/UnitClass.h"
#include "core/DamageType.h"
#include "core/Weapon.h"
#include "core/Armor.h"
#include "core/UnitType.h"
#include "core/Faction.h"
#include "core/WorldResources.h"
#include "core/JsonUtil.hpp"
#include "core/Util.h"

using namespace warmonger::core;

const QString World::DefinitionFile = "world.json";

World::World(QObject *parent) :
    GameObject(parent),
    path(),
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

QString World::getPath() const
{
    return this->path;
}

void World::setPath(const QString &path)
{
    this->path = path;
}

QString World::getDescription() const
{
    return this->description;
}

void World::setDescription(const QString &description)
{
    this->description = description;
}

QList<const TerrainType *> World::getTerrainTypes() const
{
    return listConstClone(this->terrainTypes);
}

QList<TerrainType *> World::getTerrainTypes()
{
    return this->terrainTypes;
}

void World::setTerrainTypes(const QList<TerrainType *> &terrainTypes)
{
    this->terrainTypes = terrainTypes;
}

QVariantList World::readTerrainTypes() const
{
    return toQVariantList<TerrainType>(this->terrainTypes);
}

QList<const UnitClass *> World::getUnitClasses() const
{
    return listConstClone(this->unitClasses);
}

QList<UnitClass *> World::getUnitClasses()
{
    return this->unitClasses;
}

void World::setUnitClasses(const QList<UnitClass *> &unitClasses)
{
    this->unitClasses = unitClasses;
}

QList<const DamageType *> World::getDamageTypes() const
{
    return listConstClone(this->damageTypes);
}

QList<DamageType *> World::getDamageTypes()
{
    return this->damageTypes;
}

void World::setDamageTypes(const QList<DamageType *> &damageTypes)
{
    this->damageTypes = damageTypes;
}

QList<const Armor *> World::getArmors() const
{
    return listConstClone(this->armors);
}

QList<Armor *> World::getArmors()
{
    return this->armors;
}

void World::setArmors(const QList<Armor *> &armors)
{
    this->armors = armors;
}

QList<const Weapon *> World::getWeapons() const
{
    return listConstClone(this->weapons);
}

QList<Weapon *> World::getWeapons()
{
    return this->weapons;
}

void World::setWeapons(const QList<Weapon *> &weapons)
{
    this->weapons = weapons;
}

QList<const UnitType *> World::getUnitTypes() const
{
    return listConstClone(this->unitTypes);
}

QList<UnitType *> World::getUnitTypes()
{
    return this->unitTypes;
}

void World::setUnitTypes(const QList<UnitType *> &unitTypes)
{
    this->unitTypes = unitTypes;
}

QList<const SettlementType *> World::getSettlementTypes() const
{
    return listConstClone(this->settlementTypes);
}

QList<SettlementType *> World::getSettlementTypes()
{
    return this->settlementTypes;
}

void World::setSettlementTypes(const QList<SettlementType *> &settlementTypes)
{
    this->settlementTypes = settlementTypes;
}

QList<const Faction *> World::getFactions() const
{
    return listConstClone(this->factions);
}

QList<Faction *> World::getFactions()
{
    return this->factions;
}

void World::setFactions(const QList<Faction *> &factions)
{
    this->factions = factions;
}

const WorldResources * World::getResources() const
{
    return this->resources;
}

WorldResources * World::getResources()
{
    return this->resources;
}

void World::setResources(WorldResources *resources)
{
    this->resources = resources;
}

QVariant World::readResources() const
{
    return QVariant::fromValue<QObject *>(this->resources);
}

void World::dataFromJson(const QJsonObject &obj)
{
    this->description = obj["description"].toString();
    this->terrainTypes = newListFromJson<TerrainType>(obj["terrainTypes"].toArray(), this);
    this->unitClasses = newListFromJson<UnitClass>(obj["unitClasses"].toArray(), this);
    this->damageTypes = newListFromJson<DamageType>(obj["damageTypes"].toArray(), this);
    this->weapons = newListFromJson<Weapon>(obj["weapons"].toArray(), this);
    this->armors = newListFromJson<Armor>(obj["armors"].toArray(), this);
    this->unitTypes = newListFromJson<UnitType>(obj["unitTypes"].toArray(), this);
    this->settlementTypes = newListFromJson<SettlementType>(obj["settlementTypes"].toArray(), this);
    this->factions = newListFromJson<Faction>(obj["factions"].toArray(), this);

    this->resources = new WorldResources(this);
    this->resources->loadFromJsonFile();
}

void World::dataToJson(QJsonObject &obj) const
{
    obj["description"] = this->description;
    obj["terrainTypes"] = listToJson<TerrainType>(this->terrainTypes);
    obj["unitClasses"] = listToJson<UnitClass>(this->unitClasses);
    obj["damageTypes"] = listToJson<DamageType>(this->damageTypes);
    obj["weapons"] = listToJson<Weapon>(this->weapons);
    obj["armors"] = listToJson<Armor>(this->armors);
    obj["unitTypes"] = listToJson<UnitType>(this->unitTypes);
    obj["settlementTypes"] = listToJson<SettlementType>(this->settlementTypes);
    obj["factions"] = listToJson<Faction>(this->factions);

    //TODO: write resources
}
