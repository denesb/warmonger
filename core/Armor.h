#ifndef CORE_ARMOR_H
#define CORE_ARMOR_H

#include <QMap>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class DamageType;

class Armor :
    public GameObject
{
    Q_OBJECT

public:
    Armor(QObject *parent=nullptr);
    ~Armor();

    QMap<const DamageType *, int> getDefenses() const;
    void setDefenses(const QMap<const DamageType *, int> &damages);

    int getDefense(const DamageType * const damageType) const;
    void setDefense(const DamageType * const damageType, int defense);

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    QMap<const DamageType *, int> defenses;
};

} // namespace core
} // namespace warmonger

#endif // CORE_ARMOR_H