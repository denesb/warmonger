#ifndef CORE_UNIT_TYPE_H
#define CORE_UNIT_TYPE_H

#include "core/Armor.h"
#include "core/GameObject.h"
#include "core/UnitClass.h"
#include "core/UnitLevel.h"
#include "core/Weapon.h"

namespace warmonger {
namespace core {

class UnitType :
    public GameObject
{
    Q_OBJECT;
    Q_PROPERTY(int hitPoints READ getHitPoints WRITE setHitPoints NOTIFY hitPointsChanged)
    Q_PROPERTY(UnitClass *klass READ getClass WRITE setClass NOTIFY classChanged)
    Q_PROPERTY(UnitLevel *level READ getLevel WRITE setLevel NOTIFY levelChanged)

public:
    UnitType(QObject *parent);
    ~UnitType();

    int getHitPoints() const;
    void setHitPoints(int hitPoints);

    UnitLevel * getLevel() const;
    void setLevel(UnitLevel *level);

    UnitClass * getClass() const;
    void setClass(UnitClass *klass);

    Armor * getArmor() const;
    void setArmor(Armor *armor);

    QList<Weapon *> getWeapons() const;
    void setWeapons(const QList<Weapon *> &weapons);

    QList<UnitType *> getUpgrades() const;
    void setUpgrades(const QList<UnitType *> &upgrades);

signals:
    void hitPointsChanged();
    void levelChanged();
    void classChanged();
    void armorChanged();
    void weaponsChanged();
    void upgradesChanged();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    int hitPoints;
    UnitLevel *level;
    UnitClass *klass;
    Armor *armor;
    QList<Weapon *> weapons;
    QList<UnitType *> upgrades;
};

} // namespace core
} // namespace warmonger

#endif // CORE_UNIT_TYPE_H
