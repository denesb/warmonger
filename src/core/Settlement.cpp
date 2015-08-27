#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/MapNode.h"
#include "core/World.h"
#include "core/Util.h"

using namespace warmonger::core;

Settlement::Settlement(QObject *parent) :
    GameObject(parent),
    settlementType(nullptr),
    mapNode(nullptr)
{
}

Settlement::~Settlement()
{
}

const SettlementType * Settlement::getSettlementType() const
{
    return this->settlementType;
}

void Settlement::setSettlementType(const SettlementType *settlementType)
{
    this->settlementType = settlementType;
}

const MapNode * Settlement::getMapNode() const
{
    return this->mapNode;
}

void Settlement::setMapNode(const MapNode *mapNode)
{
    this->mapNode = mapNode;
}

void Settlement::dataFromJson(const QJsonObject &obj)
{
    World *world = this->parent()->findChild<World *>(QString(), Qt::FindDirectChildrenOnly);
    if (world == nullptr)
    {
        wError("core.Settlement") << "world is null";
        throw Exception(Exception::NullPointer);
    }

    this->settlementType = resolveReference<SettlementType>(
        obj["settlementType"].toString(), world
    );
    this->mapNode = resolveReference<MapNode>(
        obj["mapNode"].toString(), this->parent()
    );
}

void Settlement::dataToJson(QJsonObject &obj) const
{
    obj["settlementType"] = this->settlementType->objectName();
    obj["mapNode"] = this->mapNode->objectName();
}
