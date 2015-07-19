#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/World.h"
#include "core/Util.h"
#include "core/Exception.h"

using namespace warmonger::core;

Settlement::Settlement(QObject *parent) :
    GameObject(parent),
    settlementType(nullptr),
    position()
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

MapPosition Settlement::getPosition() const
{
    return this->position;
}

void Settlement::setPosition(const MapPosition &position)
{
    this->position = position;
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
        wError("core.Settlement") << "Unable to resolve reference <SettlementType>" << settlementTypeName;
        throw Exception(Exception::UnresolvedReference, {"SettlementType", settlementTypeName});
    }

    this->position = MapPosition(obj["position"].toString());
}

void Settlement::dataToJson(QJsonObject &obj) const
{
    obj["settlementType"] = this->settlementType->objectName();
    obj["position"] = this->position.toStr();
}
