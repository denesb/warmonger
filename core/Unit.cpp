#include "core/Unit.h"

using namespace warmonger::core;

static const QString loggerName{"core.Unit"};

Unit::Unit(QObject *parent) :
    QObject(parent),
    type(nullptr),
    mapNode(nullptr),
    owner(nullptr),
    experiencePoints(0.0),
    hitPoints(0.0),
    movementPoints(0.0)
{
}

Unit::~Unit()
{
}

QString Unit::getDisplayName() const
{
    return this->displayName;
}

void Unit::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

UnitType * Unit::getType() const
{
    return this->type;
}

void Unit::setType(UnitType *type)
{
    if (this->type != type)
    {
        const UnitType *oldType = this->type;
        this->type = type;

        this->onTypeChanged(oldType);
        emit typeChanged();
    }
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

void Unit::onTypeChanged(const UnitType *oldUnitType)
{
    if (oldUnitType == nullptr)
    {
        this->hitPoints = this->type->getHitPoints();
    }
    else
    {
        int hpPercentage = this->hitPoints * 100 / oldUnitType->getHitPoints();
        this->hitPoints = (this->type->getHitPoints() * hpPercentage) / 100;
    }
}
