#include "core/UnitType.h"
#include "core/JsonUtil.hpp"

using namespace warmonger::core;

UnitType::UnitType(QObject *parent) :
    WorldItem(parent),
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

UnitClass *UnitType::getUnitClass() const
{
    return this->klass;
}

void UnitType::setUnitClass(UnitClass *klass)
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

Armor * UnitType::getArmor() const
{
    return this->armor;
}

void UnitType::setArmor(Armor *armor)
{
    this->armor = armor;
}

QList<Weapon *> UnitType::getWeapons() const
{
    return this->weapons;
}

void UnitType::setWeapons(const QList<Weapon *> &weapons)
{
    this->weapons = weapons;
}

void UnitType::fromJson(const QJsonObject &obj)
{
    WorldItem::fromJson(obj);

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
        //TODO: error handling
    }
    this->klass = this->parent()->findChild<UnitClass *>(obj["class"].toString());
    this->level = obj["level"].toInt();
    this->armor = this->parent()->findChild<Armor *>(obj["armor"].toString());
    this->weapons = referenceListFromJson<Weapon>(obj["weapons"].toArray(), this);
}

QJsonObject UnitType::toJson() const
{
    QJsonObject &&obj = WorldItem::toJson();

    obj["hitPoints"] = this->hitPoints;
    obj["rank"] = this->rank;
    obj["class"] = this->klass->objectName();
    obj["level"] = this->level;
    obj["armor"] = this->armor->objectName();
    obj["weapons"] = referenceListToJson<Weapon>(this->weapons);

    return std::move(obj);
}
