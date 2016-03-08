#ifndef CORE_UNIT_TYPE_H
#define CORE_UNIT_TYPE_H

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
    ~UnitType();

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

    QList<Weapon *> getWeapons() const;
    void setWeapons(const QList<Weapon *> &weapons);

    QVariantList readWeapons() const;

    QList<UnitType *> getUpgrades() const;
    void setUpgrades(const QList<UnitType *> &upgrades);

signals:
    void displayNameChanged();
    void classChanged();
    void levelChanged();
    void hitPointsChanged();
    void recruitmentCostChanged();
    void upkeepCostChanged();
    void armorChanged();
    void weaponsChanged();
    void upgradesChanged();

private:
    QString displayName;
    UnitClass *klass;
    UnitLevel *level;
    int hitPoints;
    int recruitmentCost;
    int upkeepCost;
    Armor *armor;
    QList<Weapon *> weapons;
    QList<UnitType *> upgrades;
};

} // namespace core
} // namespace warmonger

#endif // CORE_UNIT_TYPE_H
