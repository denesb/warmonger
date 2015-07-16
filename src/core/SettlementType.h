#ifndef CORE_SETTLEMENT_TYPE_H
#define CORE_SETTLEMENT_TYPE_H

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class UnitType;

class SettlementType :
    public GameObject
{
    Q_OBJECT;

public:
    SettlementType(QObject *parent);
    ~SettlementType();

    int getGoldPerTurn() const;
    void setGoldPerTurn(int goldPerTurn);

    QList<const UnitType *> getRecruits() const;
    void setRecruits(const QList<const UnitType *> &recruits);

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    int goldPerTurn;
    QList<const UnitType *> recruits;
};

} // namespace core
} // namespace warmonger

#endif // CORE_SETTLEMENT_TYPE_H
