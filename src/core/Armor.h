#ifndef CORE_ARMOR_H
#define CORE_ARMOR_H

#include <QMap>

#include "core/WorldItem.h"

namespace warmonger {
namespace core {

class DamageType;

class Armor :
    public WorldItem
{
    Q_OBJECT

public:
    Armor(QObject *parent);
    ~Armor();

    int getRange() const;
    void setRange(int range);

    QMap<const DamageType *, int> getDefenses() const;
    void setDefenses(const QMap<const DamageType *, int> &damages);

    int getDefense(const DamageType * const damageType) const;
    void setDefense(const DamageType * const damageType, int defense);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    int range;
    QMap<const DamageType *, int> defenses;
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_ARMOR_H