#include "core/Armor.h"
#include "core/DamageType.h"
#include "core/JsonUtil.hpp"

using namespace warmonger::core;

Armor::Armor(QObject *parent) :
    GameObject(parent)
{
}

Armor::~Armor()
{
}

int Armor::getRange() const
{
    return this->range;
}

void Armor::setRange(int range)
{
    this->range = range;
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

void Armor::dataFromJson(const QJsonObject &obj)
{
    this->range = obj["range"].toInt();
    this->defenses = objectValueMapFromJson<DamageType>(obj["defenses"].toObject(), this);
}

void Armor::dataToJson(QJsonObject &obj) const
{
    obj["range"] = this->range;
    obj["defenses"] = objectValueMapToJson<DamageType>(this->defenses);
}
