#include <QDir>
#include <QFile>
#include <QJsonDocument>

#include "core/JsonUtil.h"
#include "core/World.h"

using namespace warmonger::core;

static const QString loggerName{"core.World"};

const QString World::fileExtension{"wwd"};

World::World(QObject *parent) :
    GameEntity(parent),
    surface(nullptr),
    armors(),
    damageTypes(),
    factions(),
    terrainTypes(),
    unitClasses(),
    unitLevels(),
    unitTypes(),
    weapons()
{
}

World::~World()
{
}

WorldSurface * World::getSurface() const
{
    return this->surface;
}

void World::setSurface(WorldSurface *surface)
{
    if (this->surface != surface)
    {
        this->surface = surface;
        emit surfaceChanged();
    }
}

void World::setSurface(const QString &surfaceName)
{
    if (this->surface != nullptr && this->surface->objectName() == surfaceName)
        return;

    this->surface = new WorldSurface(this);
    this->surface->load(this, surfaceName);

    emit surfaceChanged();
}

QList<Armor *> World::getArmors() const
{
    return this->armors;
}

void World::setArmors(const QList<Armor *> &armors)
{
    this->armors = armors;
}

QList<DamageType *> World::getDamageTypes() const
{
    return this->damageTypes;
}

void World::setDamageTypes(const QList<DamageType *> &damageTypes)
{
    this->damageTypes = damageTypes;
}

QList<Faction *> World::getFactions() const
{
    return this->factions;
}

void World::setFactions(const QList<Faction *> &factions)
{
    this->factions = factions;
}

QVariantList World::readFactions() const
{
    return toQVariantList<Faction>(this->factions);
}

QList<SettlementType *> World::getSettlementTypes() const
{
    return this->settlementTypes;
}

void World::setSettlementTypes(const QList<SettlementType *> &settlementTypes)
{
    if (this->settlementTypes != settlementTypes)
    {
        this->settlementTypes = settlementTypes;
        emit settlementTypesChanged();
    }
}

QVariantList World::readSettlementTypes() const
{
    return toQVariantList<SettlementType>(this->settlementTypes);
}

QList<TerrainType *> World::getTerrainTypes() const
{
    return this->terrainTypes;
}

void World::setTerrainTypes(const QList<TerrainType *> &terrainTypes)
{
    if (this->terrainTypes != terrainTypes)
    {
        this->terrainTypes = terrainTypes;
        emit terrainTypesChanged();
    }
}

QVariantList World::readTerrainTypes() const
{
    return toQVariantList<TerrainType>(this->terrainTypes);
}

QList<UnitClass *> World::getUnitClasses() const
{
    return this->unitClasses;
}

void World::setUnitClasses(const QList<UnitClass *> &unitClasses)
{
    this->unitClasses = unitClasses;
}

QList<UnitLevel *> World::getUnitLevels() const
{
    return this->unitLevels;
}

void World::setUnitLevels(const QList<UnitLevel *> &unitLevels)
{
    if (this->unitLevels != unitLevels)
    {
        this->unitLevels = unitLevels;
        emit unitLevelsChanged();
    }
}

QList<UnitType *> World::getUnitTypes() const
{
    return this->unitTypes;
}

void World::setUnitTypes(const QList<UnitType *> &unitTypes)
{
    if (this->unitTypes != unitTypes)
    {
        this->unitTypes = unitTypes;
        emit unitTypesChanged();
    }
}

QVariantList World::readUnitTypes() const
{
    return toQVariantList<UnitType>(this->unitTypes);
}

QList<Weapon *> World::getWeapons() const
{
    return this->weapons;
}

void World::setWeapons(const QList<Weapon *> &weapons)
{
    this->weapons = weapons;
}

void World::dataFromJson(const QJsonObject &obj)
{
    //N.B.: the order here is leaf->root in the world items dependency tree
    this->damageTypes = newListFromJson<DamageType>(obj["damageTypes"].toArray(), this);
    this->armors = newListFromJson<Armor>(obj["armors"].toArray(), this);
    this->weapons = newListFromJson<Weapon>(obj["weapons"].toArray(), this);
    this->terrainTypes = newListFromJson<TerrainType>(obj["terrainTypes"].toArray(), this);
    this->unitClasses = newListFromJson<UnitClass>(obj["unitClasses"].toArray(), this);
    this->unitLevels = newListFromJson<UnitLevel>(obj["unitLevels"].toArray(), this);
    this->unitTypes = newListFromJson<UnitType>(obj["unitTypes"].toArray(), this);
    this->settlementTypes = newListFromJson<SettlementType>(obj["settlementTypes"].toArray(), this);
    this->factions = newListFromJson<Faction>(obj["factions"].toArray(), this);

    this->setSurface("default");
}

void World::dataToJson(QJsonObject &obj) const
{
    //N.B.: the order here is leaf->root in the world items dependency tree
    obj["damageTypes"] = listToJson<DamageType>(this->damageTypes);
    obj["armors"] = listToJson<Armor>(this->armors);
    obj["weapons"] = listToJson<Weapon>(this->weapons);
    obj["terrainTypes"] = listToJson<TerrainType>(this->terrainTypes);
    obj["unitClasses"] = listToJson<UnitClass>(this->unitClasses);
    obj["unitLevels"] = listToJson<UnitLevel>(this->unitLevels);
    obj["unitTypes"] = listToJson<UnitType>(this->unitTypes);
    obj["settlementTypes"] = listToJson<SettlementType>(this->settlementTypes);
    obj["factions"] = listToJson<Faction>(this->factions);
}
