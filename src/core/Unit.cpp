#include "core/Unit.h"
#include "core/UnitType.h"
#include "core/MapNode.h"
#include "core/Player.h"
#include "core/JsonUtil.h"

using namespace warmonger::core;

static const QString category{"core"};

Unit::Unit(QObject *parent) :
    GameObject(parent),
    unitType(nullptr),
    mapNode(nullptr),
    owner(nullptr),
    experiencePoints(0),
    hitPoints(0),
    movementPoints(0)
{
}

Unit::~Unit()
{
}

UnitType * Unit::getUnitType() const
{
    return this->unitType;
}

void Unit::setUnitType(UnitType *unitType)
{
    if (this->unitType != unitType)
    {
        this->unitType = unitType;
        emit unitTypeChanged();
    }
}

QObject * Unit::readUnitType() const
{
    return this->unitType;
}

void Unit::writeUnitType(QObject *unitType)
{
    UnitType *ut = qobject_cast<UnitType *>(unitType);
    if (ut == nullptr)
    {
        wError(category) << "unitType is null or has wrong type";
        throw Exception(Exception::InvalidValue);
    }

    this->setUnitType(ut);
}

MapNode * Unit::getMapNode() const
{
    return this->mapNode;
}

void Unit::setMapNode(MapNode *mapNode)
{
    if (this->mapNode != mapNode)
    {
        this->mapNode = mapNode;
        emit mapNodeChanged();
    }
}

QObject * Unit::readMapNode() const
{
    return this->mapNode;
}

void Unit::writeMapNode(QObject *mapNode)
{
    MapNode *n = qobject_cast<MapNode *>(mapNode);
    if (n == nullptr)
    {
        wError(category) << "mapNode is null or has wrong type";
        throw Exception(Exception::InvalidValue);
    }

    this->setMapNode(n);
}

Player * Unit::getOwner() const
{
    return this->owner;
}

void Unit::setOwner(Player *owner)
{
    if (this->owner != owner)
    {
        this->owner = owner;
        emit ownerChanged();
    }
}

QObject * Unit::readOwner() const
{
    return this->owner;
}

void Unit::writeOwner(QObject *owner)
{
    Player *o = qobject_cast<Player *>(owner);
    if (o == nullptr)
    {
        wError(category) << "owner is null or has wrong type";
        throw Exception(Exception::InvalidValue);
    }
    this->setOwner(o);
}
int Unit::getExperiencePoints() const
{
    return this->experiencePoints;
}

void Unit::setExperiencePoints(int experiencePoints)
{
    if (this->experiencePoints != experiencePoints)
    {
        this->experiencePoints = experiencePoints;
        emit experiencePointsChanged();
    }
}

int Unit::getHitPoints() const
{
    return this->hitPoints;
}

void Unit::setHitPoints(int hitPoints)
{
    if (this->hitPoints != hitPoints)
    {
        this->hitPoints = hitPoints;
        emit hitPointsChanged();
    }
}

int Unit::getMovementPoints() const
{
    return this->movementPoints;
}

void Unit::setMovementPoints(int movementPoints)
{
    if (this->movementPoints != movementPoints)
    {
        this->movementPoints = movementPoints;
        emit movementPointsChanged();
    }
}

void Unit::dataFromJson(const QJsonObject &obj)
{
    this->unitType = resolveReference<UnitType>(
        obj["unitType"].toString(),
        this->parent()
    );
    this->mapNode = resolveReference<MapNode>(
        obj["mapNode"].toString(),
        this->parent()
    );
    this->owner = resolveReference<Player>(
        obj["owner"].toString(),
        this->parent()
    );
    this->hitPoints = obj["hitPoints"].toInt();
    this->movementPoints = obj["movementPoints"].toInt();
    this->experiencePoints = obj["experiencePoints"].toInt();
}

void Unit::dataToJson(QJsonObject &obj) const
{
    obj["unitType"] = this->unitType->objectName();
    obj["mapNode"] = this->mapNode->objectName();
    obj["owner"] = this->owner->objectName();
    obj["experiencePoints"] = this->experiencePoints;
    obj["hitPoints"] = this->hitPoints;
    obj["movementPoints"] = this->movementPoints;
}
