#include "core/Unit.h"
#include "core/UnitType.h"
#include "core/World.h"
#include "core/JsonUtil.hpp"
#include "Util.h"

using namespace warmonger::core;

Unit::Unit(QObject *parent) :
    GameObject(parent),
    unitType(nullptr),
    position(),
    hitPoints(0),
    movementPoints(0),
    experience(0)
{
}

Unit::~Unit()
{
}

const UnitType * Unit::getUnitType() const
{
    return this->unitType;
}

void Unit::setUnitType(const UnitType *unitType)
{
    this->unitType = unitType;
}

MapPosition Unit::getPosition() const
{
    return this->position;
}

void Unit::setPosition(const MapPosition &position)
{
    this->position = position;
}

int Unit::getHitpoints() const
{
    return this->hitPoints;
}

void Unit::setHitpoints(int hitPoints)
{
    this->hitPoints = hitPoints;
}

int Unit::getMovementpoints() const
{
    return this->movementPoints;
}

void Unit::setMovementpoints(int movementPoints)
{
    this->movementPoints = movementPoints;
}

int Unit::getExperience() const
{
    return this->experience;
}

void Unit::setExperience(int experience)
{
    this->experience = experience;
}

void Unit::fromJson(const QJsonObject &obj)
{
    //TODO: error handling
    World *world = this->parent()->findChild<World *>(QString(), Qt::FindDirectChildrenOnly);

    this->unitType = world->findChild<UnitType *>(obj["unitType"].toString());
    this->position = MapPosition(obj["position"].toString());
    this->hitPoints = obj["hitPoints"].toInt();
    this->movementPoints = obj["movementPoints"].toInt();
    this->experience = obj["experience"].toInt();
}

QJsonObject Unit::toJson() const
{
    QJsonObject obj;

    obj["unitType"] = this->unitType->objectName();
    obj["position"] = this->position.toStr();
    obj["hitPoints"] = this->hitPoints;
    obj["movementPoints"] = this->movementPoints;
    obj["experience"] = this->experience;

    return std::move(obj);
}
