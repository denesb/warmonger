#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/MapNode.h"
#include "core/World.h"
#include "core/Util.h"
#include "core/Exception.h"

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

    const QString settlementTypeName = obj["settlementType"].toString();
    this->settlementType = world->findChild<SettlementType *>(settlementTypeName);
    if (this->settlementType == nullptr)
    {
        Exception e(Exception::UnresolvedReference, {"SettlementType", settlementTypeName});
        wError("core.Settlement") << e.getMessage();
        throw e;
    }

    const QString mapNodeName = obj["mapNode"].toString();
    this->mapNode = this->parent()->findChild<MapNode *>(mapNodeName);
    if (this->mapNode == nullptr)
    {
        Exception e(Exception::UnresolvedReference, {"MapNode", mapNodeName});
        wError("core.Settlement") << e.getMessage();
        throw e;
    }
}

void Settlement::dataToJson(QJsonObject &obj) const
{
    obj["settlementType"] = this->settlementType->objectName();
    obj["mapNode"] = this->mapNode->objectName();
}
