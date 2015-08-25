#include <QFile>
#include <QDir>
#include <QJsonDocument>

#include "core/World.h"
#include "core/WorldSurface.h"
#include "core/TerrainType.h"
#include "core/SettlementType.h"
#include "core/UnitClass.h"
#include "core/DamageType.h"
#include "core/Weapon.h"
#include "core/Armor.h"
#include "core/UnitType.h"
#include "core/Faction.h"
#include "core/World.h"
#include "core/JsonUtil.hpp"
#include "core/Util.h"

using namespace warmonger::core;

static const QString module{"core.World"};

World::World(QObject *parent) :
    GameEntity(parent),
    surface(nullptr),
    terrainTypes(),
    unitClasses(),
    damageTypes(),
    weapons(),
    armors(),
    unitTypes(),
    factions()
{
}

QString World::specification(const QString &objectName) const
{
    return "worlds:" + objectName + ".wwd";
}

const WorldSurface * World::getSurface() const
{
    return this->surface;
}

void World::setSurface(const WorldSurface *surface) const
{
    if (this->surface != surface)
    {
        this->surface = surface;
        emit surfaceChanged();
    }
}

void World::setSurface(const QString &surfaceName) const
{
    if (this->surface != nullptr && this->surface->objectName() == surfaceName)
        return;

    QDir worldDir(this->path + "/surfaces");
    QStringList entryList = worldDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);

    const QString rootPath = worldDir.absolutePath() + "/";
    QStringList searchPaths;
    for (const QString dirEntry : entryList)
    {
        searchPaths << rootPath + dirEntry;
    }

    QDir::setSearchPaths("surfaces", searchPaths);

    // Ugly, but necessary, it does not violate logical constness
    World *parent = const_cast<World *>(this);
    WorldSurface *surface = new WorldSurface(parent);

    surface->load(surface->specification(surfaceName));

    this->surface = surface;

    // Need to reset the search path, so that the next world can load it's own
    // surfaces
    QDir::setSearchPaths("surfaces", QStringList());

    emit surfaceChanged();
}

QVariant World::readSurface() const
{
    WorldSurface *o = const_cast<WorldSurface *>(this->surface);
    return QVariant::fromValue<QObject *>(o);
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
    if (this->terrainTypes != terrainTypes)
    {
        this->terrainTypes = terrainTypes;
        emit terrainTypesChanged();
    }
}

QVariant World::readTerrainTypes() const
{
    return QVariant::fromValue(toQObjectList<TerrainType>(this->terrainTypes));
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
    if (this->unitTypes != unitTypes)
    {
        this->unitTypes = unitTypes;
        emit unitTypesChanged();
    }
}

QVariant World::readUnitTypes() const
{
    return QVariant::fromValue(toQObjectList<UnitType>(this->unitTypes));
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
    if (this->settlementTypes != settlementTypes)
    {
        this->settlementTypes = settlementTypes;
        emit settlementTypesChanged();
    }
}

QVariant World::readSettlementTypes() const
{
    return QVariant::fromValue(toQObjectList<SettlementType>(this->settlementTypes));
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

void World::dataFromJson(const QJsonObject &obj)
{
    GameEntity::dataFromJson(obj);
    this->terrainTypes = newListFromJson<TerrainType>(obj["terrainTypes"].toArray(), this);
    this->unitClasses = newListFromJson<UnitClass>(obj["unitClasses"].toArray(), this);
    this->damageTypes = newListFromJson<DamageType>(obj["damageTypes"].toArray(), this);
    this->weapons = newListFromJson<Weapon>(obj["weapons"].toArray(), this);
    this->armors = newListFromJson<Armor>(obj["armors"].toArray(), this);
    this->unitTypes = newListFromJson<UnitType>(obj["unitTypes"].toArray(), this);
    this->settlementTypes = newListFromJson<SettlementType>(obj["settlementTypes"].toArray(), this);
    this->factions = newListFromJson<Faction>(obj["factions"].toArray(), this);
}

void World::dataToJson(QJsonObject &obj) const
{
    GameEntity::dataToJson(obj);
    obj["terrainTypes"] = listToJson<TerrainType>(this->terrainTypes);
    obj["unitClasses"] = listToJson<UnitClass>(this->unitClasses);
    obj["damageTypes"] = listToJson<DamageType>(this->damageTypes);
    obj["weapons"] = listToJson<Weapon>(this->weapons);
    obj["armors"] = listToJson<Armor>(this->armors);
    obj["unitTypes"] = listToJson<UnitType>(this->unitTypes);
    obj["settlementTypes"] = listToJson<SettlementType>(this->settlementTypes);
    obj["factions"] = listToJson<Faction>(this->factions);
}
