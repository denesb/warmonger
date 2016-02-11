#include <QDir>
#include <QFile>
#include <QJsonDocument>

#include "core/QVariantUtil.h"
#include "core/QJsonUtil.h"
#include "core/World.h"

using namespace warmonger::core;

static const QString loggerName{"core.World"};

const QString World::fileExtension{"wwd"};

World::World(QObject *parent) :
    GameEntity(parent),
    surface(nullptr),
    damageTypes(),
    armors(),
    weapons(),
    terrainTypes(),
    unitClasses(),
    unitLevels(),
    unitTypes(),
    settlementTypes(),
    factions()
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
    return toQVariantList(this->factions);
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
    return toQVariantList(this->settlementTypes);
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
    return toQVariantList(this->terrainTypes);
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
    return toQVariantList(this->unitTypes);
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
    this->damageTypes = fromQJsonArray<QList<DamageType *>>(
        obj["damageTypes"].toArray(),
        ObjectConstructor<DamageType>(this)
    );
    this->armors = fromQJsonArray<QList<Armor *>>(
        obj["armors"].toArray(),
        ObjectConstructor<Armor>(this)
    );
    this->weapons = fromQJsonArray<QList<Weapon *>>(
        obj["weapons"].toArray(),
        ObjectConstructor<Weapon>(this)
    );
    this->terrainTypes = fromQJsonArray<QList<TerrainType *>>(
        obj["terrainTypes"].toArray(),
        ObjectConstructor<TerrainType>(this)
    );
    this->unitClasses = fromQJsonArray<QList<UnitClass *>>(
        obj["unitClasses"].toArray(),
        ObjectConstructor<UnitClass>(this)
    );
    this->unitLevels = fromQJsonArray<QList<UnitLevel *>>(
        obj["unitLevels"].toArray(),
        ObjectConstructor<UnitLevel>(this)
    );
    this->unitTypes = fromQJsonArray<QList<UnitType *>>(
        obj["unitTypes"].toArray(),
        ObjectConstructor<UnitType>(this)
    );
    this->settlementTypes = fromQJsonArray<QList<SettlementType *>>(
        obj["settlementTypes"].toArray(),
        ObjectConstructor<SettlementType>(this)
    );
    this->factions = fromQJsonArray<QList<Faction *>>(
        obj["factions"].toArray(),
        ObjectConstructor<Faction>(this)
    );

    this->setSurface("default");
}

void World::dataToJson(QJsonObject &obj) const
{
    //N.B.: the order here is leaf->root in the world items dependency tree
    obj["damageTypes"] = toQJsonArray(
        this->damageTypes,
        objectToQJsonObject<DamageType>
    );
    obj["armors"] = toQJsonArray(
        this->armors,
        objectToQJsonObject<Armor>
    );
    obj["weapons"] = toQJsonArray(
        this->weapons,
        objectToQJsonObject<Weapon>
    );
    obj["terrainTypes"] = toQJsonArray(
        this->terrainTypes,
        objectToQJsonObject<TerrainType>
    );
    obj["unitClasses"] = toQJsonArray(
        this->unitClasses,
        objectToQJsonObject<UnitClass>
    );
    obj["unitLevels"] = toQJsonArray(
        this->unitLevels,
        objectToQJsonObject<UnitLevel>
    );
    obj["unitTypes"] = toQJsonArray(
        this->unitTypes,
        objectToQJsonObject<UnitType>
    );
    obj["settlementTypes"] = toQJsonArray(
        this->settlementTypes,
        objectToQJsonObject<SettlementType>
    );
    obj["factions"] = toQJsonArray(
        this->factions,
        objectToQJsonObject<Faction>
    );
}
