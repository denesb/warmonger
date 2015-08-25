#ifndef CORE_WORLD_H
#define CORE_WORLD_H

#include <QList>

#include "core/GameEntity.h"

namespace warmonger {
namespace core {

class WorldSurface;
class TerrainType;
class UnitClass;
class DamageType;
class Weapon;
class Armor;
class UnitType;
class SettlementType;
class Faction;

class World :
    public GameEntity
{
    Q_OBJECT
    Q_PROPERTY(QVariant surface READ readSurface NOTIFY surfaceChanged)
    Q_PROPERTY(QVariant terrainTypes READ readTerrainTypes NOTIFY terrainTypesChanged)
    Q_PROPERTY(QVariant unitTypes READ readUnitTypes NOTIFY unitTypesChanged)
    Q_PROPERTY(QVariant settlementTypes READ readSettlementTypes NOTIFY settlementTypesChanged)

public:
    World(QObject *parent);

    virtual QString specification(const QString &objectName) const;

    const WorldSurface * getSurface() const;
    void setSurface(const WorldSurface *surface) const;
    void setSurface(const QString &surfaceName) const;
    QVariant readSurface() const;

    QList<const TerrainType *> getTerrainTypes() const;
    QList<TerrainType *> getTerrainTypes();
    void setTerrainTypes(const QList<TerrainType *> &terrainTypes);
    QVariant readTerrainTypes() const;

    QList<const UnitClass *> getUnitClasses() const;
    QList<UnitClass *> getUnitClasses();
    void setUnitClasses(const QList<UnitClass *> &unitClasses);

    QList<const DamageType *> getDamageTypes() const;
    QList<DamageType *> getDamageTypes();
    void setDamageTypes(const QList<DamageType *> &damageTypes);

    QList<const Armor *> getArmors() const;
    QList<Armor *> getArmors();
    void setArmors(const QList<Armor *> &armors);

    QList<const Weapon *> getWeapons() const;
    QList<Weapon *> getWeapons();
    void setWeapons(const QList<Weapon *> &weapons);

    QList<const UnitType *> getUnitTypes() const;
    QList<UnitType *> getUnitTypes();
    void setUnitTypes(const QList<UnitType *> &unitTypes);
    QVariant readUnitTypes() const;

    QList<const SettlementType *> getSettlementTypes() const;
    QList<SettlementType *> getSettlementTypes();
    void setSettlementTypes(const QList<SettlementType *> &settlementTypes);
    QVariant readSettlementTypes() const;

    QList<const Faction *> getFactions() const;
    QList<Faction *> getFactions();
    void setFactions(const QList<Faction *> &factions);

signals:
    void surfaceChanged() const;
    void terrainTypesChanged() const;
    void unitTypesChanged() const;
    void settlementTypesChanged() const;

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    mutable const WorldSurface *surface;
    QList<TerrainType *> terrainTypes;
    QList<UnitClass *> unitClasses;
    QList<DamageType *> damageTypes;
    QList<Weapon *> weapons;
    QList<Armor *> armors;
    QList<UnitType *> unitTypes;
    QList<SettlementType *> settlementTypes;
    QList<Faction *> factions;
};

} // namespace core
} // namespace warmonger

#endif // CORE_WORLD_H
