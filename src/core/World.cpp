#include <QFile>
#include <QJsonDocument>

#include "core/World.h"
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
    if (this->path != path)
    {
        this->path = path;
        emit pathChanged();
    }
}

QString World::getDescription() const
{
    return this->description;
}

void World::setDescription(const QString &description)
{
    if (this->description != description)
    {
        this->description = description;
        emit descriptionChanged();
    }
}

QSize World::getTileSize() const
{
    return this->tileSize;
}

void World::setTileSize(const QSize &tileSize)
{
    if (this->tileSize != tileSize)
    {
        this->tileSize = tileSize;
        emit tileSizeChanged();
    }
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

QMap<QString, QString> World::getResourcePaths() const
{
    return this->resourcePaths;
}

void World::setResourcePaths(const QMap<QString, QString> &resourcePaths)
{
    if (this->resourcePaths != resourcePaths)
    {
        this->resourcePaths = resourcePaths;
        emit resourcePathsChanged();
    }
}

QVariantMap World::readResourcePaths() const
{
    return this->toQVariantMap(this->resourcePaths);
}

QString World::getResourcePath(const QString &resourceName) const
{
    //TODO: un-hardcode!!!
    return this->path + QStringLiteral("/surfaces/default/") + this->resourcePaths[resourceName];
}

void World::dataFromJson(const QJsonObject &obj)
{
    this->description = obj["description"].toString();
    this->terrainTypes = newListFromJson<TerrainType>(obj["terrainTypes"].toArray(), this);
    this->tileSize = sizeFromJson(obj["tileSize"].toObject());
    this->unitClasses = newListFromJson<UnitClass>(obj["unitClasses"].toArray(), this);
    this->damageTypes = newListFromJson<DamageType>(obj["damageTypes"].toArray(), this);
    this->weapons = newListFromJson<Weapon>(obj["weapons"].toArray(), this);
    this->armors = newListFromJson<Armor>(obj["armors"].toArray(), this);
    this->unitTypes = newListFromJson<UnitType>(obj["unitTypes"].toArray(), this);
    this->settlementTypes = newListFromJson<SettlementType>(obj["settlementTypes"].toArray(), this);
    this->factions = newListFromJson<Faction>(obj["factions"].toArray(), this);

    //TODO: move this to it's own class
    QJsonDocument doc = loadJsonDocument(this->path + "/surfaces/default/surface.json");
    this->resourcePaths = this->mapFromJson(doc.object());
}

void World::dataToJson(QJsonObject &obj) const
{
    obj["description"] = this->description;
    obj["tileSize"] = sizeToJson(this->tileSize);
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

QVariantMap World::toQVariantMap(const QMap<QString, QString> &qmap) const
{
    const QString basePath = this->path + "/surfaces/default/";
    QVariantMap vmap;
    QMap<QString, QString>::ConstIterator it;
    for (it = qmap.constBegin(); it != qmap.constEnd(); it++)
    {
        const QString path(basePath + it.value());
        vmap.insert(it.key(), QVariant(path));
    }

    return std::move(vmap);
}

QMap<QString, QString> World::mapFromJson(const QJsonObject &obj) const
{
    QMap<QString, QString> map;
    QJsonObject::ConstIterator it;
    for (it = obj.constBegin(); it != obj.constEnd(); it++)
    {
        map.insert(it.key(), it.value().toString());
    }

    return std::move(map);
}

QJsonObject World::mapToJson(const QMap<QString, QString> &map) const
{
    QJsonObject obj;
    QMap<QString, QString>::ConstIterator it;
    for (it = map.constBegin(); it != map.constEnd(); it++)
    {
        obj[it.key()] = it.value();
    }

    return std::move(obj);
}
