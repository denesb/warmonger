#ifndef CORE_WEAPON_H
#define CORE_WEAPON_H

#include <map>

#include <QObject>

#include "core/DamageType.h"
#include "core/WeaponType.h"

namespace warmonger {
namespace core {

class Weapon :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)

public:
    explicit Weapon(QObject *parent=nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    int getRange() const;
    void setRange(int range);

    std::map<const DamageType *, int> getDamages() const;
    void setDamages(const std::map<const DamageType *, int> &damages);

    int getDamage(const DamageType * const damageType) const;
    void setDamage(const DamageType * const damageType, int damage);

    WeaponType * getType() const;
    void setType(WeaponType *type);

signals:
    void displayNameChanged();
    void typeChanged();

private:
    QString displayName;
    int range;
    std::map<const DamageType *, int> damages;
    WeaponType *type;
};

} // namespace core
} // namespace warmonger

#endif // CORE_WEAPON_H
