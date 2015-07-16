#include "core/SettlementType.h"
#include "core/UnitType.h"
#include "core/JsonUtil.hpp"

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

QList<const UnitType *> SettlementType::getRecruits() const
{
    return this->recruits;
}

void SettlementType::setRecruits(const QList<const UnitType *> &recruits)
{
    this->recruits = recruits;
}

void SettlementType::dataFromJson(const QJsonObject &obj)
{
    this->goldPerTurn = obj["goldPerTurn"].toInt();
    this->recruits = referenceListFromJson<UnitType>(obj["recruits"].toArray(), this);
}

void SettlementType::dataToJson(QJsonObject &obj) const
{
    obj["goldPerTurn"] = this->goldPerTurn;
    obj["recruits"] = referenceListToJson<UnitType>(this->recruits);
}

