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
    Q_PROPERTY(QObject * surface READ readSurface NOTIFY surfaceChanged)
    Q_PROPERTY(QVariantList terrainTypes READ readTerrainTypes NOTIFY terrainTypesChanged)
    Q_PROPERTY(QVariantList unitTypes READ readUnitTypes NOTIFY unitTypesChanged)
    Q_PROPERTY(QVariantList settlementTypes READ readSettlementTypes NOTIFY settlementTypesChanged)
    Q_PROPERTY(QVariantList factions READ readFactions NOTIFY factionsChanged)

public:
    Q_INVOKABLE World();

    Q_INVOKABLE virtual QString fileExtension() const;

    WorldSurface * getSurface() const;
    void setSurface(WorldSurface *surface);
    void setSurface(const QString &surfaceName);
    QObject * readSurface() const;

    QList<TerrainType *> getTerrainTypes() const;
    void setTerrainTypes(const QList<TerrainType *> &terrainTypes);
    QVariantList readTerrainTypes() const;

    QList<UnitClass *> getUnitClasses() const;
    void setUnitClasses(const QList<UnitClass *> &unitClasses);

    QList<DamageType *> getDamageTypes() const;
    void setDamageTypes(const QList<DamageType *> &damageTypes);

    QList<Armor *> getArmors() const;
    void setArmors(const QList<Armor *> &armors);

    QList<Weapon *> getWeapons() const;
    void setWeapons(const QList<Weapon *> &weapons);

    QList<UnitType *> getUnitTypes() const;
    void setUnitTypes(const QList<UnitType *> &unitTypes);
    QVariantList readUnitTypes() const;

    QList<SettlementType *> getSettlementTypes() const;
    void setSettlementTypes(const QList<SettlementType *> &settlementTypes);
    QVariantList readSettlementTypes() const;

    QList<Faction *> getFactions() const;
    void setFactions(const QList<Faction *> &factions);
    QVariantList readFactions() const;

signals:
    void surfaceChanged();
    void terrainTypesChanged();
    void unitTypesChanged();
    void settlementTypesChanged();
    void factionsChanged();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    WorldSurface *surface;
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
