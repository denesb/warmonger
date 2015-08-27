#include "core/Unit.h"
#include "core/UnitType.h"
#include "core/MapNode.h"
#include "core/World.h"
#include "core/JsonUtil.h"

using namespace warmonger::core;

Unit::Unit(QObject *parent) :
    GameObject(parent),
    unitType(nullptr),
    mapNode(nullptr),
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

const MapNode * Unit::getMapNode() const
{
    return this->mapNode;
}

void Unit::setMapNode(const MapNode *mapNode)
{
    this->mapNode = mapNode;
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

    this->unitType = resolveReference<UnitType>(
        obj["unitType"].toString(),
        world
    );
    this->mapNode = resolveReference<MapNode>(
        obj["mapNode"].toString(),
        this->parent()
    );
    this->hitPoints = obj["hitPoints"].toInt();
    this->movementPoints = obj["movementPoints"].toInt();
    this->experience = obj["experience"].toInt();
}

void Unit::dataToJson(QJsonObject &obj) const
{
    obj["unitType"] = this->unitType->objectName();
    obj["mapNode"] = this->mapNode->objectName();
    obj["hitPoints"] = this->hitPoints;
    obj["movementPoints"] = this->movementPoints;
    obj["experience"] = this->experience;
}
