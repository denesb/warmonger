#include "core/QVariantUtil.h"
#include "core/QJsonUtil.h"
#include "core/UnitType.h"

using namespace warmonger::core;

static const QString loggerName{"core.UnitType"};

UnitType::UnitType(QObject *parent) :
    GameObject(parent),
    hitPoints(0),
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
    this->hitPoints = obj["hitPoints"].toInt();
    this->level = resolveReference<UnitLevel>(
        obj["level"].toString(),
        this->parent()
    );
    this->klass = resolveReference<UnitClass>(
        obj["class"].toString(),
        this->parent()
    );
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
    obj["hitPoints"] = this->hitPoints;
    obj["level"] = this->level->objectName();
    obj["class"] = this->klass->objectName();
    obj["armor"] = this->armor->objectName();
    obj["weapons"] = toQJsonArray(this->weapons, qObjectName);
    obj["upgrades"] = toQJsonArray(this->upgrades, qObjectName);
}
