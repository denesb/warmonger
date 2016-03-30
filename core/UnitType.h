#ifndef CORE_UNIT_TYPE_H
#define CORE_UNIT_TYPE_H

#include <map>
#include <vector>

#include <QObject>
#include <QVariant>

#include "core/Armor.h"
#include "core/UnitClass.h"
#include "core/UnitLevel.h"
#include "core/Weapon.h"

namespace warmonger {
namespace core {

class UnitType :
    public QObject
{
    Q_OBJECT;
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(UnitClass *klass READ getClass WRITE setClass NOTIFY classChanged)
    Q_PROPERTY(UnitLevel *level READ getLevel WRITE setLevel NOTIFY levelChanged)
    Q_PROPERTY(int hitPoints READ getHitPoints WRITE setHitPoints NOTIFY hitPointsChanged)
    Q_PROPERTY(int recruitmentCost READ getRecruitmentCost WRITE setRecruitmentCost NOTIFY recruitmentCostChanged)
    Q_PROPERTY(int upkeepCost READ getRecruitmentCost WRITE setRecruitmentCost NOTIFY upkeepCostChanged)
    Q_PROPERTY(Armor *armor READ getArmor WRITE setArmor NOTIFY armorChanged)
    Q_PROPERTY(QVariantList weapons READ readWeapons NOTIFY weaponsChanged)

public:
    explicit UnitType(QObject *parent=nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    UnitClass * getClass() const;
    void setClass(UnitClass *klass);

    UnitLevel * getLevel() const;
    void setLevel(UnitLevel *level);

    int getHitPoints() const;
    void setHitPoints(int hitPoints);

    int getRecruitmentCost() const;
    void setRecruitmentCost(int recruitmentCost);

    int getUpkeepCost() const;
    void setUpkeepCost(int upkeepCost);

    Armor * getArmor() const;
    void setArmor(Armor *armor);

    std::vector<Weapon *> getWeapons() const;
    void setWeapons(const std::vector<Weapon *> &weapons);

    QVariantList readWeapons() const;

    std::map<WeaponType *, int> getAttackSkills() const;
    void setAttackSkills(const std::map<WeaponType *, int> &attackSkills);

    int getAttackSkill(WeaponType *weaponType) const;

    std::map<WeaponClass *, int> getDefenseSkills() const;
    void setDefenseSkills(const std::map<WeaponClass *, int> &defenseSkills);

    int getDefenseSkill(WeaponClass *weaponClass) const;

    std::vector<UnitType *> getUpgrades() const;
    void setUpgrades(const std::vector<UnitType *> &upgrades);

signals:
    void displayNameChanged();
    void classChanged();
    void levelChanged();
    void hitPointsChanged();
    void recruitmentCostChanged();
    void upkeepCostChanged();
    void armorChanged();
    void weaponsChanged();
    void attackSkillsChanged();
    void defenseSkillsChanged();
    void upgradesChanged();

private:
    QString displayName;
    UnitClass *klass;
    UnitLevel *level;
    int hitPoints;
    int recruitmentCost;
    int upkeepCost;
    Armor *armor;
    std::vector<Weapon *> weapons;
    std::map<WeaponType *, int> attackSkills;
    std::map<WeaponClass *, int> defenseSkills;
    std::vector<UnitType *> upgrades;
};

} // namespace core
} // namespace warmonger

#endif // CORE_UNIT_TYPE_H
