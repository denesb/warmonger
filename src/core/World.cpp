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
#include "core/JsonUtil.h"

using namespace warmonger::core;

static const QString category{"core"};

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

    QDir worldDir(this->path + "/surfaces");
    QStringList entryList = worldDir.entryList(
        QDir::AllDirs | QDir::NoDotAndDotDot
    );

    const QString rootPath = worldDir.absolutePath() + "/";
    QStringList searchPaths;
    for (const QString dirEntry : entryList)
    {
        searchPaths << rootPath + dirEntry;
    }

    QDir::setSearchPaths("surfaces", searchPaths);

    this->surface = new WorldSurface(this);
    this->surface->load(surface->specification(surfaceName));

    // Need to reset the search path, so that the next world can load it's own
    // surfaces
    QDir::setSearchPaths("surfaces", QStringList());

    emit surfaceChanged();
}

QObject * World::readSurface() const
{
    return this->surface;
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

QList<DamageType *> World::getDamageTypes() const
{
    return this->damageTypes;
}

void World::setDamageTypes(const QList<DamageType *> &damageTypes)
{
    this->damageTypes = damageTypes;
}

QList<Armor *> World::getArmors() const
{
    return this->armors;
}

void World::setArmors(const QList<Armor *> &armors)
{
    this->armors = armors;
}

QList<Weapon *> World::getWeapons() const
{
    return this->weapons;
}

void World::setWeapons(const QList<Weapon *> &weapons)
{
    this->weapons = weapons;
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
