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
    Q_PROPERTY(int hitPoints READ getHitPoints WRITE setHitPoints NOTIFY hitPointsChanged)
    Q_PROPERTY(QObject * unitClass READ readUnitClass WRITE writeUnitClass NOTIFY unitClassChanged)

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
    void setUnitRank(UnitRank unitRank);

    UnitClass * getUnitClass() const;
    void setUnitClass(UnitClass *unitClass);
    QObject * readUnitClass() const;
    void writeUnitClass(QObject *unitClass);

    int getLevel() const;
    void setLevel(int level);

    Armor * getArmor() const;
    void setArmor(Armor *armor);

    QList<Weapon *> getWeapons() const;
    void setWeapons(const QList<Weapon *> &weapons);

signals:
    void hitPointsChanged();
    void unitClassChanged();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    int hitPoints;
    UnitRank unitRank;
    UnitClass *unitClass;
    int level;
    Armor *armor;
    QList<Weapon *> weapons;
};

} // namespace core
} // namespace warmonger

#endif // CORE_UNIT_TYPE_H
