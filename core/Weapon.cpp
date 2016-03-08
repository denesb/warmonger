#include "core/DamageType.h"
#include "core/Weapon.h"

using namespace warmonger::core;

Weapon::Weapon(QObject *parent) :
    QObject(parent),
    range(),
    damages()
{
}

QString Weapon::getDisplayName() const
{
    return this->displayName;
}

void Weapon::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

int Weapon::getRange() const
{
    return this->range;
}

void Weapon::setRange(int range)
{
    this->range = range;
}

QMap<const DamageType *, int> Weapon::getDamages() const
{
    return this->damages;
}

void Weapon::setDamages(const QMap<const DamageType *, int> &damages)
{
    this->damages = damages;
}

int Weapon::getDamage(const DamageType * const damageType) const
{
    return this->damages[damageType];
}

void Weapon::setDamage(const DamageType *const damageType, int damage)
{
    this->damages[damageType] = damage;
}
