#include <QDebug>
#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/World.h"
#include "core/Util.h"
#include "core/Exception.h"
#include "core/Log.h"

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
    World *world = this->parent()->parent()->findChild<World *>(QString(), Qt::FindDirectChildrenOnly);
    if (world == nullptr)
    {
        qCCritical(root) << "world is null";
        throw Exception(Exception::NullPointer);
    }

    const QString settlementTypeName = obj["settlementType"].toString();
    this->settlementType = world->findChild<SettlementType *>(settlementTypeName);
    if (this->settlementType == nullptr)
    {
        qCCritical(root) << "Unable to resolve reference <SettlementType>" << settlementTypeName;
        throw Exception(Exception::UnresolvedReference, {"SettlementType", settlementTypeName});
    }

    this->position = MapPosition(obj["position"].toString());
}

QJsonObject Settlement::toJson() const
{
    QJsonObject obj;

    obj["settlementType"] = this->settlementType->objectName();
    obj["position"] = this->position.toStr();

    return std::move(obj);
}
