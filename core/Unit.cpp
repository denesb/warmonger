#include "core/Map.h"
#include "core/Unit.h"

using namespace warmonger::core;

static const QString loggerName{"core.Unit"};

const QMap<Unit::UnitRank, QString> Unit::rank2str{
    std::make_pair(Unit::Soldier, "soldier"),
    std::make_pair(Unit::Officer, "officer"),
    std::make_pair(Unit::Leader, "leader")
};
const QMap<Unit::UnitRank, QString> Unit::rankNames{
    std::make_pair(Unit::Soldier, "Soldier"),
    std::make_pair(Unit::Officer, "Officer"),
    std::make_pair(Unit::Leader, "Leader")
};
const QMap<QString, Unit::UnitRank> Unit::str2rank{
    std::make_pair("soldier", Unit::Soldier),
    std::make_pair("officer", Unit::Officer),
    std::make_pair("leader", Unit::Leader)
};

Unit::Unit(QObject *parent) :
    QObject(parent),
    rank(Unit::Soldier),
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

Unit::UnitRank Unit::getRank() const
{
    return this->rank;
}

void Unit::setRank(Unit::UnitRank rank)
{
    if (this->rank != rank)
    {
        this->rank = rank;
        emit rankChanged();
    }
}

QString Unit::getRankName() const
{
    return Unit::rankNames[this->rank];
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

double Unit::getExperiencePoints() const
{
    return this->experiencePoints;
}

void Unit::setExperiencePoints(double experiencePoints)
{
    if (this->experiencePoints != experiencePoints)
    {
        this->experiencePoints = experiencePoints;
        emit experiencePointsChanged();
    }
}

double Unit::getHitPoints() const
{
    return this->hitPoints;
}

void Unit::setHitPoints(double hitPoints)
{
    if (this->hitPoints != hitPoints)
    {
        this->hitPoints = hitPoints;
        emit hitPointsChanged();
    }
}

double Unit::getMovementPoints() const
{
    return this->movementPoints;
}

void Unit::setMovementPoints(double movementPoints)
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
