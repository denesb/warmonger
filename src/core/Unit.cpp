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

void Unit::dataFromJson(const QJsonObject &obj)
{
    World *world = this->parent()->findChild<World *>(QString(), Qt::FindDirectChildrenOnly);
    if (world == nullptr)
    {
        wError("core.Unit") << "world is null";
        throw Exception(Exception::NullPointer);
    }

    const QString unitTypeName = obj["unitType"].toString();
    this->unitType = world->findChild<UnitType *>(unitTypeName);
    if (this->unitType == nullptr)
    {
        wError("core.Unit") << "Unable to resolve reference <UnitType>" << unitTypeName;
        throw Exception(Exception::UnresolvedReference, {"UnitType", unitTypeName});
    }

    this->position = MapPosition(obj["position"].toString());
    this->hitPoints = obj["hitPoints"].toInt();
    this->movementPoints = obj["movementPoints"].toInt();
    this->experience = obj["experience"].toInt();
}

void Unit::dataToJson(QJsonObject &obj) const
{
    obj["unitType"] = this->unitType->objectName();
    obj["position"] = this->position.toStr();
    obj["hitPoints"] = this->hitPoints;
    obj["movementPoints"] = this->movementPoints;
    obj["experience"] = this->experience;
}
