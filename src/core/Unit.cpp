#include "core/Unit.h"
#include "core/UnitType.h"
#include "core/MapNode.h"
#include "core/Player.h"
#include "core/JsonUtil.h"

using namespace warmonger::core;

static const QString category{"core"};

const QMap<Unit::UnitRank, QString> Unit::rank2str{
    std::make_pair(Unit::Soldier, "soldier"),
    std::make_pair(Unit::Officer, "officer"),
    std::make_pair(Unit::Leader, "leader")
};
const QMap<QString, Unit::UnitRank> Unit::str2rank{
    std::make_pair("soldier", Unit::Soldier),
    std::make_pair("officer", Unit::Officer),
    std::make_pair("leader", Unit::Leader)
};

Unit::Unit(QObject *parent) :
    GameObject(parent),
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

Unit::UnitRank Unit::getRank() const
{
    return this->rank;
}

void Unit::setRank(Unit::UnitRank rank)
{
    this->rank = rank;
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

QObject * Unit::readType() const
{
    return this->type;
}

void Unit::writeType(QObject *type)
{
    UnitType *ut = qobject_cast<UnitType *>(type);
    if (ut == nullptr)
    {
        wError(category) << "type is null or has wrong type";
        throw Exception(Exception::InvalidValue);
    }

    this->setType(ut);
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

void Unit::onTypeChanged(const UnitType *oldUnitType)
{
    int hpPercentage = this->hitPoints * 100 / oldUnitType->getHitPoints();
    this->hitPoints = (this->type->getHitPoints() * hpPercentage) / 100;
}

void Unit::dataFromJson(const QJsonObject &obj)
{
    const QString rankStr = obj["rank"].toString();
    if (Unit::str2rank.contains(rankStr))
    {
        this->rank = Unit::str2rank[rankStr];
    }
    else
    {
        wError(category) << "Invalid unit rank: " << rankStr;
        throw Exception(Exception::InvalidValue);
    }
    this->type = resolveReference<UnitType>(
        obj["type"].toString(),
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
    obj["rank"] = Unit::rank2str[this->rank];
    obj["type"] = this->type->objectName();
    obj["mapNode"] = this->mapNode->objectName();
    obj["owner"] = this->owner->objectName();
    obj["experiencePoints"] = this->experiencePoints;
    obj["hitPoints"] = this->hitPoints;
    obj["movementPoints"] = this->movementPoints;
}
