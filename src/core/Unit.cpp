#include "core/Unit.h"
#include "core/UnitType.h"
#include "core/MapNode.h"
#include "core/Player.h"
#include "core/JsonUtil.h"

using namespace warmonger::core;

static const QString category{"core"};

const QMap<Unit::UnitRank, QString> Unit::unitRank2str{
    std::make_pair(Unit::Soldier, "soldier"),
    std::make_pair(Unit::Officer, "officer"),
    std::make_pair(Unit::Leader, "leader")
};
const QMap<QString, Unit::UnitRank> Unit::str2unitRank{
    std::make_pair("soldier", Unit::Soldier),
    std::make_pair("officer", Unit::Officer),
    std::make_pair("leader", Unit::Leader)
};

Unit::Unit(QObject *parent) :
    GameObject(parent),
    unitRank(Unit::Soldier),
    unitType(nullptr),
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

Unit::UnitRank Unit::getUnitRank() const
{
    return this->unitRank;
}

void Unit::setUnitRank(Unit::UnitRank unitRank)
{
    this->unitRank = unitRank;
}

UnitType * Unit::getUnitType() const
{
    return this->unitType;
}

void Unit::setUnitType(UnitType *unitType)
{
    if (this->unitType != unitType)
    {
        const UnitType *oldUnitType = this->unitType;
        this->unitType = unitType;

        this->onUnitTypeChanged(oldUnitType);
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

void Unit::onUnitTypeChanged(const UnitType *oldUnitType)
{
    int hpPercentage = this->hitPoints * 100 / oldUnitType->getHitPoints();
    this->hitPoints = (this->unitType->getHitPoints() * hpPercentage) / 100;
}

void Unit::dataFromJson(const QJsonObject &obj)
{
    const QString unitRankStr = obj["rank"].toString();
    if (Unit::str2unitRank.contains(unitRankStr))
    {
        this->unitRank = Unit::str2unitRank[unitRankStr];
    }
    else
    {
        wError(category) << "Invalid unit rank: " << unitRankStr;
        throw Exception(Exception::InvalidValue);
    }
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
    this->hitPoints = obj["hitPoints"].toDouble();
    this->movementPoints = obj["movementPoints"].toDouble();
    this->experiencePoints = obj["experiencePoints"].toDouble();
}

void Unit::dataToJson(QJsonObject &obj) const
{
    obj["rank"] = Unit::unitRank2str[this->unitRank];
    obj["unitType"] = this->unitType->objectName();
    obj["mapNode"] = this->mapNode->objectName();
    obj["owner"] = this->owner->objectName();
    obj["experiencePoints"] = this->experiencePoints;
    obj["hitPoints"] = this->hitPoints;
    obj["movementPoints"] = this->movementPoints;
}
