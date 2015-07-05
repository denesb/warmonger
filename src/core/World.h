#ifndef CORE_WORLD_H
#define CORE_WORLD_H

#include "core/TerrainType.h"
#include "core/UnitClass.h"
#include "core/DamageType.h"
#include "core/Weapon.h"
#include "core/Armor.h"
#include "core/UnitType.h"
#include "core/Faction.h"

namespace warmonger {
namespace core {

class World :
    public WorldItem
{
    Q_OBJECT

public:
    static const QString WorldDefinitionFile;

    World(QObject *parent = nullptr);

    int getVillageGoldPt() const;
    void setVillageGoldPt(int villageGoldPt);

    int getCityGoldPt() const;
    void setCityGoldPt(int cityGoldPt);

    QString getDescription() const;
    void setDescription(const QString &description);

    QList<TerrainType *> getTerrainTypes() const;
    void setTerrainTypes(const QList<TerrainType *> &terrainTypes);

    QList<UnitClass *> getUnitClasses() const;
    void setUnitClasses(const QList<UnitClass *> &unitClasses);

    QList<DamageType *> getDamageTypes() const;
    void setDamageTypes(const QList<DamageType *> &damageTypes);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    QString description;
    int villageGoldPt;
    int cityGoldPt;
    QList<TerrainType *> terrainTypes;
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
