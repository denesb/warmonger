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

std::vector<UnitType *> Faction::getUnitTypes() const
{
    return this->unitTypes;
}

QVariantList Faction::readUnitTypes() const
{
    return toQVariantList(this->unitTypes);
}

void Faction::setUnitTypes(const std::vector<UnitType *> &unitTypes)
{
    if (this->unitTypes != unitTypes)
    {
        this->unitTypes = unitTypes;
        emit unitTypesChanged();
    }
}

void Faction::addUnitType(UnitType *unitType)
{
    this->unitTypes.push_back(unitType);
    emit unitTypesChanged();
}

std::map<SettlementType *, std::vector<UnitType *>> Faction::getRecruits() const
{
    return this->recruits;
}

QVariantMap Faction::readRecruits() const
{
    return toQVariantMap(
        this->recruits,
        std::bind(&QObject::objectName, std::placeholders::_1),
        containerToQVariant<std::vector<UnitType *>>
    );
}

void Faction::setRecruits(const std::map<SettlementType *, std::vector<UnitType *>> &recruits)
{
    if (this->recruits != recruits)
    {
        this->recruits = recruits;
        emit recruitsChanged();
    }
}

std::vector<UnitType *> Faction::getRecruitsFor(SettlementType *settlementType) const
{
    return this->recruits.at(settlementType);
}

bool Faction::canRecruitFrom(SettlementType *settlementType, UnitType *unitType) const
{
    const std::vector<UnitType *> rs = this->recruits.at(settlementType);
    return std::find(rs.cbegin(), rs.cend(), unitType) != rs.end();
}
