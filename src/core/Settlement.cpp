#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/World.h"
#include "core/Util.h"
#include "core/Error.h"

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

void Settlement::fromJson(const QJsonObject &obj)
{
    World *world = this->parent()->findChild<World *>(QString(), Qt::FindDirectChildrenOnly);
    if (world == nullptr)
        throw Error(Error::NullPointer);

    this->settlementType = world->findChild<SettlementType *>(obj["settlementType"].toString());
    if (this->settlementType == nullptr)
        throw Error(Error::UnresolvedReference, {"SettlementType", obj["settlementType"].toString()});

    this->position = MapPosition(obj["position"].toString());
}

QJsonObject Settlement::toJson() const
{
    QJsonObject obj;

    obj["settlementType"] = this->settlementType->objectName();
    obj["position"] = this->position.toStr();

    return std::move(obj);
}
