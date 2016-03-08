#include <functional>

#include "core/Faction.h"
#include "core/SettlementType.h"
#include "core/QVariantUtil.h"
#include "core/UnitType.h"

using namespace warmonger::core;

Faction::Faction(QObject *parent) :
    QObject(parent)
{
}

Faction::~Faction()
{
}

QString Faction::getDisplayName() const
{
    return this->displayName;
}

void Faction::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
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
        std::bind(&QObject::objectName, std::placeholders::_1),
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
