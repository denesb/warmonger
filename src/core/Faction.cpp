#include "core/Faction.h"
#include "core/UnitType.h"
#include "core/JsonUtil.h"

using namespace warmonger::core;

Faction::Faction(QObject *parent) :
    GameObject(parent)
{
}

Faction::~Faction()
{
}

QList<const UnitType *> Faction::getUnitTypes() const
{
    return this->unitTypes;
}

void Faction::setUnitTypes(const QList<const UnitType *> &unitTypes)
{
    this->unitTypes = unitTypes;
}

void Faction::dataFromJson(const QJsonObject &obj)
{
    this->unitTypes = referenceListFromJson<UnitType>(obj["unitTypes"].toArray(), this);
}

void Faction::dataToJson(QJsonObject &obj) const
{
    obj["unitTypes"] = referenceListToJson<UnitType>(this->unitTypes);
}
