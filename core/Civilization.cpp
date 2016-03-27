#include <functional>

#include "core/Civilization.h"
#include "core/SettlementType.h"
#include "core/QVariantUtil.h"
#include "core/UnitType.h"

using namespace warmonger::core;

Civilization::Civilization(QObject *parent) :
    QObject(parent)
{
}

QString Civilization::getDisplayName() const
{
    return this->displayName;
}

void Civilization::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

std::vector<UnitType *> Civilization::getUnitTypes() const
{
    return this->unitTypes;
}

QVariantList Civilization::readUnitTypes() const
{
    return toQVariantList(this->unitTypes);
}

void Civilization::setUnitTypes(const std::vector<UnitType *> &unitTypes)
{
    if (this->unitTypes != unitTypes)
    {
        this->unitTypes = unitTypes;
        emit unitTypesChanged();
    }
}

void Civilization::addUnitType(UnitType *unitType)
{
    this->unitTypes.push_back(unitType);
    emit unitTypesChanged();
}

std::map<SettlementType *, std::vector<UnitType *>> Civilization::getRecruits() const
{
    return this->recruits;
}

QVariantMap Civilization::readRecruits() const
{
    return toQVariantMap(
        this->recruits,
        std::bind(&QObject::objectName, std::placeholders::_1),
        containerToQVariant<std::vector<UnitType *>>
    );
}

void Civilization::setRecruits(const std::map<SettlementType *, std::vector<UnitType *>> &recruits)
{
    if (this->recruits != recruits)
    {
        this->recruits = recruits;
        emit recruitsChanged();
    }
}

std::vector<UnitType *> Civilization::getRecruitsFor(SettlementType *settlementType) const
{
    return this->recruits.at(settlementType);
}

bool Civilization::canRecruitFrom(SettlementType *settlementType, UnitType *unitType) const
{
    const std::vector<UnitType *> rs = this->recruits.at(settlementType);
    return std::find(rs.cbegin(), rs.cend(), unitType) != rs.end();
}
