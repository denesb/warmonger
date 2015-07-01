#include "core/UnitClass.h"
#include "core/JsonUtil.hpp"

using namespace core;

UnitClass::UnitClass(QObject *parent) :
    WorldItem(parent),
    movementPoints(0),
    movements(),
    attacks(),
    defenses()
{
}

UnitClass::~UnitClass()
{
}

int UnitClass::getMovementPoints() const
{
    return this->movementPoints;
}

void UnitClass::setMovementPoints(int movementPoints)
{
    this->movementPoints = movementPoints;
}

QMap<TerrainType *, int> UnitClass::getMovements() const
{
    return this->movements;
}

void UnitClass::setMovements(const QMap<TerrainType *, int> &movements)
{
    this->movements = movements;
}

int UnitClass::getMovement(TerrainType *terrainType) const
{
    return this->movements[terrainType];
}

void UnitClass::setMovement(TerrainType *terrainType, int movement)
{
    this->movements[terrainType] = movement;
}

QMap<TerrainType *, int> UnitClass::getAttacks() const
{
    return this->attacks;
}

void UnitClass::setAttacks(const QMap<TerrainType *, int> &attacks)
{
    this->attacks = attacks;
}

int UnitClass::getAttack(TerrainType *terrainType) const
{
    return this->attacks[terrainType];
}

void UnitClass::setAttack(TerrainType *terrainType, int attack)
{
    this->attacks[terrainType] = attack;
}

QMap<TerrainType *, int> UnitClass::getDefenses() const
{
    return this->defenses;
}

void UnitClass::setDefenses(const QMap<TerrainType *, int> &defenses)
{
    this->defenses = defenses;
}

int UnitClass::getDefense(TerrainType *terrainType) const
{
    return this->defenses[terrainType];
}

void UnitClass::setDefense(TerrainType *terrainType, int defense)
{
    this->defenses[terrainType] = defense;
}

void UnitClass::fromJson(const QJsonObject &obj)
{
    WorldItem::fromJson(obj);
    this->movementPoints = obj["movementPoints"].toInt();
    this->movements = objectValueMapFromJson<TerrainType>(obj["terrainMovements"].toObject(), this);
    this->attacks = objectValueMapFromJson<TerrainType>(obj["terrainAttacks"].toObject(), this);
    this->defenses = objectValueMapFromJson<TerrainType>(obj["terrainDefenses"].toObject(), this);
}

QJsonObject UnitClass::toJson() const
{
    QJsonObject &&obj = WorldItem::toJson();

    obj["movementPoints"] = this->movementPoints;
    obj["terrainMovements"] = objectValueMapToJson(this->movements);
    obj["terrainAttacks"] = objectValueMapToJson(this->attacks);
    obj["terrainDefenses"] = objectValueMapToJson(this->defenses);

    return std::move(obj);
}

