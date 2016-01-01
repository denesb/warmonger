#include "core/SettlementType.h"
#include "core/UnitType.h"
#include "core/JsonUtil.h"

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
    return toQVariantList<UnitType *>(this->recruits);
}

void SettlementType::writeRecruits(QVariantList recruits)
{
    QList<UnitType *> r = fromQVariantList<UnitType *>(recruits);
    this->setRecruits(r);
}

void SettlementType::dataFromJson(const QJsonObject &obj)
{
    this->goldPerTurn = obj["goldPerTurn"].toInt();
    this->recruits = referenceListFromJson<UnitType>(
        obj["recruits"].toArray(),
        this->parent()
    );
}

void SettlementType::dataToJson(QJsonObject &obj) const
{
    obj["goldPerTurn"] = this->goldPerTurn;
    obj["recruits"] = referenceListToJson<UnitType>(this->recruits);
}

