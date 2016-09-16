#include <functional>

#include "core/Civilization.h"
#include "core/UnitType.h"
#include "utils/QVariantUtils.h"

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
    return utils::toQVariantList(this->unitTypes);
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
