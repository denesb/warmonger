#include "core/QJsonUtil.h"
#include "core/UnitClass.h"

using namespace warmonger::core;

UnitClass::UnitClass(QObject *parent) :
    GameObject(parent),
    movementPoints(0),
    movementCosts(),
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

QMap<const TerrainType *, int> UnitClass::getMovementCosts() const
{
    return this->movementCosts;
}

void UnitClass::setMovementCosts(const QMap<const TerrainType *, int> &movementCosts)
{
    this->movementCosts = movementCosts;
}

int UnitClass::getMovementCost(const TerrainType *terrainType) const
{
    return this->movementCosts[terrainType];
}

void UnitClass::setMovementCost(const TerrainType *terrainType, int movement)
{
    this->movementCosts[terrainType] = movement;
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

    ReferenceResolver<TerrainType> terrainTypeReferenceResolver(
        this->parent()
    );
    this->movementCosts = fromQJsonObject<QMap<const TerrainType *, int>>(
        obj["terrainMovementCosts"].toObject(),
        terrainTypeReferenceResolver,
        qJsonValueToInt
    );
    this->attacks = fromQJsonObject<QMap<const TerrainType *, int>>(
        obj["terrainAttacks"].toObject(),
        terrainTypeReferenceResolver,
        qJsonValueToInt
    );
    this->defenses = fromQJsonObject<QMap<const TerrainType *, int>>(
        obj["terrainDefenses"].toObject(),
        terrainTypeReferenceResolver,
        qJsonValueToInt
    );
}

void UnitClass::dataToJson(QJsonObject &obj) const
{
    obj["movementPoints"] = this->movementPoints;
    obj["terrainMovementCosts"] = toQJsonObject(
        this->movementCosts,
        qObjectName,
        constructQJsonValue<int>
    );
    obj["terrainAttacks"] = toQJsonObject(
        this->attacks,
        qObjectName,
        constructQJsonValue<int>
    );
    obj["terrainDefenses"] = toQJsonObject(
        this->defenses,
        qObjectName,
        constructQJsonValue<int>
    );
}

