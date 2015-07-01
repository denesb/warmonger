#include "core/Faction.h"
#include "core/JsonUtil.hpp"

using namespace core;

Faction::Faction(QObject *parent) :
    WorldItem(parent)
{
}

Faction::~Faction()
{
}

QList<UnitType *> Faction::getUnitTypes() const
{
    return this->unitTypes;
}

void Faction::setUnitTypes(const QList<UnitType *> &unitTypes)
{
    this->unitTypes = unitTypes;
}

void Faction::fromJson(const QJsonObject &obj)
{
    WorldItem::fromJson(obj);
    this->unitTypes = referenceListFromJson<UnitType>(obj["unitTypes"].toArray(), this);
}

QJsonObject Faction::toJson() const
{
    QJsonObject &&obj = WorldItem::toJson();

    obj["unitTypes"] = referenceListToJson<UnitType>(this->unitTypes);

    return std::move(obj);
}
