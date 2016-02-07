#include "core/Armor.h"
#include "core/DamageType.h"
#include "core/QJsonUtil.h"

using namespace warmonger::core;

Armor::Armor(QObject *parent) :
    GameObject(parent),
    defenses()
{
}

Armor::~Armor()
{
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
    this->defenses = fromQJsonObject<QMap<const DamageType *, int>>(
        obj["defenses"].toObject(),
        ReferenceResolver<DamageType>(this->parent()),
        qJsonValueToInt
    );
}

void Armor::dataToJson(QJsonObject &obj) const
{
    obj["defenses"] = toQJsonObject(
        this->defenses,
        qObjectName,
        constructQJsonValue<int>
    );
}
