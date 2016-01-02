#include "core/QVariantUtil.h"
#include "core/QJsonUtil.h"
#include "core/UnitType.h"

using namespace warmonger::core;

static const QString loggerName{"core.UnitType"};

UnitType::UnitType(QObject *parent) :
    GameObject(parent),
    klass(nullptr),
    level(nullptr),
    hitPoints(0),
    recruitmentCost(0),
    upkeepCost(0),
    armor(nullptr),
    weapons(),
    upgrades()
{
}

UnitType::~UnitType()
{
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

UnitLevel * UnitType::getLevel() const
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

int UnitType::getRecruitmentCost() const
{
    return this->recruitmentCost;
}

void UnitType::setRecruitmentCost(int recruitmentCost)
{
    if (this->recruitmentCost != recruitmentCost)
    {
        this->recruitmentCost = recruitmentCost;
        emit recruitmentCostChanged();
    }
}

int UnitType::getUpkeepCost() const
{
    return this->upkeepCost;
}

void UnitType::getUpkeepCost(int upkeepCost)
{
    if (this->upkeepCost != upkeepCost)
    {
        this->upkeepCost = upkeepCost;
        emit upkeepCostChanged();
    }
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

QVariantList UnitType::readWeapons() const
{
    return toQVariantList(this->weapons);
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
    this->klass = resolveReference<UnitClass>(
        obj["class"].toString(),
        this->parent()
    );
    this->level = resolveReference<UnitLevel>(
        obj["level"].toString(),
        this->parent()
    );
    this->hitPoints = obj["hitPoints"].toInt();
    this->recruitmentCost = obj["recruitmentCost"].toInt();
    this->upkeepCost = obj["upkeepCost"].toInt();
    this->armor = resolveReference<Armor>(
        obj["armor"].toString(),
        this->parent()
    );
    this->weapons = fromQJsonArray<QList<Weapon *>>(
        obj["weapons"].toArray(),
        ReferenceResolver<Weapon>(this->parent())
    );
    this->upgrades = fromQJsonArray<QList<UnitType *>>(
        obj["upgrades"].toArray(),
        ReferenceResolver<UnitType>(this->parent())
    );
}

void UnitType::dataToJson(QJsonObject &obj) const
{
    obj["class"] = this->klass->objectName();
    obj["level"] = this->level->objectName();
    obj["hitPoints"] = this->hitPoints;
    obj["recruitmentCost"] = this->recruitmentCost;
    obj["upkeepCost"] = this->upkeepCost;
    obj["armor"] = this->armor->objectName();
    obj["weapons"] = toQJsonArray(this->weapons, qObjectName);
    obj["upgrades"] = toQJsonArray(this->upgrades, qObjectName);
}
