#include "core/World.h"
#include "core/JsonUtil.hpp"

using namespace core;

const QString World::WorldDefinitionFile = "world.json";

World::World(QObject *parent) :
    WorldItem(parent),
    description(""),
    villageGoldPt(0),
    cityGoldPt(0),
    terrainTypes(),
    unitClasses(),
    damageTypes(),
    weapons(),
    armors(),
    unitTypes(),
    factions()
{
}

int World::getVillageGoldPt() const
{
    return this->villageGoldPt;
}

void World::setVillageGoldPt(int villageGoldPt)
{
    this->villageGoldPt = villageGoldPt;
}

int World::getCityGoldPt() const
{
    return this->cityGoldPt;
}

void World::setCityGoldPt(int cityGoldPt)
{
    this->cityGoldPt = cityGoldPt;
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

QList<UnitClass *> World::getUnitClasses() const
{
    return this->unitClasses;
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

void World::fromJson(const QJsonObject &obj)
{
    this->description = obj["description"].toString();
    this->villageGoldPt = obj["villageGoldPt"].toInt();
    this->cityGoldPt = obj["villageGoldPt"].toInt();
    this->terrainTypes = newListFromJson<TerrainType>(obj["terrainTypes"].toArray(), this);
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
    obj["villageGoldPt"] = this->villageGoldPt;
    obj["cityGoldPt"] = this->cityGoldPt;
    obj["terrainTypes"] = listToJson<TerrainType>(this->terrainTypes);
    obj["unitClasses"] = listToJson<UnitClass>(this->unitClasses);
    obj["damageTypes"] = listToJson<DamageType>(this->damageTypes);
    obj["weapons"] = listToJson<Weapon>(this->weapons);
    obj["armors"] = listToJson<Armor>(this->armors);
    obj["unitTypes"] = listToJson<UnitType>(this->unitTypes);
    obj["factions"] = listToJson<Faction>(this->factions);

    return std::move(obj);
}
