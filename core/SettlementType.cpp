#include "core/QJsonUtil.h"
#include "core/QVariantUtil.h"
#include "core/SettlementType.h"
#include "core/UnitType.h"

using namespace warmonger::core;

SettlementType::SettlementType(QObject *parent) :
    GameObject(parent),
    goldPerTurn(0),
    recruits()
{
}

SettlementType::~SettlementType()
{
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

void SettlementType::dataFromJson(const QJsonObject &obj)
{
    this->goldPerTurn = obj["goldPerTurn"].toInt();
    this->recruits = fromQJsonArray<QList<UnitType *>>(
        obj["recruits"].toArray(),
        ReferenceResolver<UnitType>(this->parent())
    );
}

void SettlementType::dataToJson(QJsonObject &obj) const
{
    obj["goldPerTurn"] = this->goldPerTurn;
    obj["recruits"] = toQJsonArray(this->recruits, qObjectName);
}

