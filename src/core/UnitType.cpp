#include "core/UnitType.h"
#include "core/UnitClass.h"
#include "core/Armor.h"
#include "core/Weapon.h"
#include "core/JsonUtil.h"

using namespace warmonger::core;

UnitType::UnitType(QObject *parent) :
    GameObject(parent),
    hitPoints(0),
    unitRank(UnitType::Soldier),
    unitClass(nullptr),
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
    if (this->hitPoints != hitPoints)
    {
        this->hitPoints = hitPoints;
        emit hitPointsChanged();
    }
}

UnitType::UnitRank UnitType::getUnitRank() const
{
    return this->unitRank;
}

void UnitType::setUnitRank(UnitType::UnitRank unitRank)
{
    this->unitRank = unitRank;
}

UnitClass *UnitType::getUnitClass() const
{
    return this->unitClass;
}

void UnitType::setUnitClass(UnitClass *unitClass)
{
    if (this->unitClass != unitClass)
    {
        this->unitClass = unitClass;
        emit unitClassChanged();
    }
}

QObject * UnitType::readUnitClass() const
{
    return this->unitClass;
}

void UnitType::writeUnitClass(QObject *unitClass)
{
    UnitClass *uc = qobject_cast<UnitClass *>(unitClass);
    if (uc == nullptr)
    {
        wError(category) << "unitClass is null or has wrong type";
        throw Exception(Exception::InvalidValue);
    }

    this->setUnitClass(uc);
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

void UnitType::dataFromJson(const QJsonObject &obj)
{
    this->hitPoints = obj["hitPoints"].toInt();
    const QString unitRankStr = obj["rank"].toString();
    if (unitRankStr == "Soldier")
    {
        this->unitRank = UnitType::Soldier;
    }
    else if (unitRankStr == "Officer")
    {
        this->unitRank = UnitType::Officer;
    }
    else if (unitRankStr == "Leader")
    {
        this->unitRank = UnitType::Leader;
    }
    else
    {
        wError("core.UnitType") << "Invalid unit rank: " << unitRankStr;
        throw Exception(Exception::InvalidValue, {unitRankStr});
    }
    this->unitClass = this->parent()->findChild<UnitClass *>(obj["class"].toString());
    this->level = obj["level"].toInt();
    this->armor = this->parent()->findChild<Armor *>(obj["armor"].toString());
    this->weapons = referenceListFromJson<Weapon>(obj["weapons"].toArray(), this);
}

void UnitType::dataToJson(QJsonObject &obj) const
{
    obj["hitPoints"] = this->hitPoints;
    obj["rank"] = this->unitRank;
    obj["class"] = this->unitClass->objectName();
    obj["level"] = this->level;
    obj["armor"] = this->armor->objectName();
    obj["weapons"] = referenceListToJson<Weapon>(this->weapons);
}
