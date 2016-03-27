#include "core/UnitClass.h"

using namespace warmonger::core;

UnitClass::UnitClass(QObject *parent) :
    QObject(parent),
    movementPoints(0),
    movementCosts(),
    attacks(),
    defenses()
{
}

UnitClass::~UnitClass()
{
}

QString UnitClass::getDisplayName() const
{
    return this->displayName;
}

void UnitClass::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
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

std::map<const TerrainType *, int> UnitClass::getMovementCosts() const
{
    return this->movementCosts;
}

void UnitClass::setMovementCosts(const std::map<const TerrainType *, int> &movementCosts)
{
    this->movementCosts = movementCosts;
}

int UnitClass::getMovementCost(const TerrainType *terrainType) const
{
    return this->movementCosts.at(terrainType);
}

void UnitClass::setMovementCost(const TerrainType *terrainType, int movement)
{
    this->movementCosts[terrainType] = movement;
}

std::map<const TerrainType *, int> UnitClass::getAttacks() const
{
    return this->attacks;
}

void UnitClass::setAttacks(const std::map<const TerrainType *, int> &attacks)
{
    this->attacks = attacks;
}

int UnitClass::getAttack(const TerrainType *terrainType) const
{
    return this->attacks.at(terrainType);
}

void UnitClass::setAttack(const TerrainType *terrainType, int attack)
{
    this->attacks[terrainType] = attack;
}

std::map<const TerrainType *, int> UnitClass::getDefenses() const
{
    return this->defenses;
}

void UnitClass::setDefenses(const std::map<const TerrainType *, int> &defenses)
{
    this->defenses = defenses;
}

int UnitClass::getDefense(const TerrainType *terrainType) const
{
    return this->defenses.at(terrainType);
}

void UnitClass::setDefense(const TerrainType *terrainType, int defense)
{
    this->defenses[terrainType] = defense;
}
