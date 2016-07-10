#include "core/SettlementType.h"
#include "core/UnitType.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

SettlementType::SettlementType(QObject *parent) :
    QObject(parent),
    goldPerTurn(0),
    recruits()
{
}

SettlementType::~SettlementType()
{
}

QString SettlementType::getDisplayName() const
{
    return this->displayName;
}

void SettlementType::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

int SettlementType::getGoldPerTurn() const
{
    return this->goldPerTurn;
}

void SettlementType::setGoldPerTurn(int goldPerTurn)
{
    this->goldPerTurn = goldPerTurn;
}

std::vector<UnitType *> SettlementType::getRecruits() const
{
    return this->recruits;
}

void SettlementType::setRecruits(const std::vector<UnitType *> &recruits)
{
    if (this->recruits != recruits)
    {
        this->recruits = recruits;
        emit recruitsChanged();
    }
}

QVariantList SettlementType::readRecruits() const
{
    return utils::toQVariantList(this->recruits);
}

void SettlementType::writeRecruits(QVariantList recruits)
{
    std::vector<UnitType *> r = utils::fromQVariantList<std::vector<UnitType *>>(recruits);
    this->setRecruits(r);
}

} // namespace core
} // namespace warmonger
