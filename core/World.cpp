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
    civilizations()
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

std::vector<Armor *> World::getArmors() const
{
    return this->armors;
}

void World::setArmors(const std::vector<Armor *> &armors)
{
    this->armors = armors;
}

std::vector<DamageType *> World::getDamageTypes() const
{
    return this->damageTypes;
}

void World::setDamageTypes(const std::vector<DamageType *> &damageTypes)
{
    this->damageTypes = damageTypes;
}

std::vector<Civilization *> World::getCivilizations() const
{
    return this->civilizations;
}

void World::setCivilizations(const std::vector<Civilization *> &civilizations)
{
    this->civilizations = civilizations;
}

QVariantList World::readCivilizations() const
{
    return toQVariantList(this->civilizations);
}

std::vector<SettlementType *> World::getSettlementTypes() const
{
    return this->settlementTypes;
}

void World::setSettlementTypes(const std::vector<SettlementType *> &settlementTypes)
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

std::vector<TerrainType *> World::getTerrainTypes() const
{
    return this->terrainTypes;
}

void World::setTerrainTypes(const std::vector<TerrainType *> &terrainTypes)
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

std::vector<UnitClass *> World::getUnitClasses() const
{
    return this->unitClasses;
}

void World::setUnitClasses(const std::vector<UnitClass *> &unitClasses)
{
    this->unitClasses = unitClasses;
}

std::vector<UnitLevel *> World::getUnitLevels() const
{
    return this->unitLevels;
}

void World::setUnitLevels(const std::vector<UnitLevel *> &unitLevels)
{
    if (this->unitLevels != unitLevels)
    {
        this->unitLevels = unitLevels;
        emit unitLevelsChanged();
    }
}

std::vector<UnitType *> World::getUnitTypes() const
{
    return this->unitTypes;
}

void World::setUnitTypes(const std::vector<UnitType *> &unitTypes)
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

std::vector<WeaponClass *> World::getWeaponClasses() const
{
    return this->weaponClasses;
}

void World::setWeaponClasses(const std::vector<WeaponClass *> &weaponClasses)
{
    if (this->weaponClasses != weaponClasses)
    {
        this->weaponClasses = weaponClasses;
        emit weaponClassesChanged();
    }
}

std::vector<WeaponType *> World::getWeaponTypes() const
{
    return this->weaponTypes;
}

void World::setWeaponTypes(const std::vector<WeaponType *> &weaponTypes)
{
    if (this->weaponTypes != weaponTypes)
    {
        this->weaponTypes = weaponTypes;
        emit weaponTypesChanged();
    }
}

std::vector<Weapon *> World::getWeapons() const
{
    return this->weapons;
}

void World::setWeapons(const std::vector<Weapon *> &weapons)
{
    if (this->weapons != weapons)
    {
        this->weapons = weapons;
        emit weaponsChanged();
    }
}
