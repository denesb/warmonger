#ifndef CORE_WORLD_H
#define CORE_WORLD_H

#include <vector>

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

    std::vector<Armor *> getArmors() const;
    void setArmors(const std::vector<Armor *> &armors);

    std::vector<DamageType *> getDamageTypes() const;
    void setDamageTypes(const std::vector<DamageType *> &damageTypes);

    std::vector<Faction *> getFactions() const;
    void setFactions(const std::vector<Faction *> &factions);
    QVariantList readFactions() const;

    std::vector<SettlementType *> getSettlementTypes() const;
    void setSettlementTypes(const std::vector<SettlementType *> &settlementTypes);
    QVariantList readSettlementTypes() const;

    std::vector<TerrainType *> getTerrainTypes() const;
    void setTerrainTypes(const std::vector<TerrainType *> &terrainTypes);
    QVariantList readTerrainTypes() const;

    std::vector<UnitClass *> getUnitClasses() const;
    void setUnitClasses(const std::vector<UnitClass *> &unitClasses);

    std::vector<UnitLevel *> getUnitLevels() const;
    void setUnitLevels(const std::vector<UnitLevel *> &unitLevels);
    QVariantList readUnitLevels() const;

    std::vector<UnitType *> getUnitTypes() const;
    void setUnitTypes(const std::vector<UnitType *> &unitTypes);
    QVariantList readUnitTypes() const;

    std::vector<Weapon *> getWeapons() const;
    void setWeapons(const std::vector<Weapon *> &weapons);

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
    std::vector<DamageType *> damageTypes;
    std::vector<Armor *> armors;
    std::vector<Weapon *> weapons;
    std::vector<TerrainType *> terrainTypes;
    std::vector<UnitClass *> unitClasses;
    std::vector<UnitLevel *> unitLevels;
    std::vector<UnitType *> unitTypes;
    std::vector<SettlementType *> settlementTypes;
    std::vector<Faction *> factions;
};

} // namespace core
} // namespace warmonger

#endif // CORE_WORLD_H
