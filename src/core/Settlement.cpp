#include "core/Settlement.h"
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
