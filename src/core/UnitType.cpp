#include "core/Armor.h"
#include "core/JsonUtil.h"
#include "core/UnitClass.h"
#include "core/UnitLevel.h"
#include "core/UnitType.h"
#include "core/Weapon.h"

using namespace warmonger::core;

static const QString category{"core"};

UnitType::UnitType(QObject *parent) :
    GameObject(parent),
    hitPoints(0),
    experiencePoints(0),
    level(nullptr),
    klass(nullptr),
    armor(nullptr),
    weapons(),
    upgrades()
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

UnitLevel * UnitType::getLevel() const
{
    return this->level;
}

QObject * UnitType::readLevel() const
{
    return this->level;
}

void UnitType::setLevel(UnitLevel *level)
{
    if (this->level != level)
    {
        this->level = level;
        emit levelChanged();
    }
}

UnitClass *UnitType::getClass() const
{
    return this->klass;
}

void UnitType::setClass(UnitClass *klass)
{
    if (this->klass != klass)
    {
        this->klass = klass;
        emit classChanged();
    }
}

QObject * UnitType::readClass() const
{
    return this->klass;
}

void UnitType::writeClass(QObject *klass)
{
    UnitClass *uc = qobject_cast<UnitClass *>(klass);
    if (uc == nullptr)
    {
        wError(category) << "klass is null or has wrong type";
        throw Exception(Exception::InvalidValue);
    }

    this->setClass(uc);
}

Armor * UnitType::getArmor() const
{
    return this->armor;
}

void UnitType::setArmor(Armor *armor)
{
    if (this->armor != armor)
    {
        this->armor = armor;
        emit armorChanged();
    }
}

QList<Weapon *> UnitType::getWeapons() const
{
    return this->weapons;
}

void UnitType::setWeapons(const QList<Weapon *> &weapons)
{
    if (this->weapons != weapons)
    {
        this->weapons = weapons;
        emit weaponsChanged();
    }
}


QList<UnitType *> UnitType::getUpgrades() const
{
    return this->upgrades;
}

void UnitType::setUpgrades(const QList<UnitType *> &upgrades)
{
    if (this->upgrades != upgrades)
    {
        this->upgrades = upgrades;
        emit upgradesChanged();
    }
}

void UnitType::dataFromJson(const QJsonObject &obj)
{
    this->hitPoints = obj["hitPoints"].toInt();
    this->experiencePoints = obj["experiencePoints"].toInt();
    this->level = this->parent()->findChild<UnitLevel *>(obj["level"].toString());
    this->klass = this->parent()->findChild<UnitClass *>(obj["class"].toString());
    this->armor = this->parent()->findChild<Armor *>(obj["armor"].toString());
    this->weapons = referenceListFromJson<Weapon>(obj["weapons"].toArray(), this);
    this->upgrades = referenceListFromJson<UnitType>(obj["upgrades"].toArray(), this);
}

void UnitType::dataToJson(QJsonObject &obj) const
{
    obj["hitPoints"] = this->hitPoints;
    obj["experiencePoints"] = this->experiencePoints;
    obj["level"] = this->level->objectName();
    obj["class"] = this->klass->objectName();
    obj["armor"] = this->armor->objectName();
    obj["weapons"] = referenceListToJson<Weapon>(this->weapons);
    obj["upgrades"] = referenceListToJson<UnitType>(this->upgrades);
}
