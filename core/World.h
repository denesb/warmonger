#ifndef CORE_WORLD_H
#define CORE_WORLD_H

#include <QObject>
#include <QVariant>

#include "core/Armor.h"
#include "core/DamageType.h"
#include "core/Faction.h"
#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/UnitClass.h"
#include "core/UnitType.h"
#include "core/UnitLevel.h"
#include "core/Weapon.h"
#include "core/WorldSurface.h"

namespace warmonger {
namespace core {

class World :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(WorldSurface *surface READ getSurface NOTIFY surfaceChanged)
    Q_PROPERTY(QVariantList factions READ readFactions NOTIFY factionsChanged)
    Q_PROPERTY(QVariantList settlementTypes READ readSettlementTypes NOTIFY settlementTypesChanged)
    Q_PROPERTY(QVariantList terrainTypes READ readTerrainTypes NOTIFY terrainTypesChanged)
    Q_PROPERTY(QVariantList unitTypes READ readUnitTypes NOTIFY unitTypesChanged)

public:
    explicit World(QObject *parent=nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    WorldSurface * getSurface() const;
    void setSurface(WorldSurface *surface);

    QList<Armor *> getArmors() const;
    void setArmors(const QList<Armor *> &armors);

    QList<DamageType *> getDamageTypes() const;
    void setDamageTypes(const QList<DamageType *> &damageTypes);

    QList<Faction *> getFactions() const;
    void setFactions(const QList<Faction *> &factions);
    QVariantList readFactions() const;

    QList<SettlementType *> getSettlementTypes() const;
    void setSettlementTypes(const QList<SettlementType *> &settlementTypes);
    QVariantList readSettlementTypes() const;

    QList<TerrainType *> getTerrainTypes() const;
    void setTerrainTypes(const QList<TerrainType *> &terrainTypes);
    QVariantList readTerrainTypes() const;

    QList<UnitClass *> getUnitClasses() const;
    void setUnitClasses(const QList<UnitClass *> &unitClasses);

    QList<UnitLevel *> getUnitLevels() const;
    void setUnitLevels(const QList<UnitLevel *> &unitLevels);
    QVariantList readUnitLevels() const;

    QList<UnitType *> getUnitTypes() const;
    void setUnitTypes(const QList<UnitType *> &unitTypes);
    QVariantList readUnitTypes() const;

    QList<Weapon *> getWeapons() const;
    void setWeapons(const QList<Weapon *> &weapons);

signals:
    void displayNameChanged();
    void factionsChanged();
    void surfaceChanged();
    void settlementTypesChanged();
    void terrainTypesChanged();
    void unitClassesChanged();
    void unitLevelsChanged();
    void unitTypesChanged();

private:
    QString displayName;
    WorldSurface *surface;
    QList<DamageType *> damageTypes;
    QList<Armor *> armors;
    QList<Weapon *> weapons;
    QList<TerrainType *> terrainTypes;
    QList<UnitClass *> unitClasses;
    QList<UnitLevel *> unitLevels;
    QList<UnitType *> unitTypes;
    QList<SettlementType *> settlementTypes;
    QList<Faction *> factions;
};

} // namespace core
} // namespace warmonger

#endif // CORE_WORLD_H
