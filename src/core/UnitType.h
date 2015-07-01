#ifndef CORE_UNIT_TYPE_H
#define CORE_UNIT_TYPE_H

#include "core/WorldItem.h"
#include "core/UnitClass.h"
#include "core/Armor.h"
#include "core/Weapon.h"

namespace core {

class UnitType :
    public WorldItem
{
    Q_OBJECT;

public:
    enum UnitRank
    {
        Soldier = 0,
        Officer = 1,
        Leader = 2
    };

    UnitType(QObject *parent = nullptr);
    ~UnitType();

    int getHitpoints() const;
    void setHitpoints(int hitpoints);

    UnitRank getUnitRank() const;
    void setUnitRank(UnitRank rank);

    UnitClass *getUnitClass() const;
    void setUnitClass(UnitClass *klass);

    int getLevel() const;
    void setLevel(int level);

    Armor * getArmor() const;
    void setArmor(Armor *armor);

    QList<Weapon *> getWeapons() const;
    void setWeapons(const QList<Weapon *> &weapons);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    int hitpoints;
    UnitRank rank;
    UnitClass *klass;
    int level;
    Armor *armor;
    QList<Weapon *> weapons;
};

}; // namespace core

#endif // CORE_UNIT_TYPE_H
