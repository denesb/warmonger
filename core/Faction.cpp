#include <functional>

#include "core/Faction.h"
#include "core/SettlementType.h"
#include "core/QJsonUtil.h"
#include "core/QVariantUtil.h"
#include "core/UnitType.h"

using namespace warmonger::core;

Faction::Faction(QObject *parent) :
    GameObject(parent)
{
}

Faction::~Faction()
{
}

QList<UnitType *> Faction::getUnitTypes() const
{
    return this->unitTypes;
}

QVariantList Faction::readUnitTypes() const
{
    return toQVariantList(this->unitTypes);
}

void Faction::setUnitTypes(const QList<UnitType *> &unitTypes)
{
    if (this->unitTypes != unitTypes)
    {
        this->unitTypes = unitTypes;
        emit unitTypesChanged();
    }
}

void Faction::addUnitType(UnitType *unitType)
{
    this->unitTypes.append(unitType);
    emit unitTypesChanged();
}

QMap<SettlementType *, QList<UnitType *>> Faction::getRecruits() const
{
    return this->recruits;
}

QVariantMap Faction::readRecruits() const
{
    return toQVariantMap(
        this->recruits,
        qObjectName,
        containerToQVariant<QList<UnitType *>>
    );
}

void Faction::setRecruits(
    const QMap<SettlementType *, QList<UnitType *>> &recruits
)
{
    if (this->recruits != recruits)
    {
        this->recruits = recruits;
        emit recruitsChanged();
    }
}

QList<UnitType *> Faction::getRecruitsFor(
    SettlementType *settlementType
) const
{
    return this->recruits[settlementType];
}

bool Faction::canRecruitFrom(
    SettlementType *settlementType,
    UnitType *unitType
) const
{
    return this->recruits[settlementType].contains(unitType);
}

void Faction::dataFromJson(const QJsonObject &obj)
{
    const ReferenceResolver<UnitType> unitTypeReferenceResolver(this->parent());

    this->unitTypes = fromQJsonArray<QList<UnitType *>>(
        obj["unitTypes"].toArray(),
        unitTypeReferenceResolver
    );
    this->recruits = fromQJsonObject<QMap<SettlementType *, QList<UnitType *>>>(
        obj["recruits"].toObject(),
        ReferenceResolver<SettlementType>(this->parent()),
        [&](const QJsonValue &v)
        {
            return fromQJsonArray<QList<UnitType *>>(
                v.toArray(),
                unitTypeReferenceResolver
            );
        }
    );
}

void Faction::dataToJson(QJsonObject &obj) const
{
    obj["unitTypes"] = toQJsonArray(this->unitTypes, qObjectName);
    obj["recruits"] = toQJsonObject(
        this->recruits,
        qObjectName,
        [](const QList<UnitType *> &l){return toQJsonArray(l, qObjectName);}
    );
}
