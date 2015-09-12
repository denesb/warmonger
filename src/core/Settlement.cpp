#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/MapNode.h"
#include "core/Player.h"
#include "core/Util.h"

using namespace warmonger::core;

static const QString category{"core"};

Settlement::Settlement(QObject *parent) :
    GameObject(parent),
    settlementType(nullptr),
    mapNode(nullptr),
    owner(nullptr)
{
}

Settlement::~Settlement()
{
}

SettlementType * Settlement::getSettlementType() const
{
    return this->settlementType;
}

void Settlement::setSettlementType(SettlementType *settlementType)
{
    if (this->settlementType != settlementType)
    {
        this->settlementType = settlementType;
        emit settlementTypeChanged();
    }
}

QObject * Settlement::readSettlementType() const
{
    return this->settlementType;
}

void Settlement::writeSettlementType(QObject *settlementType)
{
    SettlementType *st = qobject_cast<SettlementType *>(settlementType);
    if (st == nullptr)
    {
        wError(category) << "settlementType is null or has wrong type";
        throw Exception(Exception::InvalidValue);
    }

    this->setSettlementType(st);
}

MapNode * Settlement::getMapNode() const
{
    return this->mapNode;
}

void Settlement::setMapNode(MapNode *mapNode)
{
    if (this->mapNode != mapNode)
    {
        this->mapNode = mapNode;
        emit mapNodeChanged();
    }
}

QObject * Settlement::readMapNode() const
{
    return this->mapNode;
}

void Settlement::writeMapNode(QObject *mapNode)
{
    MapNode *n = qobject_cast<MapNode *>(mapNode);
    if (n == nullptr)
    {
        wError(category) << "mapNode is null or has wrong type";
        throw Exception(Exception::InvalidValue);
    }

    this->setMapNode(n);
}

Player * Settlement::getOwner() const
{
    return this->owner;
}

void Settlement::setOwner(Player *owner)
{
    if (this->owner != owner)
    {
        this->owner = owner;
        emit ownerChanged();
    }
}

QObject * Settlement::readOwner() const
{
    return this->owner;
}

void Settlement::writeOwner(QObject *owner)
{
    Player *o = qobject_cast<Player *>(owner);
    if (o == nullptr)
    {
        wError(category) << "owner is null or has wrong type";
        throw Exception(Exception::InvalidValue);
    }
    this->setOwner(o);
}

void Settlement::dataFromJson(const QJsonObject &obj)
{
    this->settlementType = resolveReference<SettlementType>(
        obj["settlementType"].toString(),
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
}

void Settlement::dataToJson(QJsonObject &obj) const
{
    obj["settlementType"] = this->settlementType->objectName();
    obj["mapNode"] = this->mapNode->objectName();
    obj["owner"] = this->owner->objectName();
}
