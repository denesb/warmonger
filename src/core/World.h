#ifndef CORE_WORLD_H
#define CORE_WORLD_H

#include <QList>

#include "core/WorldItem.h"

namespace warmonger {
namespace core {

class TerrainType;
class UnitClass;
class DamageType;
class Weapon;
class Armor;
class UnitType;
class SettlementType;
class Faction;

class World :
    public WorldItem
{
    Q_OBJECT

public:
    static const QString WorldDefinitionFile;

    World(QObject *parent = nullptr);
 
    QString getDescription() const;
    void setDescription(const QString &description);

    QList<TerrainType *> getTerrainTypes() const;
    void setTerrainTypes(const QList<TerrainType *> &terrainTypes);

    QList<UnitClass *> getUnitClasses() const;
    void setUnitClasses(const QList<UnitClass *> &unitClasses);

    QList<DamageType *> getDamageTypes() const;
    void setDamageTypes(const QList<DamageType *> &damageTypes);

    //TODO: getter/setter for Weapon, Armor

    QList<SettlementType *> getSettlementTypes() const;
    void setSettlementTypes(const QList<SettlementType *> &settlementTypes);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    QString description;
    QList<TerrainType *> terrainTypes;
    QList<SettlementType *> settlementTypes;
    QList<UnitClass *> unitClasses;
    QList<DamageType *> damageTypes;
    QList<Weapon *> weapons;
    QList<Armor *> armors;
    QList<UnitType *> unitTypes;
    QList<Faction *> factions;
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_WORLD_H
