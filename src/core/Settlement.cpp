#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/World.h"
#include "core/JsonUtil.hpp"

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

QPoint Settlement::getPosition() const
{
    return this->position;
}

void Settlement::setPosition(const QPoint &position)
{
    this->position = position;
}

void Settlement::fromJson(const QJsonObject &obj)
{
    //TODO: error handling
    World *world = this->parent()->findChild<World *>(QString(), Qt::FindDirectChildrenOnly);

    this->settlementType = world->findChild<SettlementType *>(obj["settlementType"].toString());
    this->position = core::fromJson(obj["position"].toObject());
}

QJsonObject Settlement::toJson() const
{
    QJsonObject obj;

    obj["settlementType"] = this->settlementType->objectName();
    obj["position"] = core::toJson(this->position);

    return std::move(obj);
}
