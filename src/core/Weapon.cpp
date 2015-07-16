#include "core/Weapon.h"
#include "core/DamageType.h"
#include "core/JsonUtil.hpp"

using namespace warmonger::core;

Weapon::Weapon(QObject *parent) :
    GameObject(parent),
    damages()
{
}

Weapon::~Weapon()
{
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

void Weapon::dataFromJson(const QJsonObject &obj)
{
    this->range = obj["range"].toInt();
    this->damages = objectValueMapFromJson<DamageType>(obj["damages"].toObject(), this);
}

void Weapon::dataToJson(QJsonObject &obj) const
{
    obj["range"] = this->range;
    obj["damages"] = objectValueMapToJson<DamageType>(this->damages);
}
