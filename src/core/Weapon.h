#ifndef CORE_WEAPON_H
#define CORE_WEAPON_H

#include <QMap>

#include "core/WorldItem.h"
#include "core/DamageType.h"

namespace warmonger {
namespace core {

class Weapon :
    public WorldItem
{
    Q_OBJECT

public:
    Weapon(QObject *parent = nullptr);
    ~Weapon();

    int getRange() const;
    void setRange(int range);

    QMap<DamageType *, int> getDamages() const;
    void setDamages(const QMap<DamageType *, int> &damages);

    int getDamage(DamageType * const damageType) const;
    void setDamage(DamageType * const damageType, int damage);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    int range;
    QMap<DamageType *, int> damages;
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_WEAPON_H
