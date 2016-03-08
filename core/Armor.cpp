#include "core/Armor.h"
#include "core/DamageType.h"

using namespace warmonger::core;

Armor::Armor(QObject *parent) :
    QObject(parent),
    defenses()
{
}

Armor::~Armor()
{
}

QString Armor::getDisplayName() const
{
    return this->displayName;
}

void Armor::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

QMap<const DamageType *, int> Armor::getDefenses() const
{
    return this->defenses;
}

void Armor::setDefenses(const QMap<const DamageType *, int> &defenses)
{
    this->defenses = defenses;
}

int Armor::getDefense(const DamageType * const damageType) const
{
    return this->defenses[damageType];
}

void Armor::setDefense(const DamageType * const damageType, int defense)
{
    this->defenses[damageType] = defense;
}
