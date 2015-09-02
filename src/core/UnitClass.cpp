#include "core/UnitClass.h"
#include "core/TerrainType.h"
#include "core/JsonUtil.h"

using namespace warmonger::core;

UnitClass::UnitClass(QObject *parent) :
    GameObject(parent),
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
    if (this->movementPoints != movementPoints)
    {
        this->movementPoints = movementPoints;
        emit movementPointsChanged();
    }
}

QMap<const TerrainType *, int> UnitClass::getMovements() const
{
    return this->movements;
}

void UnitClass::setMovements(const QMap<const TerrainType *, int> &movements)
{
    this->movements = movements;
}

int UnitClass::getMovement(const TerrainType *terrainType) const
{
    return this->movements[terrainType];
}

void UnitClass::setMovement(const TerrainType *terrainType, int movement)
{
    this->movements[terrainType] = movement;
}

QMap<const TerrainType *, int> UnitClass::getAttacks() const
{
    return this->attacks;
}

void UnitClass::setAttacks(const QMap<const TerrainType *, int> &attacks)
{
    this->attacks = attacks;
}

int UnitClass::getAttack(const TerrainType *terrainType) const
{
    return this->attacks[terrainType];
}

void UnitClass::setAttack(const TerrainType *terrainType, int attack)
{
    this->attacks[terrainType] = attack;
}

QMap<const TerrainType *, int> UnitClass::getDefenses() const
{
    return this->defenses;
}

void UnitClass::setDefenses(const QMap<const TerrainType *, int> &defenses)
{
    this->defenses = defenses;
}

int UnitClass::getDefense(const TerrainType *terrainType) const
{
    return this->defenses[terrainType];
}

void UnitClass::setDefense(const TerrainType *terrainType, int defense)
{
    this->defenses[terrainType] = defense;
}

void UnitClass::dataFromJson(const QJsonObject &obj)
{
    this->movementPoints = obj["movementPoints"].toInt();
    this->movements = objectValueMapFromJson<const TerrainType>(obj["terrainMovements"].toObject(), this);
    this->attacks = objectValueMapFromJson<const TerrainType>(obj["terrainAttacks"].toObject(), this);
    this->defenses = objectValueMapFromJson<const TerrainType>(obj["terrainDefenses"].toObject(), this);
}

void UnitClass::dataToJson(QJsonObject &obj) const
{
    obj["movementPoints"] = this->movementPoints;
    obj["terrainMovements"] = objectValueMapToJson(this->movements);
    obj["terrainAttacks"] = objectValueMapToJson(this->attacks);
    obj["terrainDefenses"] = objectValueMapToJson(this->defenses);
}

