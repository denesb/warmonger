#include <functional>

#include "core/Faction.h"
#include "core/JsonUtil.h"
#include "core/SettlementType.h"
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
    return toQVariantList<UnitType *>(this->unitTypes);
}

void Faction::setUnitTypes(const QList<UnitType *> &unitTypes)
{
    if (this->unitTypes != unitTypes)
    {
        this->unitTypes = unitTypes;
        emit unitTypesChanged();
    }
}

QMap<SettlementType *, QList<UnitType *>> Faction::getRecruits() const
{
    return this->recruits;
}

QVariantMap Faction::readRecruits() const
{
    return toQVariantMap(
        this->recruits.constBegin(),
        this->recruits.constEnd(),
        std::bind(&QObject::objectName, std::placeholders::_1),
        listToQVariant<UnitType *>
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
    std::function<QList<UnitType *>(const QJsonArray &)> unitTypesFromJsonFunc
        = std::bind(
        referenceListFromJson<UnitType>,
        std::placeholders::_1,
        this->parent()
    );

    this->unitTypes = unitTypesFromJsonFunc(obj["unitTypes"].toArray());
    this->recruits = mapFromJson<QMap<SettlementType *, QList<UnitType *>>>(
        obj["recruits"].toObject(),
        std::bind(
            resolveReference<SettlementType>,
            std::placeholders::_1,
            this->parent()
        ),
        [&](const QJsonValue &v){return unitTypesFromJsonFunc(v.toArray());}
    );
}

void Faction::dataToJson(QJsonObject &obj) const
{
    obj["unitTypes"] = referenceListToJson<UnitType>(this->unitTypes);
}
