#ifndef CORE_UNIT_TYPE_H
#define CORE_UNIT_TYPE_H

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class UnitClass;
class UnitLevel;
class Armor;
class Weapon;

class UnitType :
    public GameObject
{
    Q_OBJECT;
    Q_PROPERTY(int hitPoints READ getHitPoints WRITE setHitPoints NOTIFY hitPointsChanged)
    Q_PROPERTY(QObject * unitClass READ readUnitClass WRITE writeUnitClass NOTIFY unitClassChanged)

public:
    UnitType(QObject *parent);
    ~UnitType();

    int getHitPoints() const;
    void setHitPoints(int hitPoints);

    int getExperiencePoints() const;
    void setExperiencePoints(int experiencePoints);

    UnitLevel * getLevel() const;
    void setLevel(UnitLevel *level);

    UnitClass * getUnitClass() const;
    void setUnitClass(UnitClass *unitClass);
    QObject * readUnitClass() const;
    void writeUnitClass(QObject *unitClass);

    Armor * getArmor() const;
    void setArmor(Armor *armor);

    QList<Weapon *> getWeapons() const;
    void setWeapons(const QList<Weapon *> &weapons);

    QList<UnitType *> getUpgrades() const;
    void setUpgrades(const QList<UnitType *> &upgrades);

signals:
    void hitPointsChanged();
    void levelChanged();
    void unitClassChanged();
    void armorChanged();
    void weaponsChanged();
    void upgradesChanged();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    int hitPoints;
    int experiencePoints;
    UnitLevel *level;
    UnitClass *unitClass;
    Armor *armor;
    QList<Weapon *> weapons;
    QList<UnitType *> upgrades;
};

} // namespace core
} // namespace warmonger

#endif // CORE_UNIT_TYPE_H
