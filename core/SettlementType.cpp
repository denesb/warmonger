#include "core/QVariantUtil.h"
#include "core/SettlementType.h"
#include "core/UnitType.h"

using namespace warmonger::core;

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

QList<UnitType *> SettlementType::getRecruits() const
{
    return this->recruits;
}

void SettlementType::setRecruits(const QList<UnitType *> &recruits)
{
    if (this->recruits != recruits)
    {
        this->recruits = recruits;
        emit recruitsChanged();
    }
}

QVariantList SettlementType::readRecruits() const
{
    return toQVariantList(this->recruits);
}

void SettlementType::writeRecruits(QVariantList recruits)
{
    QList<UnitType *> r = fromQVariantList<QList<UnitType *>>(recruits);
    this->setRecruits(r);
}
