#ifndef CORE_UNIT_TYPE_H
#define CORE_UNIT_TYPE_H

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class UnitClass;
class Armor;
class Weapon;

class UnitType :
    public GameObject
{
    Q_OBJECT;

public:
    enum UnitRank
    {
        Soldier = 0,
        Officer = 1,
        Leader = 2
    };

    UnitType(QObject *parent);
    ~UnitType();

    int getHitPoints() const;
    void setHitPoints(int hitPoints);

    UnitRank getUnitRank() const;
    void setUnitRank(UnitRank rank);

    const UnitClass * getUnitClass() const;
    void setUnitClass(const UnitClass *klass);

    int getLevel() const;
    void setLevel(int level);

    const Armor * getArmor() const;
    void setArmor(const Armor *armor);

    QList<const Weapon *> getWeapons() const;
    void setWeapons(const QList<const Weapon *> &weapons);

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    int hitPoints;
    UnitRank rank;
    const UnitClass *klass;
    int level;
    const Armor *armor;
    QList<const Weapon *> weapons;
};

} // namespace core
} // namespace warmonger

#endif // CORE_UNIT_TYPE_H
