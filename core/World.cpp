#include "core/QVariantUtil.h"
#include "core/World.h"

using namespace warmonger::core;

static const QString loggerName{"core.World"};

World::World(QObject *parent) :
    QObject(parent),
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

QString World::getDisplayName() const
{
    return this->displayName;
}

void World::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
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
