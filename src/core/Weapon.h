#ifndef CORE_WEAPON_H
#define CORE_WEAPON_H

#include <QMap>

#include "core/WorldItem.h"

namespace warmonger {
namespace core {

class DamageType;

class Weapon :
    public WorldItem
{
    Q_OBJECT

public:
    Weapon(QObject *parent);
    ~Weapon();

    int getRange() const;
    void setRange(int range);

    QMap<const DamageType *, int> getDamages() const;
    void setDamages(const QMap<const DamageType *, int> &damages);

    int getDamage(const DamageType * const damageType) const;
    void setDamage(const DamageType * const damageType, int damage);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    int range;
    QMap<const DamageType *, int> damages;
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_WEAPON_H
