#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/MapNode.h"
#include "core/Player.h"
#include "core/Util.h"

using namespace warmonger::core;

static const QString category{"core"};

Settlement::Settlement(QObject *parent) :
    GameObject(parent),
    type(nullptr),
    mapNode(nullptr),
    owner(nullptr)
{
}

Settlement::~Settlement()
{
}

SettlementType * Settlement::getType() const
{
    return this->type;
}

void Settlement::setType(SettlementType *type)
{
    if (this->type != type)
    {
        this->type = type;
        emit typeChanged();
    }
}

QObject * Settlement::readType() const
{
    return this->type;
}

void Settlement::writeType(QObject *type)
{
    SettlementType *st = qobject_cast<SettlementType *>(type);
    if (st == nullptr)
    {
        wError(category) << "type is null or has wrong type";
        throw Exception(Exception::InvalidValue);
    }

    this->setType(st);
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
    this->type = resolveReference<SettlementType>(
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
}

void Settlement::dataToJson(QJsonObject &obj) const
{
    obj["type"] = this->type->objectName();
    obj["mapNode"] = this->mapNode->objectName();
    obj["owner"] = this->owner->objectName();
}
