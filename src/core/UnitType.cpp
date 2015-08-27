#include "core/UnitType.h"
#include "core/UnitClass.h"
#include "core/Armor.h"
#include "core/Weapon.h"
#include "core/JsonUtil.h"

using namespace warmonger::core;

UnitType::UnitType(QObject *parent) :
    GameObject(parent),
    hitPoints(0),
    rank(UnitType::Soldier),
    klass(nullptr),
    level(0),
    armor(nullptr),
    weapons()
{
}

UnitType::~UnitType()
{
}

int UnitType::getHitPoints() const
{
    return this->hitPoints;
}

void UnitType::setHitPoints(int hitPoints)
{
    this->hitPoints = hitPoints;
}

UnitType::UnitRank UnitType::getUnitRank() const
{
    return this->rank;
}

void UnitType::setUnitRank(UnitType::UnitRank rank)
{
    this->rank = rank;
}

const UnitClass *UnitType::getUnitClass() const
{
    return this->klass;
}

void UnitType::setUnitClass(const UnitClass *klass)
{
    this->klass = klass;
}

int UnitType::getLevel() const
{
    return this->level;
}

void UnitType::setLevel(int level)
{
    this->level = level;
}

const Armor * UnitType::getArmor() const
{
    return this->armor;
}

void UnitType::setArmor(const Armor *armor)
{
    this->armor = armor;
}

QList<const Weapon *> UnitType::getWeapons() const
{
    return this->weapons;
}

void UnitType::setWeapons(const QList<const Weapon *> &weapons)
{
    this->weapons = weapons;
}

void UnitType::dataFromJson(const QJsonObject &obj)
{
    this->hitPoints = obj["hitPoints"].toInt();
    const QString rankStr = obj["rank"].toString();
    if (rankStr == "Soldier")
    {
        this->rank = UnitType::Soldier;
    }
    else if (rankStr == "Officer")
    {
        this->rank = UnitType::Officer;
    }
    else if (rankStr == "Leader")
    {
        this->rank = UnitType::Leader;
    }
    else
    {
        wError("core.UnitType") << "Invalid unit rank: " << rankStr;
        throw Exception(Exception::InvalidValue, {rankStr});
    }
    this->klass = this->parent()->findChild<UnitClass *>(obj["class"].toString());
    this->level = obj["level"].toInt();
    this->armor = this->parent()->findChild<Armor *>(obj["armor"].toString());
    this->weapons = referenceListFromJson<Weapon>(obj["weapons"].toArray(), this);
}

void UnitType::dataToJson(QJsonObject &obj) const
{
    obj["hitPoints"] = this->hitPoints;
    obj["rank"] = this->rank;
    obj["class"] = this->klass->objectName();
    obj["level"] = this->level;
    obj["armor"] = this->armor->objectName();
    obj["weapons"] = referenceListToJson<Weapon>(this->weapons);
}
